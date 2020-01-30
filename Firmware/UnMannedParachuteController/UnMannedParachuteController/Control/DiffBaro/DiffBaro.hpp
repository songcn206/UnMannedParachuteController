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

class DiffBaro {
	private:
		static int16_t pressure;
	
	public:
		static void CheckForUpdates() {
			System :: DisableInterruptsByPriority((System :: IntLevel)I2cDiffBaro :: I2cDiffBaroInterruptLevel);
			if (I2cDiffBaro :: GetMeasureReady()) {
				volatile uint8_t* pointer = I2cDiffBaro :: GetPointer();
				pressure = (pointer[1] << 8) | pointer[0];
				
				I2cDiffBaro :: SetMeasureReady(false);
			}
			System :: EnableInterruptsByPriority((System :: IntLevel)I2cDiffBaro :: I2cDiffBaroInterruptLevel);
		}
	
	
		static int16_t GetPressure() {
			return pressure;
		}
	
};



#endif /* DIFFBARO_H_ */