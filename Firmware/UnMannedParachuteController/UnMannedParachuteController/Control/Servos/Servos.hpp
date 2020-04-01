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
#include "HAL/Timers/GenericTimer/TickTimer.hpp"


class Servos {
	public:
	
	private:
		static constexpr uint8_t leftMotorZeroOffset = 39; 
		static constexpr uint8_t leftMotorMaxOffset = 35;
		static constexpr uint8_t triangleSide_mm = 75;
		
		static uint8_t leftMotorPosition;
		static uint8_t rightMotorPosition;
		static bool autoControlMotors;
		
		// P variables
		static constexpr float autoControlPConstant = 0.1;
		
		// PID variables and constants
		static uint32_t previousTime;
		static int16_t previousAccY;
		static int32_t globalIntegral;
		static constexpr float PidPValue = 0.2f;
		static constexpr float PidIValue = 0.00075f;
		static constexpr float PidDValue = 0.000125f;
		static constexpr float PidIDegrade = 0.97;
		
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
		static int16_t LengthToAngle(int16_t len);
};



#endif /* SERVOS_H_ */