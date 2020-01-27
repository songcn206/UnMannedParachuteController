/*
 * ParseUart.h
 *
 * Created: 09.01.2020 14:11:48
 *  Author: timot
 */ 


#ifndef PARSEUART_HPP_
#define PARSEUART_HPP_

#include "HAL/UART/UART.hpp"
#include "HAL/System/Pins.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

class ParseExtUart {
	private:
		static volatile uint8_t* arrayStartPointer;
		static volatile uint8_t* arrayEndPointer;
		static volatile uint8_t* runningPointer;

		static constexpr char terminatingChar = ExtUart :: terminatingChar;
		static constexpr uint8_t parseBufferSize = 200;	
		
		static uint8_t parseBuffer[parseBufferSize];
		static uint8_t parseBufferPos;
		
	public:
		static void Parse() {
			while (GetDataSafe(runningPointer) != 0) {
				//led2 :: Toggle();
				uint8_t data = GetDataSafe(runningPointer);
				parseBuffer[parseBufferPos] = data;
				SetDataSafe(runningPointer, 0);
				
				if (data == terminatingChar) {
					if (MatchCommands(&parseBuffer[0], parseBufferPos, "START", sizeof("START"))) {
						led1 :: Toggle();
					} else if (MatchCommands(&parseBuffer[0], parseBufferPos, "CANCEL", sizeof("CANCEL"))) {
						led3 :: Toggle();
					}
					parseBufferPos = parseBufferSize - 1; // Hack
				}
				
				if (runningPointer == arrayEndPointer) {
					runningPointer = arrayStartPointer;
				} else {
					runningPointer++;
				}
				
				if (parseBufferPos == parseBufferSize - 1) {
					parseBufferPos = 0;
				} else {
					parseBufferPos++;
				}
			}
		}

		static uint8_t GetDataSafe(volatile uint8_t* pointer) {
			System :: DisableInterruptsByPriority(ExtUart :: GetInterruptLevel());
			uint8_t tempData = *pointer;
			System :: EnableInterruptsByPriority(ExtUart :: GetInterruptLevel());
			return tempData;
		}

		static void SetDataSafe(volatile uint8_t* pointer, uint8_t data) {
			System :: DisableInterruptsByPriority(ExtUart :: GetInterruptLevel());
			*pointer = data;
			System :: EnableInterruptsByPriority(ExtUart :: GetInterruptLevel());
		}

		static bool MatchCommands(uint8_t* pointer, uint8_t bufferlength, const char* command, uint8_t commandLength) {
			if (bufferlength < commandLength - 1) {
				return false;
			}
			for (uint8_t i = 0; i < bufferlength; i++) {
				if (*pointer != *command) {
					return false;
				}
				pointer++;
				command++;
			}
			return true;
		}
};

class ParseGPSUart {
	private:
		static volatile uint8_t* arrayStartPointer;
		static volatile uint8_t* arrayEndPointer;
		static volatile uint8_t* runningPointer;

		static constexpr char terminatingChar = GpsUart :: terminatingChar;
		static constexpr uint8_t parseBufferSize = 200;
		
		static uint8_t parseBuffer[parseBufferSize];
		static uint8_t parseBufferPos;
		
		//float volatile dateTime;
		static float volatile latitude;
		static float volatile longitude;
		static float volatile altitude;
		static uint8_t volatile gpsCount;
	
	public:
		static void Parse() {
			while (GetDataSafe(runningPointer) != 0) {
				uint8_t data = GetDataSafe(runningPointer);
				parseBuffer[parseBufferPos] = data;
				SetDataSafe(runningPointer, 0);
				if (data == terminatingChar) {
					
					if (MatchCommands(&parseBuffer[0], parseBufferPos, "$PUBX", sizeof("$PUBX"))) {
						HandlePUBXCommand(&parseBuffer[0], parseBufferPos);
						
					} 
					
					parseBufferPos = parseBufferSize - 1; // Hack
				}
				
				if (runningPointer == arrayEndPointer) {
					runningPointer = arrayStartPointer;
				} else {
					runningPointer++;
				}
				
				if (parseBufferPos == parseBufferSize - 1) {
					parseBufferPos = 0;
				} else {
					parseBufferPos++;
				}
			}
		}
		
		static uint8_t GetDataSafe(volatile uint8_t* pointer) {
			System :: DisableInterruptsByPriority(GpsUart :: GetInterruptLevel());
			uint8_t tempData = *pointer;
			System :: EnableInterruptsByPriority(GpsUart :: GetInterruptLevel());
			return tempData;
		}

		static void SetDataSafe(volatile uint8_t* pointer, uint8_t data) {
			System :: DisableInterruptsByPriority(GpsUart :: GetInterruptLevel());
			*pointer = data;
			System :: EnableInterruptsByPriority(GpsUart :: GetInterruptLevel());
		}
		
		static bool MatchCommands(uint8_t* pointer, uint8_t bufferlength, const char* command, uint8_t commandLength) {
			if (bufferlength < commandLength - 1) {
				return false;
			}
			for (uint8_t i = 0; i < commandLength - 1; i++) {
				if (*pointer != *command) {
					return false;
				}
				pointer++;
				command++;
			}
			return true;
		}
		
		static void HandlePUBXCommand(uint8_t* bufferpointer, uint8_t parseBufferLen) {
			uint8_t gpsDataStartArray[21] = {};
			uint8_t gpsDataPosition = 0;
			gpsDataStartArray[gpsDataPosition] = 0;
			gpsDataPosition++;
			
			for(uint8_t i = 0; i < parseBufferLen; i++) {
				if (bufferpointer[i] == ',') {
					gpsDataStartArray[gpsDataPosition] = i + 1;
					gpsDataPosition++;
				}
			}

			//uint8_t dateTimePos = gpsDataStartArray[2];
			uint8_t latitudePos = gpsDataStartArray[3];
			uint8_t longitudePos = gpsDataStartArray[5];
			uint8_t altitudePos = gpsDataStartArray[7];
			uint8_t gpsCountPos = gpsDataStartArray[18];
			
			//char tempDateTime[9];
			char tempLatitude[10];
			char tempLongitude[11];
			char tempAltitude[7];
			char tempGpsCount[2];
			
			uint8_t altitudePointPos = 0;
			
			ExtUart :: SendString("[GPS]");
			/*ExtUart :: SendString(" T ");
			for (uint8_t i = 0; i < (gpsDataStartArray[3] - 1 - dateTimePos); i++) {
				ExtUart :: SendByte((uint8_t)bufferpointer[dateTimePos + i]);
				tempDateTime[i] = bufferpointer[dateTimePos + i];	
			}*/
			
			ExtUart :: SendString(" La ");
			for (uint8_t i = 0; i <  (gpsDataStartArray[4] - 1 - latitudePos); i++) {
				ExtUart :: SendByte((uint8_t)bufferpointer[latitudePos + i]);
				tempLatitude[i] = bufferpointer[latitudePos + i];
			}

			ExtUart :: SendString(" Lo ");
			for (uint8_t i = 0; i <  (gpsDataStartArray[6] - 1 - longitudePos); i++) {
				ExtUart :: SendByte((uint8_t)bufferpointer[longitudePos + i]);
				tempLongitude[i] = bufferpointer[longitudePos + i];
			}	
			ExtUart :: SendString(" Alt ");
			for (uint8_t i = 0; i <  (gpsDataStartArray[8] - 1 - altitudePos); i++) {
				ExtUart :: SendByte((uint8_t)bufferpointer[altitudePos + i]);
				tempAltitude[i] = bufferpointer[altitudePos + i];
				if (bufferpointer[altitudePos + i] == '.') {
					altitudePointPos = i;
				}
			}
			
			ExtUart :: SendString(" No ");
			for (uint8_t i = 0; i <  (gpsDataStartArray[19] - 1 - gpsCountPos); i++) {
				ExtUart :: SendByte((uint8_t)bufferpointer[gpsCountPos + i]);
				tempGpsCount[i] = bufferpointer[gpsCountPos + i];
			}		
			ExtUart :: SendString("\n");

			//dateTime = StringToFloat(tempDateTime, sizeof(tempDateTime), 6);
			latitude = StringToFloat(tempLatitude, gpsDataStartArray[4] - 1 - latitudePos, 4);
			longitude = StringToFloat(tempLongitude, gpsDataStartArray[6] - 1 - longitudePos, 5);
			altitude = StringToFloat(tempAltitude, gpsDataStartArray[8] - 1 - altitudePos, altitudePointPos);
			gpsCount = StringToUint8(tempGpsCount, gpsDataStartArray[19] - 1 - gpsCountPos);
			
			
			//latitude2 = StringToDouble(tempLatitude, sizeof(tempLatitude), 6);
			
			
			/*
			latitude = atof(tempLatitude);
			longitude = atof(tempLongitude);
			altitude = atof(tempAltitude);
			*/
			led1 :: Toggle();
			
		}
		
		static uint8_t StringToUint8(const char* pointer, uint8_t len) {
			if (len == 3) {
				return 100 * (pointer[0] - 0b00110000)  + 10 * (pointer[1] - 0b00110000) + (pointer[2] - 0b00110000);
			} else if (len == 2) {
				return 10 * (pointer[0] - 0b00110000) + (pointer[1] - 0b00110000);
			} else {
				return pointer[0] - 0b00110000;
			}
		}
		
		static float StringToFloat(const char* pointer, uint8_t len, uint8_t pointPos) {
			float result = 0.0f;
			
			uint8_t tempValue;
			float powerOf = (float)findPowerOf(10, pointPos - 1);
			for (uint8_t i = 0; i < len; i++) {
				if (i != pointPos) {
					tempValue = pointer[i] - 0b00110000;
					result += tempValue * powerOf;
					powerOf = powerOf / 10.0f;
				}
			}
			return result;
		}
		 
		static uint32_t findPowerOf(uint8_t base, uint8_t exponent) {
			uint32_t result = 1;
			for (uint8_t i = 0; i < exponent; i++) {
				result = result * base;
			}
			return result;		
		}
		
		static double GetLatitude() {
			return latitude;
		}
		
		static float GetLongitude() {
			return longitude;
		}
		
		static float GetAltitude() {
			return altitude;
		}
		
		static uint8_t GetGPSCount() {
			return gpsCount;
		}
};
#endif /* PARSEUART_H_ */