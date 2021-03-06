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
		
		static constexpr uint8_t ImuSpiInterruptLevel = (uint8_t)System :: IntLevel :: Med;
		
	private:
		static volatile SpiState state;
		static volatile uint8_t data[18];
		static volatile uint8_t dataPointer;
		static volatile uint8_t DataAddr[18];
		static volatile ImuSettings settings[16];
		static volatile uint8_t settingsPointer;
		static volatile bool DataWritten;
		static volatile ImuSettings magnetometer[5];
		static volatile uint8_t magnetometerPointer;
		static volatile bool gotIDFirstTime;
		
		static constexpr uint8_t imuID = 0xea; // ID from datasheet
		static constexpr uint8_t imuIDAddr = 0x00;
		static constexpr uint8_t dummyData = 0xff;
	
	public:
		static void Init() {
			SPIE.CTRL =  SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_3_gc | SPI_PRESCALER_DIV64_gc;
			SPIE.INTCTRL = SPI_INTLVL_MED_gc;
			SPIE.STATUS = SPI_IF_bm;
			InitConnection();	
		}
		
		static void InterruptHandler() {
			switch(state) {
				case SpiState :: SendDummyData:
					SPIE.DATA = dummyData;
					state = SpiState :: ReadDataFromRegister;
					break;
				case SpiState :: ReadDataFromRegister:
					data[dataPointer] = SPIE.DATA;
					ImuCS :: SetHigh();
					if (dataPointer >= (sizeof(DataAddr) - 1)) {
						dataPointer = 0;
						state = SpiState :: DataUpdated;
					} else {
						dataPointer++;
						ImuCS :: SetLow();
						SPIE.DATA = (1 << 7) | DataAddr[dataPointer];
						state = SpiState :: SendDummyData;
					}
					break;
				case SpiState :: ApplyingSettings:
					if (DataWritten) {
						ImuCS :: SetHigh();
						if (settingsPointer >= (sizeof(settings) / sizeof(ImuSettings))) {
							GenTimerD0 :: StartImuDataCommunication();
							state = SpiState :: Wait;
							settingsPointer = 0;
						} else {
							ImuCS :: SetLow();
							SPIE.DATA = settings[settingsPointer].addr;
							DataWritten = false;
						}
					} else {
						SPIE.DATA = settings[settingsPointer].value;
						settingsPointer++;
						DataWritten = true;
					}
					break;
				case SpiState :: UpdatingMagnetometer:
					if (DataWritten) {
						ImuCS :: SetHigh();
						if (magnetometerPointer >= (sizeof(magnetometer) / sizeof(ImuSettings))) {
							ReadIMURegistersValues();
							magnetometerPointer = 0;
						} else {
							ImuCS :: SetLow();
							SPIE.DATA = magnetometer[magnetometerPointer].addr;
							DataWritten = false;
						}
					} else {
						SPIE.DATA = magnetometer[magnetometerPointer].value;
						magnetometerPointer++;
						DataWritten = true;
					}
					break;
				case SpiState::Uninited:
					if (DataWritten) {
						ImuCS :: SetHigh();
						if (SPIE.DATA == imuID) {
							ExtUart :: SendString("Got IMU ID\n");
							state = SpiState :: ApplyingSettings;
							SetSettings();
						} else {
							ImuCS :: SetLow();
							SPIE.DATA = (1 << 7 | imuIDAddr);
							DataWritten = false;
						}
					} else {
						SPIE.DATA = dummyData;
						DataWritten = true;
					}
					break;
				default:
					break;
			}
		}
		
		
		static void SetSettings() {
			ImuCS :: SetLow();
			SPIE.DATA = settings[0].addr;
			DataWritten = false;
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
			DataWritten = false;
			System :: EnableInterruptsByPriority((System :: IntLevel)ImuSpiInterruptLevel);
		}
		
		static void DataTransferCompleted() {
			state = SpiState :: Wait;			
		}
		
		static void InitConnection() {
			System :: DisableInterruptsByPriority((System :: IntLevel)ImuSpiInterruptLevel);
			state = SpiState :: Uninited;
			ImuCS :: SetLow();
			SPIE.DATA = (1 << 7 | imuIDAddr);
			DataWritten = false;
			System :: EnableInterruptsByPriority((System :: IntLevel)ImuSpiInterruptLevel);
		}
};



#endif /* SPI_H_ */