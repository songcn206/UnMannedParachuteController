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
		static constexpr uint8_t leftMotorZeroOffset = 39; 
		static constexpr uint8_t leftMotorMaxOffset = 35;
		static constexpr float autoControlPConstant = 0.2;
		
		static uint8_t leftMotorPosition;
		static uint8_t rightMotorPosition;
		static bool autoControlMotors;
	
	public:
		static void SetLeftMotorPosition(int16_t degrees); 
		static void SetRightMotorPosition(int16_t degrees);
		static uint8_t GetRightMotorPosition();
		static uint8_t GetLeftMotorPosition();
		
		static void AutoControlMotors();
		static void SetAutoControlMotors(bool b);
		static bool GetAutoControlMotors();
	
	private:
		static uint16_t CalculateTimerCompareMatch(uint8_t degrees);
		static float CalculateLeftMotorError(uint8_t degrees);
		static int16_t PController(int16_t accY);
		static int16_t PIDController(int16_t accY);

		static uint8_t CheckDegrees(int16_t value);


};



#endif /* SERVOS_H_ */