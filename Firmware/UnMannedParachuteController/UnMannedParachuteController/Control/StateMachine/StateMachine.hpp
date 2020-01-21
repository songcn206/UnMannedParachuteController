/*
 * StateMachine.h
 *
 * Created: 09.01.2020 14:10:46
 *  Author: timot
 */ 


#ifndef STATEMACHINE_HPP_
#define STATEMACHINE_HPP_

#include "HAL/UART/UART.hpp"
#include "Control/Imu/Imu.hpp"

class StateMachine {
	public:
		static void sendStatus() {
			ExtUart :: SendString("[IMU] AX ");
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
			ExtUart :: SendString("\n");
		}
};



#endif /* STATEMACHINE_H_ */