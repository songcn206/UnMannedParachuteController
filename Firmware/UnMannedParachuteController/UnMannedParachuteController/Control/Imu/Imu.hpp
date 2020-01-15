/*
 * Imu.h
 *
 * Created: 11.01.2020 20:13:39
 *  Author: timot
 */ 


#ifndef IMU_HPP_
#define IMU_HPP_
#include "HAL/SPI/ImuSPI.hpp"


class Imu {
	public:
		static int16_t acc[3];
		static int16_t gyro[3];
		static int16_t mag[3];
		
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
			//ExtUart :: SendString("CheckUpdates\n");
			System :: DisableInterruptsByPriority((System :: IntLevel)ImuSpi :: ImuSpiInterruptLevel);
			if (ImuSpi :: GetState() == ImuSpi::SpiState::DataUpdated) {
				volatile uint8_t* pointer = ImuSpi :: GetData();
				uint8_t highByte;
				uint8_t lowByte;
				for (uint8_t i = 0; i < 3; i++) {
					highByte = *pointer;
					lowByte = *(pointer+1);
					acc[i] = (highByte << 8) | lowByte;
					pointer += 2;
				}
				for (uint8_t i = 0; i < 3; i++) {
					highByte = *pointer;
					lowByte = *(pointer+1);
					gyro[i] = (highByte << 8) | lowByte;
					pointer += 2;
				}
				for (uint8_t i = 0; i < 3; i++) {
					lowByte = *pointer;
					highByte = *(pointer+1);
					mag[i] = (highByte << 8) | lowByte;
					pointer += 2;
				}
				ImuSpi :: DataTransferCompleted();
			}
			System :: EnableInterruptsByPriority((System :: IntLevel)ImuSpi :: ImuSpiInterruptLevel);
		}
};



#endif /* IMU_H_ */