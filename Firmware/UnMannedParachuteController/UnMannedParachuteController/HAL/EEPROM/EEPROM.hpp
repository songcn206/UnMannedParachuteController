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

class InternalEeprom {
	public:
		enum class EepromAddress : uint16_t {
			LastServoPWM,		
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
	public:
	
	private:
		static constexpr uint16_t pagesCount = 1024;
		static constexpr uint8_t maxWritesPerPage = 6;
		static constexpr uint16_t bytesPerPage = 256;
		static constexpr uint8_t bytesPerPacket = 42;
		static uint32_t currentAddress;
		static uint8_t writesWithinPage;
		static uint16_t currentPage;
		static uint16_t dataPacketNr; 
		static uint8_t dataToBeSaved[bytesPerPacket];
		static uint32_t lastAddress;
		
	public:	
		static bool SaveData(int16_t accx, int16_t accy, int16_t accz, 
			int16_t gyrox, int16_t gyroy, int16_t gyroz, 
			int16_t magx, int16_t magy, int16_t magz, 
			float gpsLat, float gpsLon, float gpsAlt, uint8_t count,
			uint16_t distance, float pressure, int16_t diffpressure, 
			uint8_t right, uint8_t left, bool retry) {
			
			if (retry) {
				return EepromSpi :: WriteDataToMemory(currentAddress, &dataToBeSaved[0], bytesPerPacket);
			} else {
				CalculateStartingAddress();
				dataToBeSaved[0] = dataPacketNr;
				dataToBeSaved[1] = (uint8_t) accx >> 8;
				dataToBeSaved[2] = (uint8_t) accx;
				dataToBeSaved[3] = (uint8_t) accy >> 8;
				dataToBeSaved[4] = (uint8_t) accy;
				dataToBeSaved[5] = (uint8_t) accz >> 8;
				dataToBeSaved[6] = (uint8_t) accz;
				dataToBeSaved[7] = (uint8_t) gyrox >> 8;
				dataToBeSaved[8] = (uint8_t) gyrox;
				dataToBeSaved[9] = (uint8_t) gyroy >> 8;
				dataToBeSaved[10] = (uint8_t) gyroy;
				dataToBeSaved[11] = (uint8_t) gyroz >> 8;
				dataToBeSaved[12] = (uint8_t) gyroz;
				dataToBeSaved[13] = (uint8_t) magx >> 8;
				dataToBeSaved[14] = (uint8_t) magx;
				dataToBeSaved[15] = (uint8_t) magy >> 8;
				dataToBeSaved[16] = (uint8_t) magy;
				dataToBeSaved[17] = (uint8_t) magz >> 8;
				dataToBeSaved[18] = (uint8_t) magz;
				WriteFloat(19, gpsLat);
				WriteFloat(23, gpsLon);
				WriteFloat(27, gpsAlt);
				dataToBeSaved[31] = count;
				dataToBeSaved[32] = (uint8_t) distance >> 8;
				dataToBeSaved[33] = (uint8_t) distance;
				WriteFloat(34, pressure);
				dataToBeSaved[38] = (uint8_t) diffpressure >> 8;
				dataToBeSaved[39] = (uint8_t) diffpressure;
				dataToBeSaved[40] = right;
				dataToBeSaved[41] = left;
				
				dataPacketNr++;
				return EepromSpi :: WriteDataToMemory(currentAddress, &dataToBeSaved[0], bytesPerPacket);
			}
		}
		
		static bool SendData() {
			lastAddress = currentAddress;
			currentAddress = 0;
			bool nAllSent = true;
			
			while (true) {
				if (EepromSpi :: GetState() == EepromSpi::SpiState :: WriteInProgress) {
					EepromSpi :: CheckWriteProgress();
				}
			}
			
			while(nAllSent) {
				CalculateStartingAddress();
				EepromSpi :: ReadDataFromMemory()
				while() {}
				
				ExtUart :: SendString("[DATA] AX ");
				ExtUart :: SendInt(Imu :: GetAccXYZ()[0]);
				ExtUart :: SendString(" AY ");
				ExtUart :: SendInt(Imu :: GetAccXYZ()[1]);
				ExtUart :: SendString(" AZ ");
				ExtUart :: SendInt(Imu :: GetAccXYZ()[2]);
				ExtUart :: SendString(" GX ");
				ExtUart :: SendInt(Imu :: GetGyroXYZ()[0]);
				ExtUart :: SendString(" GY ");
				ExtUart :: SendInt(Imu :: GetGyroXYZ()[1]);
				ExtUart :: SendString(" GZ ");
				ExtUart :: SendInt(Imu :: GetGyroXYZ()[2]);
				ExtUart :: SendString(" MX ");
				ExtUart :: SendInt(Imu :: GetMagXYZ()[0]);
				ExtUart :: SendString(" MY ");
				ExtUart :: SendInt(Imu :: GetMagXYZ()[1]);
				ExtUart :: SendString(" MZ ");
				ExtUart :: SendInt(Imu :: GetMagXYZ()[2]);
				ExtUart :: SendString(" La ");
				ExtUart :: SendFloat(ParseGPSUart :: GetLatitude());
				ExtUart :: SendString(" Lo ");
				ExtUart :: SendFloat(ParseGPSUart :: GetLongitude());
				ExtUart :: SendString(" Alt ");
				ExtUart :: SendFloat(ParseGPSUart :: GetAltitude());
				ExtUart :: SendString(" No ");
				ExtUart :: SendUInt(ParseGPSUart :: GetGPSCount());
				ExtUart :: SendString(" D ");
				ExtUart :: SendUInt(Sonar :: GetDistance());
				ExtUart :: SendString(" APr ");
				ExtUart :: SendFloat(AbsoluteBaro :: GetPressure());
				ExtUart :: SendString(" DPr ");
				ExtUart :: SendInt(DiffBaro :: GetPressure());
				ExtUart :: SendString(" RM ");
				ExtUart :: SendUInt(Servos :: GetRightMotorPosition());
				ExtUart :: SendString(" LM ");
				ExtUart :: SendUInt(Servos :: GetLeftMotorPosition());
				ExtUart :: SendString("\n");
				if (currentAddress == lastAddress) {
					nAllSent = false;
				}
			}
			
		}
		
	private:
		static void CalculateStartingAddress() {
			if (writesWithinPage == maxWritesPerPage) {
				currentPage++;
				currentAddress = currentPage * bytesPerPage;
			} else if (currentAddress == 0) {
			
			} else {
				currentAddress += bytesPerPage;
			}
		}
		
		static void WriteFloat(uint8_t index, float value) {
			uint8_t temp[sizeof(float)];
			memcpy(temp, &value, sizeof value);

			for(uint8_t i = 0; i < 4; i++) {
				dataToBeSaved[index + i] = temp[i];
			}
		}
};



#endif /* EEPROM_H_ */