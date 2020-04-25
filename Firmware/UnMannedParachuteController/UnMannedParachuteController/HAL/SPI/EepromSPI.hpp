/*
 * EepromSPI.h
 *
 * Created: 25/04/2020 12:46:05
 *  Author: Timo Tiirats
 */ 


#ifndef EEPROMSPI_HPP_
#define EEPROMSPI_HPP_

#include "HAL/System/System.hpp"
#include "HAL/UART/UART.hpp"

class EepromSpi {
	public:
		enum class SpiState {
			ReadData,
			WriteData,
			DataUpdated,
			ApplyingSettings,
			Initing,
			Uninited,
			Wait,
			CheckingWriteProgress,
			WriteInProgress
		};
	
		struct EepromSettings {
			uint8_t addr;
			uint8_t value;
		};
	
		static constexpr uint8_t EepromSpiInterruptLevel = (uint8_t)System :: IntLevel :: Med;
	
	
	private:
		static volatile uint8_t addressWrites;
		static volatile SpiState state;
		static volatile bool sendDummyData;
		static volatile uint32_t startAddress;
		static volatile uint8_t byteCount;
		static volatile uint8_t* arrayPointer;
		
	
	public:
		static void Init() {
			SPIC.CTRL =  SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_3_gc | SPI_PRESCALER_DIV128_gc;
			SPIC.INTCTRL = SPI_INTLVL_MED_gc;
			SPIC.STATUS = SPI_IF_bm;
			CheckCommunication();
		}
	
		static void InterruptHandler() {
			switch(state) {
				case SpiState::ApplyingSettings:
					EepromCS :: SetHigh();
					state = SpiState :: Wait;
					break;
				case SpiState::Wait:
				case SpiState::DataUpdated:
					break;
				case SpiState::Initing:
					if (!(bool)addressWrites) {
						if (sendDummyData) {
							SPIC.DATA = 0x00;
							sendDummyData = false;
						} else {
							EepromCS :: SetHigh();
							uint8_t ID = SPIC.DATA;
							if (ID == 0b00100000) {
								ExtUart :: SendString("Got EEPROM ID\n");
								StartApplyingSettings();
							} else {
								CheckCommunication();
							}
						}
					} else {
						SPIC.DATA = 0x00;
						addressWrites--;
					}
					break;
				case SpiState :: ReadData:
					if (!(bool)addressWrites) {
						if (sendDummyData) {
							SPIC.DATA = 0x00;
							sendDummyData = false;
						} else {
							if (byteCount > 0) {
								*arrayPointer = SPIC.DATA;
								arrayPointer++;
								SPIC.DATA = 0x00;
								byteCount--;
							} else {
								EepromCS :: SetHigh();
								state = SpiState :: DataUpdated;
							}
						}	
					} else {
						SPIC.DATA = (uint8_t)(startAddress >> (8 * (addressWrites - 1)));
						addressWrites--;
					}
					break;
				case SpiState :: WriteData:
					if (!(bool)addressWrites) {
						if (byteCount > 0) {
							SPIC.DATA = *arrayPointer;
							arrayPointer++;
							byteCount--;
						} else {
							EepromCS :: SetHigh();
							state = SpiState :: WriteInProgress;
						}
					} else {
						SPIC.DATA = (uint8_t)(startAddress >> (8 * (addressWrites - 1)));
						addressWrites--;
					}
					break;
				case SpiState :: CheckingWriteProgress:
					if (sendDummyData) {
						SPIC.DATA = 0x00;
						sendDummyData = false;
					} else {
						if (SPIC.DATA & 0x01) {
							state = SpiState::WriteInProgress;
						} else {
							state = SpiState::Wait;
						}
					}
					break;
				default:
					System :: Halt("Unhandeled state in Eeprom\n");
			}
		}
	
		static void CheckCommunication() {
			state = SpiState :: Initing;
			EepromCS :: SetLow();
			SPIC.DATA = 0b10000011; // Read identification page
			sendDummyData = true;
			addressWrites = 3;
		}
	
		static void StartApplyingSettings() {
			state = SpiState :: ApplyingSettings;
			EepromCS :: SetLow();
			SPIC.DATA = 0b00000110;	// Write enable		
		}
		
		
		static bool ReadDataFromMemory(uint32_t start, uint8_t* array, uint8_t bytes) {
			if (state == SpiState :: Wait) {
				state = SpiState :: ReadData;
				startAddress = start;
				byteCount = bytes;
				arrayPointer = array;
				addressWrites = 3;
				sendDummyData = true;
				EepromCS :: SetLow();
				SPIC.DATA = 0b00000011; // Read from memory array
				return true;
			}
			return false;
		}
		
		static bool WriteDataToMemory(uint32_t start, uint8_t* array, uint8_t bytes) {
			if (state == SpiState :: Wait) {
				state = SpiState :: WriteData;
				startAddress = start;
				byteCount = bytes;
				addressWrites = 3;
				EepromCS :: SetLow();
				SPIC.DATA = 0b00000010; // write to memory array
				return true;
			}
			return false;
		}
		
		static void CheckWriteProgress() {
			state = SpiState :: CheckingWriteProgress;
			sendDummyData = true;
			EepromCS :: SetLow();
			SPIC.DATA = 0b00000101;	// read status register
		}
	
		/*static volatile uint8_t* GetDataPointer() {
			return &data[0];
		}
		
		*/
		static SpiState GetState() {
			return state;
		}
	
		static void ChangeStateToWait() {
			state = SpiState::Wait;
		}
};



#endif /* EEPROMSPI_H_ */