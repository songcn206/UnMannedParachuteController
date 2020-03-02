/*
 * Servos.cpp
 *
 * Created: 25.02.2020 20:33:45
 *  Author: timot
 */ 

#include "Servos.hpp"

void Servos :: SetLeftMotorPosition(uint8_t degrees) {
	//PwmTimer :: UpdateCCReg('A', CalculateTimerCompareMatch((180 - degrees) + leftMotorOffset));
	PwmTimer :: UpdateCCReg('A', CalculateTimerCompareMatch2(degrees));
}

void Servos :: SetRightMotorPosition(uint8_t degrees) {
	PwmTimer :: UpdateCCReg('D', CalculateTimerCompareMatch(degrees));
}

uint16_t Servos :: CalculateTimerCompareMatch(uint8_t degrees) {
	return PwmTimer :: rightMotorMin + degrees * ((PwmTimer :: rightMotorMax - PwmTimer :: rightMotorMin)/(180 - 0));
}

uint16_t Servos :: CalculateTimerCompareMatch2(uint8_t degrees) {
	return PwmTimer :: leftMotorMin - degrees * ((PwmTimer :: leftMotorMin - PwmTimer :: leftMotorMax)/(180 - 0));
}