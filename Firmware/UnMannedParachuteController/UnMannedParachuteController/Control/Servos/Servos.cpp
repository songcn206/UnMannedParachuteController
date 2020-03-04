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
bool Servos :: autoControlMotors =  true;

void Servos :: SetLeftMotorPosition(int16_t degrees) {
	leftMotorPosition = CheckDegrees(degrees);
	PwmTimer :: UpdateCCReg('A', CalculateTimerCompareMatch((180 - leftMotorPosition ) + CalculateLeftMotorError(180 - leftMotorPosition)));
}

void Servos :: SetRightMotorPosition(int16_t degrees) {
	rightMotorPosition = CheckDegrees(degrees);
	PwmTimer :: UpdateCCReg('D', CalculateTimerCompareMatch(rightMotorPosition));
}

uint8_t Servos :: CheckDegrees(int16_t value) {
	if (value < 0) {
		return 0;
	} else if (value > 180) {
		return 180;
	}
	return value;
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
		int16_t value = PController(accY);
		SetRightMotorPosition(-PController(accY));
		SetLeftMotorPosition(PController(accY));
	}
}

void Servos :: SetAutoControlMotors(bool b) {
	autoControlMotors = b;
}

bool Servos :: GetAutoControlMotors() {
	return autoControlMotors;
}

int16_t Servos :: PController(int16_t accY) {
	return accY * autoControlPConstant;
}

int16_t Servos :: PIDController(int16_t accY) {
	
}