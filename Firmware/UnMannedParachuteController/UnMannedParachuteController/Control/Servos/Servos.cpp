/*
 * Servos.cpp
 *
 * Created: 25.02.2020 20:33:45
 *  Author: timot
 */ 

#include "Servos.hpp"
#include "Control/Imu/Imu.hpp"
#include "HAL/ADC/ADC.hpp"

uint8_t Servos :: leftMotorPosition_mm = 0;
uint8_t Servos :: rightMotorPosition_mm = 0;
uint8_t Servos :: leftMotorPosition_degrees;
uint8_t Servos :: rightMotorPosition_degrees;
bool Servos :: autoControlMotors =  false;

uint32_t Servos :: previousTime = 0;
int16_t Servos :: previousAccY = 0;
int32_t Servos :: globalIntegral = 0;

void Servos :: SetLeftMotorPosition(int16_t len) {
	leftMotorPosition_mm = ClipLength(len);
	leftMotorPosition_degrees = LengthToAngle(leftMotorPosition_mm);
	PwmTimer :: UpdateCCReg('A', CalculateTimerCompareMatch((180 - leftMotorPosition_degrees) + CalculateLeftMotorError(180 - leftMotorPosition_degrees)));
}

void Servos :: SetRightMotorPosition(int16_t len) {
	rightMotorPosition_mm = ClipLength(len);
	rightMotorPosition_degrees = LengthToAngle(rightMotorPosition_mm);
	PwmTimer :: UpdateCCReg('B', CalculateTimerCompareMatch(rightMotorPosition_degrees));
}

uint8_t Servos :: ClipLength(int16_t value) {
	if (value < 0) {
		return 0;
	} else if (value > 150) {
		return 150;
	}
	return value;
}

uint16_t Servos :: CalculateTimerCompareMatch(uint8_t degrees) {
	return PwmTimer :: rightMotorMin + degrees * ((PwmTimer :: rightMotorMax - PwmTimer :: rightMotorMin)/(180 - 0));
}

float Servos :: CalculateLeftMotorError(uint8_t degrees) {
	return leftMotorMaxOffset + degrees * offsetConstant;
}

uint8_t Servos :: GetLeftMotorPosition() {
	return leftMotorPosition_mm;
}

uint8_t Servos :: GetRightMotorPosition() {
	return rightMotorPosition_mm;
}

void Servos :: InitPIDController() {
	previousTime = 0;
	previousAccY = 0;
	globalIntegral = 0;
}

void Servos :: AutoControlMotors() {
	/*if (Imu :: GetAccXYZ()[0] > 150) {
		SetLeftMotorPosition(0);
		SetRightMotorPosition(0);
	} else if (Sonar :: GetDistance() < automaticTurnOffDistance) {
		autoControlMotors = false;
	} */if (Sonar :: GetDistance() < breakingDistance_mm) {
		uint16_t motorBreaking = (Sonar :: GetDistance() - automaticTurnOffDistance) * (-0.078947) + 150;
		
		if (leftMotorPosition_mm < motorBreaking) {
			SetLeftMotorPosition(motorBreaking);
		} 
		
		if (rightMotorPosition_mm < motorBreaking) {
			SetRightMotorPosition(motorBreaking);
		}
		
	} else {
		int16_t accY = Imu :: GetAccXYZ()[1];
		//int16_t controllerValue = PController(accY);
		int16_t controllerValue = PIDController(accY);
		SetRightMotorPosition(-controllerValue);
		SetLeftMotorPosition(controllerValue);
	}
}

void Servos :: SetAutoControlMotors(bool b) {
	autoControlMotors = b;
	InitPIDController();
}

bool Servos :: GetAutoControlMotors() {
	return autoControlMotors;
}

int16_t Servos :: PController(int16_t accY) {
	return accY * autoControlPConstant;
}

int16_t Servos :: PIDController(int16_t accY) {
	uint32_t currentTime;
	uint16_t timeDifference;
	int16_t accYDifference;
	static float derivative;
	int16_t returnValue;
	
	// Calculating integral
	currentTime = TickTimer :: GetTicks();
	timeDifference = (currentTime - previousTime) / 10;
	previousTime = currentTime;
	int16_t integral = timeDifference * accY;
	globalIntegral += integral;
	
	// Calculating derivative
	accYDifference = accY - previousAccY;
	previousAccY = accY;
	derivative = accYDifference / timeDifference;
	
	returnValue = PidPValue * accY + PidIValue * globalIntegral + PidDValue * derivative;
	globalIntegral *= PidIDegrade;
	
	return returnValue;
}

int16_t Servos :: LengthToAngle(int16_t len) {
	if (len <= 0) {
		return 0;
	} else if (len >= (2 * Servos :: triangleSide_mm)) {
		return 180;
	}

	return 57.29577f * acos((2 * pow(triangleSide_mm, 2) - pow(len, 2)) / (2 * pow(triangleSide_mm, 2)));
}