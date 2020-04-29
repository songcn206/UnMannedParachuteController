/*
 * DiffBaro.h
 *
 * Created: 30.01.2020 18:13:13
 *  Author: timot
 */ 


#ifndef DIFFBARO_H_
#define DIFFBARO_H_

#include "HAL/System/System.hpp"
#include "HAL/I2C/I2C.hpp"

#include <stdio.h>
#include <string.h>

class DiffBaro {
	private:
		static int16_t pressure;
		static uint8_t data[2];
	
	public:
		static void CheckForUpdates() {
			
			System :: DisableInterruptsByPriority((System :: IntLevel)I2cDiffBaro :: I2cDiffBaroInterruptLevel);
			if (I2cDiffBaro :: GetMeasureReady()) {
				memcpy(&data[0], (const void*)I2cDiffBaro :: GetPointer(), sizeof(data));
				I2cDiffBaro :: SetMeasureReady(false);
				System :: EnableInterruptsByPriority((System :: IntLevel)I2cDiffBaro :: I2cDiffBaroInterruptLevel);	
				
				pressure = (data[1] << 8) | data[0];

			} else {
				System :: EnableInterruptsByPriority((System :: IntLevel)I2cDiffBaro :: I2cDiffBaroInterruptLevel);	
			}
		}
	
		static int16_t GetPressure() {
			return pressure;
		}
};



#endif /* DIFFBARO_H_ */