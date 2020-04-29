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
			SPIC.CTRL =  SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_3_gc | SPI_PRESCALER_DIV16_gc;
			SPIC.INTCTRL = SPI_INTLVL_MED_gc;
			SPIC.STATUS = SPI_IF_bm;
			CheckCommunication();
		}
	
		static void InterruptHandler() {
			switch(state) {
				case SpiState :: ApplyingSettings:
					EepromCS :: SetHigh();
					StartWriteData();
					break;
				case SpiState :: Wait:
				case SpiState :: DataUpdated:
					break;
				case SpiState :: Initing:
					if (!(bool)addressWrites) {
						if (sendDummyData) {
							SPIC.DATA = 0x00;
							sendDummyData = false;
						} else {
							EepromCS :: SetHigh();
							uint8_t ID = SPIC.DATA;
							if (ID == 0b00100000) { // ID from datasheet
								ExtUart :: SendString("Got EEPROM ID\n");
								state = SpiState :: Wait;
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
							SPIC.DATA = 0x10;
							sendDummyData = false;
						} else {
							if (byteCount == 1) {
								EepromCS :: SetHigh();
								state = SpiState :: DataUpdated;
								*arrayPointer = SPIC.DATA;
							} else {
								*arrayPointer = SPIC.DATA;
								arrayPointer++;
								byteCount--;
								SPIC.DATA = 0x00;
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
						EepromCS :: SetHigh();
						if (SPIC.DATA & 0x01) {
							state = SpiState :: WriteInProgress;
						} else {
							state = SpiState :: Wait;
						}
					}
					break;
				default:
					System :: Halt("Unhandeled state in EepromSpi\n");
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
	
		static bool ReadDataFromMemory(uint32_t start, volatile uint8_t* array, uint8_t bytes) {
			System :: DisableInterruptsByPriority((System :: IntLevel)EepromSpiInterruptLevel);
			if (state == SpiState :: Wait) {
				state = SpiState :: ReadData;
				startAddress = start;
				byteCount = bytes;
				arrayPointer = array;
				addressWrites = 3;
				sendDummyData = true;
				EepromCS :: SetLow();
				SPIC.DATA = 0b00000011; // Read from memory array
				System :: EnableInterruptsByPriority((System :: IntLevel)EepromSpiInterruptLevel);
				return true;
			}
			System :: EnableInterruptsByPriority((System :: IntLevel)EepromSpiInterruptLevel);
			return false;
		}
		
		static bool WriteDataToMemory(uint32_t start, uint8_t* array, uint8_t bytes) {
			System :: DisableInterruptsByPriority((System :: IntLevel)EepromSpiInterruptLevel);
			if (state == SpiState :: Wait) {
				StartApplyingSettings();
				startAddress = start;
				byteCount = bytes;
				arrayPointer = array;
				addressWrites = 3;
				System :: EnableInterruptsByPriority((System :: IntLevel)EepromSpiInterruptLevel);
				return true;
			}
			System :: EnableInterruptsByPriority((System :: IntLevel)EepromSpiInterruptLevel);
			return false;
		}
		
		static void StartWriteData() {
			state = SpiState :: WriteData;
			EepromCS :: SetLow();
			SPIC.DATA = 0b00000010; // write to memory array
		}
		
		static bool CheckWriteProgress() {
			System :: DisableInterruptsByPriority((System :: IntLevel)EepromSpiInterruptLevel);
			if (state == SpiState :: Wait || state == SpiState::WriteInProgress) {
				state = SpiState :: CheckingWriteProgress;
				sendDummyData = true;
				EepromCS :: SetLow();
				SPIC.DATA = 0b00000101;	// read status register
				System :: EnableInterruptsByPriority((System :: IntLevel)EepromSpiInterruptLevel);
				return true;
			}
			System :: EnableInterruptsByPriority((System :: IntLevel)EepromSpiInterruptLevel);
			return false;
		}
		
		static SpiState GetState() {
			System :: DisableInterruptsByPriority((System :: IntLevel)EepromSpiInterruptLevel);
			SpiState tempState = state;
			System :: EnableInterruptsByPriority((System :: IntLevel)EepromSpiInterruptLevel);
			return tempState;
		}
	
		static void ChangeStateToWait() {
			System :: DisableInterruptsByPriority((System :: IntLevel)EepromSpiInterruptLevel);
			state = SpiState :: Wait;
			System :: EnableInterruptsByPriority((System :: IntLevel)EepromSpiInterruptLevel);
		}
};

#endif /* EEPROMSPI_H_ */