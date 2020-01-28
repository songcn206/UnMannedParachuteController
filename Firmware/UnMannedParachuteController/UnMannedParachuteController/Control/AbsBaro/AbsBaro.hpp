/*
 * AbsBaro.h
 *
 * Created: 28.01.2020 20:09:07
 *  Author: timot
 */ 


#ifndef ABSBARO_HPP_
#define ABSBARO_HPP_

#include "HAL/System/System.hpp"
#include "HAL/UART/UART.hpp"
#include "HAL/SPI/AbsSpi.hpp"

class AbsoluteBaro {
	private:
		static float pressure;
	
	public: 
		static void CheckForUpdates() {
			System :: DisableInterruptsByPriority((System :: IntLevel)AbsSpi :: AbsSpiInterruptLevel);
			if (AbsSpi :: GetState() == AbsSpi::SpiState::DataUpdated) {
				volatile uint8_t* pointer = AbsSpi :: GetDataPointer();
				uint32_t highByte = pointer[2];
				uint8_t lowByte = pointer[1];
				uint8_t ultraLowByte = pointer[0];
				
				uint16_t middle = lowByte << 8;
				uint32_t tempPressure = (highByte << 16) | middle | ultraLowByte;
				pressure = tempPressure / 4096.0f;
				
				//ExtUart :: SendFloat(pressure);
				//ExtUart :: SendString("\n");
				
				AbsSpi :: ChangeStateToWait();
			}
			System :: EnableInterruptsByPriority((System :: IntLevel)AbsSpi :: AbsSpiInterruptLevel);
		}
		
		
		static float GetPressure() {
			return pressure;
		}
};



#endif /* ABSBARO_H_ */