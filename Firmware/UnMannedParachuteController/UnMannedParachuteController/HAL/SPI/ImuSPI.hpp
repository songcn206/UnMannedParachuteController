/*
 * SPI.h
 *
 * Created: 09.01.2020 21:02:44
 *  Author: timot
 */ 


#ifndef IMUSPI_HPP_
#define IMUSPI_HPP_

#include "HAL/UART/UART.hpp"
#include "HAL/System/System.hpp"
#include "HAL/Timers/GenericTimer/GenTimerD0.hpp"

class ImuSpi {
	public:
		enum class SpiState {
			SendDummyData,
			ReadDataFromRegister,
			Wait,
			Uninited,
			DataUpdated,
			ApplyingSettings,
			UpdatingMagnetometer
		};
		
		struct ImuSettings {
			uint8_t addr;
			uint8_t value;
		};
		
		static constexpr uint8_t ImuSpiInterruptLevel = (uint8_t)System :: IntLevel :: High;
		
	private:

		static volatile SpiState state;
		static volatile uint8_t data[18];
		static volatile uint8_t dataPointer;
		static volatile uint8_t DataAddr[18];
		static volatile ImuSettings settings[19];
		static volatile uint8_t settingsPointer;
		static volatile bool isLastSettingApplied;
		static volatile ImuSettings magnetometer[5];
		static volatile uint8_t magnetometerPointer;
	
	public:
		static void Init() {
			SPIE.CTRL =  SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_3_gc | SPI_PRESCALER_DIV128_gc;
			SPIE.INTCTRL = SPI_INTLVL_HI_gc;
			SPIE.STATUS = SPI_IF_bm;
			state = SpiState :: ApplyingSettings;
			SetSettings();	
		}
		
		static void InterruptHandler() {
			led2 :: Toggle();
			switch(state) {
				case SpiState::SendDummyData:
					SPIE.DATA = 0xff;
					state = SpiState::ReadDataFromRegister;
					break;
				case SpiState::ReadDataFromRegister:
					data[dataPointer] = SPIE.DATA;
					ImuCS :: SetHigh();
					if (dataPointer >= 17) {
						dataPointer = 0;
						state = SpiState::DataUpdated;
						
					} else {
						dataPointer++;
						ImuCS :: SetLow();
						SPIE.DATA = (1 << 7) | DataAddr[dataPointer];
						state = SpiState::SendDummyData;
					}
					break;
				case SpiState::ApplyingSettings:
					if (isLastSettingApplied) {
						ExtUart :: SendString("Set ");
						ImuCS :: SetHigh();
						if (settingsPointer >= (sizeof(settings) / sizeof(ImuSettings))) {
							GenTimerD0 :: StartImuDataCommunication();
							state = SpiState :: Wait;
							settingsPointer = 0;
						} else {
							ImuCS :: SetLow();
							SPIE.DATA = settings[settingsPointer].addr;
							isLastSettingApplied = false;
						}
					} else {
						SPIE.DATA = settings[settingsPointer].value;
						settingsPointer++;
						isLastSettingApplied = true;
					}
					break;
				case SpiState::UpdatingMagnetometer:
					if (isLastSettingApplied) {
						ImuCS :: SetHigh();
						if (magnetometerPointer >= (sizeof(magnetometer) / sizeof(ImuSettings))) {
							ReadIMURegistersValues();
							magnetometerPointer = 0;
						} else {
							ImuCS :: SetLow();
							SPIE.DATA = magnetometer[magnetometerPointer].addr;
							isLastSettingApplied = false;
						}
					} else {
						SPIE.DATA = magnetometer[magnetometerPointer].value;
						magnetometerPointer++;
						isLastSettingApplied = true;
					}
					break;
				default:
					break;
			}
		}
		
		
		static void SetSettings() {
			//ExtUart :: SendString("Start settings\n");
			ImuCS :: SetLow();
			SPIE.DATA = settings[0].addr;
			isLastSettingApplied = false;
			//ExtUart :: SendString("Send addr\n");
		}
		
		static SpiState GetState() {
			return state;
		}
		
		static volatile uint8_t* GetData() {
			return &data[0];
		}

		static void ReadIMURegistersValues() {
			ImuCS :: SetLow();
			SPIE.DATA = (1 << 7) | DataAddr[0];
			state = SpiState::SendDummyData;
		}
		
		static void UpdateMagnetometerRegisters() {
			System :: DisableInterruptsByPriority((System :: IntLevel)ImuSpiInterruptLevel);
			ImuCS :: SetLow();
			SPIE.DATA = magnetometer[0].addr;
			state = SpiState::UpdatingMagnetometer;
			isLastSettingApplied = false;
			System :: EnableInterruptsByPriority((System :: IntLevel)ImuSpiInterruptLevel);
		}
		
		static void DataTransferCompleted() {
			state = SpiState::Wait;			
		}
		
};



#endif /* SPI_H_ */