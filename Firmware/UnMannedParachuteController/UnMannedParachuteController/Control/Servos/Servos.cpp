/*
 * Servos.cpp
 *
 * Created: 25.02.2020 20:33:45
 *  Author: timot
 */ 

#include "Servos.hpp"

void Servos :: SetLeftMotorPosition(uint8_t degrees) {
	PwmTimer :: UpdateCCReg('A', CalculateTimerCompareMatch(degrees));
}

void Servos :: SetRightMotorPosition(uint8_t degrees) {
	PwmTimer :: UpdateCCReg('D', CalculateTimerCompareMatch(degrees));
}

uint16_t Servos :: CalculateTimerCompareMatch(uint8_t degrees) {
	return PwmTimer :: motorMin + degrees * ((PwmTimer :: motorMax - PwmTimer :: motorMin)/(180 - 0));
}