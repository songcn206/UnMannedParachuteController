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
	private:
		static constexpr uint8_t leftMotorZeroOffset = 32;
		static constexpr uint8_t leftMotorMaxOffset = 27;
		static constexpr float offsetConstant =  (float) (leftMotorZeroOffset - leftMotorMaxOffset) / 180.0;
		static constexpr uint8_t triangleSide_mm = 75;
		static constexpr uint16_t breakingDistance_mm = 2500;
		static constexpr uint16_t automaticTurnOffDistance = 600;
		static constexpr float breakingConstant = (float) 150 / ((int16_t)automaticTurnOffDistance - breakingDistance_mm);
		
		static uint8_t leftMotorPosition_mm;
		static uint8_t rightMotorPosition_mm;
		static uint8_t leftMotorPosition_degrees;
		static uint8_t rightMotorPosition_degrees;
		static bool autoControlMotors;
		
		// P variables
		static constexpr float autoControlPConstant = 0.02;
		
		// PID variables and constants
		static uint32_t previousTime;
		static int16_t previousAccY;
		static int32_t globalIntegral;
		static constexpr float PidPValue = 0.075f;
		static constexpr float PidIValue = 0.0005f; // Empirically tested 0.001
		static constexpr float PidDValue = 0.0001f; // Empirically tested 0.0001
		static constexpr float PidIDegrade = 0.999;  // Empirically tested
		
	public:	
		static void SetLeftMotorPosition(int16_t len); 
		static void SetRightMotorPosition(int16_t len);
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
		static uint8_t ClipLength(int16_t value);
		static int16_t LengthToAngle(int16_t len);
		static void InitPIDController();
};



#endif /* SERVOS_H_ */