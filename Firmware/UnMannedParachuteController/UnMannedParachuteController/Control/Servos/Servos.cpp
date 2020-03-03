/*
 * Servos.cpp
 *
 * Created: 25.02.2020 20:33:45
 *  Author: timot
 */ 

#include "Servos.hpp"
#include "Control/Imu/Imu.hpp"

uint8_t Servos :: leftMotorPosition = 255;
uint8_t Servos :: rightMotorPosition = 255;

void Servos :: SetLeftMotorPosition(uint8_t degrees) {
	leftMotorPosition = degrees;
	PwmTimer :: UpdateCCReg('A', CalculateTimerCompareMatch((180 - leftMotorPosition ) + CalculateLeftMotorError(180 - leftMotorPosition)));
}

void Servos :: SetRightMotorPosition(uint8_t degrees) {
	rightMotorPosition = degrees;
	PwmTimer :: UpdateCCReg('D', CalculateTimerCompareMatch(degrees));
}

uint16_t Servos :: CalculateTimerCompareMatch(uint8_t degrees) {
	return PwmTimer :: rightMotorMin + degrees * ((PwmTimer :: rightMotorMax - PwmTimer :: rightMotorMin)/(180 - 0));
}

float Servos :: CalculateLeftMotorError(uint8_t degrees) {
	return 0.0222f * degrees + 35; // Maybe add long format of linear interpolation. COnstants in hpp
}

uint8_t Servos :: GetLeftMotorPosition() {
	return leftMotorPosition;
}

uint8_t Servos :: GetRightMotorPosition() {
	return rightMotorPosition;
}

void Servos :: AutoControlMotors() {
	if (Imu :: GetAccXYZ()[0] > 100) {
		SetLeftMotorPosition(0);
		SetRightMotorPosition(0);
	} else {
		int16_t accY = Imu :: GetAccXYZ()[1];
		//ExtUart :: SendString("\n");
		//ExtUart :: SendInt(accY);
		//ExtUart :: SendString("\n");
		if (accY > 0) {
			SetRightMotorPosition(0);
			SetLeftMotorPosition((uint8_t)GetPControllerDegrees(accY, false));
		} else if (accY < 0) {
			SetRightMotorPosition((uint8_t)GetPControllerDegrees(accY, true));
			SetLeftMotorPosition(0);
		}
	}
}

uint8_t Servos :: GetPControllerDegrees(int16_t accY, bool inv) {
	int16_t value;
	if (inv) {
		accY = accY * (-1);
	}
	value = accY / autoControlPConstant;
	if (value < 0) {
		value = 0;
	} else if (value > 180) {
		value = 180;
	}
	return value;
}