/*
 * EEPROM.h
 *
 * Created: 11/02/2020 13:42:12
 *  Author: Timo Tiirats
 */ 


#ifndef EEPROM_HPP_
#define EEPROM_HPP_

#include <avr/eeprom.h>
#include <stdio.h>
#include <string.h>
#include "HAL/System/System.hpp"
#include "HAL/SPI/EepromSPI.hpp"
#include "HAL/EEPROM/EEPROM.hpp"
#include "HAL/System/Pins.hpp"

class InternalEeprom {
	public:
		enum class EepromAddress : uint16_t {
			LastSavedPacket = 0,		
		};
		
	private:
		static constexpr uint16_t eepromStart = EEPROM_START;
		static constexpr uint16_t eepromEnd = EEPROM_END;
	
	public:
		static uint16_t ReadUint16(uint16_t pos) {
			return eeprom_read_word((uint16_t*)eepromStart + pos);
		}
		
		static uint8_t ReadUint8(uint16_t pos) {
			return eeprom_read_byte((uint8_t*)eepromStart + pos);
		}
		
		static void WriteUint16(uint16_t pos, uint16_t value) {
			if (eepromStart + pos >= eepromEnd) {
				System :: Halt("EEPROM OVERFLOW\n");
			}
			eeprom_write_word((uint16_t*)(eepromStart + pos), value);
		}
		
		static void WriteUint8(uint16_t pos, uint8_t value) {
			if (eepromStart + pos >= eepromEnd) {
				System :: Halt("EEPROM OVERFLOW\n");
			}
			eeprom_write_byte((uint8_t*)(eepromStart + pos), value);
		}
};

class ExternalEeprom {
	private:
		static constexpr uint16_t pagesCount = 1024;
		static constexpr uint8_t maxWritesPerPage = 6;
		static constexpr uint16_t bytesPerPage = 256;
		static constexpr uint8_t bytesPerPacket = 41;
		static uint32_t currentAddress;
		static uint8_t writesWithinPage;
		static uint16_t currentPage;
		static uint16_t dataPacketNr; 
		static uint8_t dataToBeSaved[bytesPerPacket];
		volatile static uint8_t dataToBeSent[bytesPerPacket];
		static uint32_t lastPacketNr;
		static bool firstPacket;
		
	public:	
		static bool InitSaving() {
			if (EepromSpi :: GetState() == EepromSpi :: SpiState :: Wait) {
				firstPacket = true;
				dataPacketNr = 0;
				currentAddress = 0;
				currentPage = 0;
				writesWithinPage = 0;
				lastPacketNr = 0;
				return true;
			}
			return false;
		}
	
		static bool SaveData(int16_t accx, int16_t accy, int16_t accz, 
			int16_t gyrox, int16_t gyroy, int16_t gyroz, 
			int16_t magx, int16_t magy, int16_t magz, 
			float gpsLat, float gpsLon, float gpsAlt, uint8_t count,
			uint16_t distance, float pressure, int16_t diffpressure, 
			uint8_t right, uint8_t left, bool retry) {
			
			if (retry) {
				return EepromSpi :: WriteDataToMemory(currentAddress, &dataToBeSaved[0], bytesPerPacket);
			} else {
				CalculateStartingAddress(); // Calculating packet starting address in EEPROM
				
				// Writing data to array
				Write16(0, accx);
				Write16(2, accy);
				Write16(4, accz);
				Write16(6, gyrox);
				Write16(8, gyroy);
				Write16(10, gyroz);
				Write16(12, magx);
				Write16(14, magy);
				Write16(16, magz);
				WriteFloat(18, gpsLat);
				WriteFloat(22, gpsLon);
				WriteFloat(26, gpsAlt);
				dataToBeSaved[30] = count;
				WriteU16(31, distance);
				WriteFloat(33, pressure);
				Write16(37, diffpressure);
				dataToBeSaved[39] = right;
				dataToBeSaved[40] = left;
				
				dataPacketNr++;
				writesWithinPage++;
				lastPacketNr = dataPacketNr;
				
				// Writing array to EEPROM
				return EepromSpi :: WriteDataToMemory(currentAddress, &dataToBeSaved[0], bytesPerPacket);
			}
		}
		
		static void SendData() {
			if (lastPacketNr == 0) { // If we have had power cycle between saving and recovering data from EEPROM
				lastPacketNr = InternalEeprom :: ReadUint16((uint16_t)InternalEeprom :: EepromAddress :: LastSavedPacket);
			}
			
			currentAddress = 0;
			currentPage = 0;
			writesWithinPage = 0;
			uint16_t sentPackets = 0;
			bool nAllSent = true;
			firstPacket = true;
			
			// Wait writing cycle to finish
			while (true) {
				if (EepromSpi :: GetState() == EepromSpi :: SpiState :: WriteInProgress) {
					EepromSpi :: CheckWriteProgress(); // Asking EEPROM if it's ready
					
				} else if (EepromSpi :: GetState() == EepromSpi :: SpiState :: WriteData ||
					EepromSpi :: GetState() == EepromSpi :: SpiState :: ApplyingSettings || 
					EepromSpi :: GetState() == EepromSpi :: SpiState :: Initing || 
					EepromSpi :: GetState() == EepromSpi :: SpiState :: Uninited) {
					asm volatile("nop"); // Eeprom should recover by itselt from these states
					
				} else {
					break;
				}
				
				for (uint8_t i = 0; i < 255; i++) {
					asm volatile("nop"); // For slowing down while loop
				}
			}
			
			while(nAllSent) {
				CalculateStartingAddress();
				EepromSpi :: ReadDataFromMemory(currentAddress, &dataToBeSent[0], bytesPerPacket);
				
				// Waiting one packet from EEPROM
				while(true) {
					if (EepromSpi :: GetState() == EepromSpi :: SpiState :: DataUpdated) {
						break;
					} else {
						for (uint8_t i = 0; i < 255; i++) {
							asm volatile("nop");
						}
					}
				}
				
				// Sending data to UART
				ExtUart :: SendString("AX ");
				ExtUart :: SendInt(Read16(0));
				ExtUart :: SendString(" AY ");
				ExtUart :: SendInt(Read16(2));
				ExtUart :: SendString(" AZ ");
				ExtUart :: SendInt(Read16(4));
				ExtUart :: SendString(" GX ");
				ExtUart :: SendInt(Read16(6));
				ExtUart :: SendString(" GY ");
				ExtUart :: SendInt(Read16(8));
				ExtUart :: SendString(" GZ ");
				ExtUart :: SendInt(Read16(10));
				ExtUart :: SendString(" MX ");
				ExtUart :: SendInt(Read16(12));
				ExtUart :: SendString(" MY ");
				ExtUart :: SendInt(Read16(14));
				ExtUart :: SendString(" MZ ");
				ExtUart :: SendInt(Read16(16));
				ExtUart :: SendString(" La ");
				ExtUart :: SendFloat(ReadFloat(18));
				ExtUart :: SendString(" Lo ");
				ExtUart :: SendFloat(ReadFloat(22));
				ExtUart :: SendString(" Alt ");
				ExtUart :: SendFloat(ReadFloat(26));
				ExtUart :: SendString(" No ");
				ExtUart :: SendUInt((uint8_t)dataToBeSent[30]);
				ExtUart :: SendString(" D ");
				ExtUart :: SendUInt(ReadU16(31));
				ExtUart :: SendString(" APr ");
				ExtUart :: SendFloat(ReadFloat(33));
				ExtUart :: SendString(" DPr ");
				ExtUart :: SendInt(Read16(37));
				ExtUart :: SendString(" RM ");
				ExtUart :: SendUInt((uint8_t)dataToBeSent[39]);
				ExtUart :: SendString(" LM ");
				ExtUart :: SendUInt((uint8_t)dataToBeSent[40]);
				ExtUart :: SendString("\n");
				
				sentPackets++;
				writesWithinPage++;
				
				EepromSpi :: ChangeStateToWait();
				
				if (sentPackets >= lastPacketNr) {
					nAllSent = false;
				}
			}
		}
		
		static bool IsMemoryFull() {
			if ((currentPage == (pagesCount - 1)) && writesWithinPage == maxWritesPerPage ){
				return true;
			} else {
				return false;
			}
		}
		
		static uint16_t GetLastPacketNr() {
			return lastPacketNr;
		}
		
	private:
		static void CalculateStartingAddress() {
			if (writesWithinPage == maxWritesPerPage) {
				currentPage++;
				writesWithinPage = 0;
				currentAddress = (uint32_t)currentPage * bytesPerPage;
			} else if (firstPacket) {
				firstPacket = false;
			} else {
				currentAddress += bytesPerPacket;
			}
			
		}
		
		// For saving float value to EEPROM array
		static void WriteFloat(uint8_t index, float value) {
			uint8_t temp[sizeof(float)];
			memcpy(temp, &value, sizeof value);

			for(uint8_t i = 0; i < 4; i++) {
				dataToBeSaved[index + i] = temp[i];
			}
		}
		
		// Retrieve float value from EEPROM array
		static float ReadFloat(uint8_t index) {
			uint8_t temp[sizeof(float)];
			float ret;
			
			for(uint8_t i = 0; i < 4; i++) {
				temp[i] = dataToBeSent[index + i];
			}

			memcpy(&ret, temp, sizeof ret);
			return ret;
		}
		
		static void WriteU16(uint8_t index, uint16_t value) {
			dataToBeSaved[index] = (uint8_t) (value >> 8);
			dataToBeSaved[index + 1] = (uint8_t) value;
		}
		
		static void Write16(uint8_t index, int16_t value) {
			WriteU16(index, (uint16_t)value);
		}
		
		static uint16_t ReadU16(uint8_t index) {
			return ((uint16_t) (dataToBeSent[index] << 8)) | ((uint16_t) (dataToBeSent[index + 1]));;
		}
		
		static int16_t Read16(uint8_t index) {
			return (int16_t)ReadU16(index);
		}
		
};



#endif /* EEPROM_H_ */