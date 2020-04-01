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

#include <stdio.h>
#include <string.h>

class AbsoluteBaro {
	private:
		static float pressure;
		static uint8_t data[3];
	
	public: 
		static void CheckForUpdates() {
			
			System :: DisableInterruptsByPriority((System :: IntLevel)AbsSpi :: AbsSpiInterruptLevel);
			if (AbsSpi :: GetState() == AbsSpi::SpiState::DataUpdated) {
				memcpy(&data[0], (const void*)AbsSpi :: GetDataPointer(), sizeof(data));
				System :: EnableInterruptsByPriority((System :: IntLevel)AbsSpi :: AbsSpiInterruptLevel);

				uint32_t highByte = data[2];
				uint8_t lowByte = data[1];
				uint8_t ultraLowByte = data[0];
				
				uint16_t middle = lowByte << 8;
				uint32_t tempPressure = (highByte << 16) | middle | ultraLowByte;
				pressure = tempPressure / 4096.0f;
				
				AbsSpi :: ChangeStateToWait();
			} else {
				System :: EnableInterruptsByPriority((System :: IntLevel)AbsSpi :: AbsSpiInterruptLevel);
			}
			
		}
		
		static float GetPressure() {
			return pressure;
		}
};



#endif /* ABSBARO_H_ */