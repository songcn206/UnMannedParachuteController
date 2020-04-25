/*
 * AbsImu.h
 *
 * Created: 28.01.2020 18:38:11
 *  Author: timot
 */ 


#ifndef ABSSPI_HPP_
#define ABSSPI_HPP_

#include "HAL/System/System.hpp"
#include "HAL/UART/UART.hpp"
#include "HAL/Timers/GenericTimer/GenTimerE0.hpp"

class AbsSpi {
	public:
		enum class SpiState {
			SendDummyData,
			ReadData,
			DataUpdated,
			ApplyingSettings,
			Initing,
			Uninited, 
			Wait	
		};
		
		struct BaroSettings {
			uint8_t addr;
			uint8_t value;
		};
		
		static constexpr uint8_t AbsSpiInterruptLevel = (uint8_t)System :: IntLevel :: Med;
		
		
	private:
		static volatile bool dataWritten;
		static volatile SpiState state;
		static volatile uint8_t data[3];
		static volatile uint8_t dataPointer;
		static volatile uint8_t DataAddr[3];
		static volatile BaroSettings settings[1];
		static volatile uint8_t settingsPointer;
	
	public:
		static void Init() {
			SPID.CTRL =  SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_3_gc | SPI_PRESCALER_DIV128_gc;
			SPID.INTCTRL = SPI_INTLVL_MED_gc;
			SPID.STATUS = SPI_IF_bm;
			CheckCommunication();
		}
		
		static void InterruptHandler() {
			switch(state) {
				case SpiState::ApplyingSettings:
					if (dataWritten) {
						AbsBaroCS :: SetHigh();
						if (settingsPointer >= (sizeof(settings) / sizeof(BaroSettings))) {
							state = SpiState::Wait;
							GenTimerE0 :: StartAbsBaroCommunication();
							settingsPointer = 0;
						} else {
							AbsBaroCS :: SetLow();
							SPID.DATA = settings[settingsPointer].addr;
							dataWritten = false;
						}
					} else {
						SPID.DATA = settings[settingsPointer].value;
						settingsPointer++;
						dataWritten = true;
					}
					break;
				case SpiState::DataUpdated:
				case SpiState::Wait:
					break;
				case SpiState::Initing:
					if (dataWritten) {
						AbsBaroCS :: SetHigh();
						uint8_t ID = SPID.DATA;

						if (ID == 0b10110001) {
							ExtUart :: SendString("Got AbsBaro ID\n");
							StartApplyingSettings();
						} else {
							//ExtUart :: SendString("Asking AbsBaro ID\n");
							CheckCommunication();	
						}
					} else {
						SPID.DATA = 0xff;
						dataWritten = true;
					}
					break;
				case SpiState::ReadData:
					data[dataPointer] = SPID.DATA;
					AbsBaroCS :: SetHigh();
					if (dataPointer >= (sizeof(DataAddr) - 1)) {
						dataPointer = 0;
						state = SpiState::DataUpdated;
					} else {
						dataPointer++;
						AbsBaroCS :: SetLow();
						SPID.DATA = (1 << 7) | DataAddr[dataPointer];
						state = SpiState::SendDummyData;
					}
					break;
				case SpiState::SendDummyData:
					SPID.DATA = 0xff;
					state = SpiState::ReadData;
					break;
				default:
					System :: Halt("Unhandeled state in AbsBaro\n");
			}
			
			
		}
		
		static void CheckCommunication() {
			//System :: DisableInterruptsByPriority(ImuSpiInterruptLevel);
			state = SpiState :: Initing;
			AbsBaroCS :: SetLow();
			SPID.DATA = 0x0f | (1 << 7);
			dataWritten = false;
			//System :: EnableInterruptsByPriority(ImuSpiInterruptLevel);
		}
		
		static void StartApplyingSettings() {
			state = SpiState :: ApplyingSettings;
			AbsBaroCS :: SetLow();
			SPID.DATA = settings[0].addr;
			dataWritten = false;
		}
		
		static void StartDataCommunication() {
			state = SpiState::SendDummyData;
			AbsBaroCS :: SetLow();
			SPID.DATA = DataAddr[0] | (1 << 7);
		}
		
		static volatile uint8_t* GetDataPointer() {
			return &data[0];
		}
		
		static SpiState GetState() {
			return state;
		}
		
		static void ChangeStateToWait() {
			state = SpiState::Wait;
		}
};


#endif /* ABSIMU_H_ */