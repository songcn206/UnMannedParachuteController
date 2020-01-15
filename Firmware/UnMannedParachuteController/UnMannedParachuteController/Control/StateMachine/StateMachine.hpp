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
			ExtUart :: SendString("[STATUS] AccX: ");
			ExtUart :: SendInt(Imu :: GetAccXYZ()[0]);
			ExtUart :: SendString(" AccY: ");
			ExtUart :: SendInt(Imu :: GetAccXYZ()[1]);
			ExtUart :: SendString(" AccZ: ");
			ExtUart :: SendInt(Imu :: GetAccXYZ()[2]);
			ExtUart :: SendString(" GyroX: ");
			ExtUart :: SendInt(Imu :: GetGyroXYZ()[0]);
			ExtUart :: SendString(" GyroY: ");
			ExtUart :: SendInt(Imu :: GetGyroXYZ()[1]);
			ExtUart :: SendString(" GyroZ: ");
			ExtUart :: SendInt(Imu :: GetGyroXYZ()[2]);
			ExtUart :: SendString(" MagX: ");
			ExtUart :: SendInt(Imu :: GetMagXYZ()[0]);
			ExtUart :: SendString(" MagY: ");
			ExtUart :: SendInt(Imu :: GetMagXYZ()[1]);
			ExtUart :: SendString(" MagZ: ");
			ExtUart :: SendInt(Imu :: GetMagXYZ()[2]);
			ExtUart :: SendString("\n");
		}
};



#endif /* STATEMACHINE_H_ */