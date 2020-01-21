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
	
	public:
		static void Parse() {

			while (GetDataSafe(runningPointer) != 0) {
				uint8_t data = GetDataSafe(runningPointer);
				parseBuffer[parseBufferPos] = data;
				SetDataSafe(runningPointer, 0);
				if (data == terminatingChar) {
					
					if (MatchCommands(&parseBuffer[0], parseBufferPos, "$PUBX", sizeof("$PUBX"))) {
						HandlePUBXCommand(&parseBuffer[0], parseBufferPos);
						
					} /*else if (MatchCommands(&parseBuffer[0], parseBufferPos, "$GPGLA", sizeof("$GPGLA"))) {
							
					}*/
					
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
		
		/*
		static uint8_t ParseUint8(uint8_t* pointer) {
			const char number[2] = {pointer[0], pointer[1]};
			pointer += 3;
			return atoi((const char*)number);
		}
		
		static float ParseFloat(uint8_t* pointer) {
			const char number[4] = {pointer[0], pointer[1], pointer[2], pointer[3]};
			pointer += 5;
			return atoi((const char*)number);
		}*/
		
		static void HandlePUBXCommand(uint8_t* bufferpointer, uint8_t parseBufferLen) {
			uint8_t gpsDataStartArray[21] = {};
			//uint8_t gpsDataLengthArray[20] = {};
			uint8_t gpsDataPosition = 0;
			gpsDataStartArray[gpsDataPosition] = 0;
			gpsDataPosition++;
			
			//uint8_t length = 0;
			for(uint8_t i = 0; i < parseBufferLen; i++) {
				//length++;
				if (bufferpointer[i] == ',') {
					bufferpointer[i] = ' ';
					gpsDataStartArray[gpsDataPosition] = i + 1;
					//gpsDataLengthArray[gpsDataPosition] = length - 1;
					gpsDataPosition++;
					//length = 0;
				}
			}
			/*
			
			int glonassSatelites;
			sscanf((const char*)bufferpointer, "%d %d", &gpsSatelites, &glonassSatelites);
			ExtUart :: SendInt(gpsSatelites);
			ExtUart :: SendString("\n");*/
			
			uint8_t dateTimePos = gpsDataStartArray[2];
			uint8_t latitudePos = gpsDataStartArray[3];
			uint8_t longitudePos = gpsDataStartArray[5];
			uint8_t altitudePos = gpsDataStartArray[7];
			uint8_t gpsCountPos = gpsDataStartArray[18];
			
			ExtUart :: SendString("[GPS] T ");
			for (uint8_t i = 0; i < (gpsDataStartArray[3] - 1 - dateTimePos); i++) {
				ExtUart :: SendByte((uint8_t)bufferpointer[dateTimePos + i]);
			}
			ExtUart :: SendString(" La ");
			for (uint8_t i = 0; i <  (gpsDataStartArray[4] - 1 - latitudePos); i++) {
				ExtUart :: SendByte((uint8_t)bufferpointer[latitudePos + i]);
			}
			ExtUart :: SendString(" Lo ");
			for (uint8_t i = 0; i <  (gpsDataStartArray[6] - 1 - longitudePos); i++) {
				ExtUart :: SendByte((uint8_t)bufferpointer[longitudePos + i]);
			}	
			ExtUart :: SendString(" Alt ");
			for (uint8_t i = 0; i <  (gpsDataStartArray[8] - 1 - altitudePos); i++) {
				ExtUart :: SendByte((uint8_t)bufferpointer[altitudePos + i]);
			}
			ExtUart :: SendString(" No ");
			for (uint8_t i = 0; i <  (gpsDataStartArray[19] - 1 - gpsCountPos); i++) {
				ExtUart :: SendByte((uint8_t)bufferpointer[gpsCountPos + i]);
			}		
			ExtUart :: SendString("\n");
			
			float latitude = 0.0f;
			float longitude = 0.0f;
			sscanf((const char*)&bufferpointer[latitudePos], "%f", &latitude);
			sscanf((const char*)&bufferpointer[longitudePos], "%f", &longitude);
			asm volatile ("nop");
			/*ExtUart :: SendUInt(dateTime);
			
			if (dateTime == 194300) {
				ExtUart :: SendString("ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd\n");
			}*/
			
			led1 :: Toggle();
		}
};
#endif /* PARSEUART_H_ */