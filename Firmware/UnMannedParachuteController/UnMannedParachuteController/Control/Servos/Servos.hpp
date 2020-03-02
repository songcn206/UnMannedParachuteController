/*
 * Servos.h
 *
 * Created: 25.02.2020 20:33:55
 *  Author: timot
 */ 


#ifndef SERVOS_HPP_
#define SERVOS_HPP_

#include "HAL/System/System.hpp"
#include "HAL/Timers/PWMTimer/PWMTimer.hpp"


class Servos {
	public:
	
	private:
		static constexpr uint8_t leftMotorOffset = 26; // Empirically tested
	
	public:
		static void SetLeftMotorPosition(uint8_t degrees); 
		static void SetRightMotorPosition(uint8_t degrees); 
	
	private:
		static uint16_t CalculateTimerCompareMatch(uint8_t degrees);
		static uint16_t CalculateTimerCompareMatch2(uint8_t degrees);
};



#endif /* SERVOS_H_ */