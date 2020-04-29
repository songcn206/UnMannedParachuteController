/*
 * Imu.h
 *
 * Created: 11.01.2020 20:13:39
 *  Author: timot
 */ 


#ifndef IMU_HPP_
#define IMU_HPP_

#include "HAL/SPI/ImuSPI.hpp"
#include "HAL/Filters/ExpMovingFilter.hpp"

#include <stdio.h>
#include <string.h>

class Imu {
	private:
		static constexpr uint8_t allowedFalsePackets = 5;
		static uint8_t discardedPackets;
		
		static int16_t acc[3];
		static int16_t gyro[3];
		static int16_t mag[3];
		
		static uint8_t rawData[18];
		static uint8_t dataPointer;
		
		typedef ExpMovingFilter<AccConf, int16_t> AccFilter;
		typedef ExpMovingFilter<GyroConf, int16_t> GyroFilter;
		typedef ExpMovingFilter<MagConf, int16_t> MagFilter;
		
		static AccFilter AccXFilter;
		static AccFilter AccYFilter;
		static AccFilter AccZFilter;
		
		static GyroFilter GyroXFilter;
		static GyroFilter GyroYFilter;
		static GyroFilter GyroZFilter;
		
		static MagFilter MagXFilter;
		static MagFilter MagYFilter;
		static MagFilter MagZFilter;
		
	public:
		static int16_t* GetAccXYZ() {
			return &acc[0];
		}
		
		static int16_t* GetGyroXYZ() {
			return &gyro[0];
		}
		
		static int16_t* GetMagXYZ() {
			return &mag[0];
		}
		
		static void CheckForUpdates() {
			System :: DisableInterruptsByPriority((System :: IntLevel)ImuSpi :: ImuSpiInterruptLevel);
			if (ImuSpi :: GetState() == ImuSpi::SpiState::DataUpdated) {
				memcpy(&rawData[0], (const void*)ImuSpi :: GetData(), sizeof(rawData));
				ImuSpi :: DataTransferCompleted();
				System :: EnableInterruptsByPriority((System :: IntLevel)ImuSpi :: ImuSpiInterruptLevel);
								
				acc[0] = AccXFilter.UpdateAndReturn((rawData[dataPointer] << 8) | rawData[dataPointer + 1]);
				dataPointer += 2;
				acc[1] = AccYFilter.UpdateAndReturn((rawData[dataPointer] << 8) | rawData[dataPointer + 1]);
				dataPointer += 2;
				acc[2] = AccZFilter.UpdateAndReturn((rawData[dataPointer] << 8) | rawData[dataPointer + 1]);
				dataPointer += 2;
				
				gyro[0] = GyroXFilter.UpdateAndReturn((rawData[dataPointer] << 8) | rawData[dataPointer + 1]);
				dataPointer += 2;
				gyro[1] = GyroYFilter.UpdateAndReturn((rawData[dataPointer] << 8) | rawData[dataPointer + 1]);
				dataPointer += 2;
				gyro[2] = GyroZFilter.UpdateAndReturn((rawData[dataPointer] << 8) | rawData[dataPointer + 1]);
				dataPointer += 2;
				
				mag[0] = MagXFilter.UpdateAndReturn((rawData[dataPointer] << 8) | rawData[dataPointer + 1]);
				dataPointer += 2;
				mag[1] = MagYFilter.UpdateAndReturn((rawData[dataPointer] << 8) | rawData[dataPointer + 1]);
				dataPointer += 2;
				mag[2] = MagZFilter.UpdateAndReturn((rawData[dataPointer] << 8) | rawData[dataPointer + 1]);
				dataPointer += 2;
	
				dataPointer = 0;
				
				if (mag[0] == 0 && mag[1] == 0 && mag[2] == 0) {
					if (allowedFalsePackets <= discardedPackets) {
						ImuSpi :: InitConnection();
						discardedPackets = 0;
					} else {
						discardedPackets++;
					}
				}
			} else {
				System :: EnableInterruptsByPriority((System :: IntLevel)ImuSpi :: ImuSpiInterruptLevel);
			}
		}
};



#endif /* IMU_H_ */