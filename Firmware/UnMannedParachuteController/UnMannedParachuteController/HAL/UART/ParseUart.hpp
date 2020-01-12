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

template <typename conf>
class ParseUart {
	public:
	
	private:
		static volatile uint8_t* arrayStartPointer;
		static volatile uint8_t* arrayEndPointer;
		static constexpr char terminatingChar = conf :: uart :: terminatingChar;
		
		static constexpr uint8_t parseBufferSize = conf :: parseBufferSize;	
		static uint8_t parseBuffer[parseBufferSize];
		static uint8_t parseBufferPos;
		static volatile uint8_t* runningPointer;

	public:
		static void Parse() {
			while (GetDataSafe(runningPointer) != 0) {
				led2 :: Toggle();
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
			System :: DisableInterruptsByPriority(conf :: uart :: GetInterruptLevel());
			uint8_t tempData = *pointer;
			System :: EnableInterruptsByPriority(conf :: uart :: GetInterruptLevel());
			return tempData;
		}

		static void SetDataSafe(volatile uint8_t* pointer, uint8_t data) {
			System :: DisableInterruptsByPriority(conf :: uart :: GetInterruptLevel());
			*pointer = data;
			System :: EnableInterruptsByPriority(conf :: uart :: GetInterruptLevel());
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

template<typename conf>
volatile uint8_t* ParseUart<conf> :: arrayStartPointer = conf :: uart :: GetRxBufferStart();

template<typename conf>
volatile uint8_t* ParseUart<conf> :: arrayEndPointer = conf :: uart :: GetRxBufferEnd();

template<typename conf>
volatile uint8_t* ParseUart<conf> :: runningPointer = arrayStartPointer;

template<typename conf>
uint8_t ParseUart<conf> :: parseBufferPos = 0;

template<typename conf>
uint8_t ParseUart<conf> :: parseBuffer[parseBufferSize] = {};

struct ExtUartParseConf {
	typedef ExtUart uart;
	static constexpr uint8_t parseBufferSize = 150;	
};
typedef ParseUart<ExtUartParseConf> ExtUartParse;




#endif /* PARSEUART_H_ */