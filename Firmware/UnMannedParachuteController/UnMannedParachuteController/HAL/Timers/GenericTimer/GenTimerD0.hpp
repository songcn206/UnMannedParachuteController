/*
 * TimerD0.h
 *
 * Created: 09.01.2020 19:11:02
 *  Author: timot
 */ 


#ifndef GENTIMERD0_HPP_
#define GENTIMERD0_HPP_
#include "HAL/System/System.hpp"
#include "HAL/Timers/Timers.hpp"
#include "HAL/System/Pins.hpp"
#include "HAL/UART/UART.hpp"


class GenTimerD0 {
	public:
		static constexpr uint16_t preScaler = 1024;
		
		static constexpr float compareMatchAValue_ms = 1.0f; // Check for updates (uart, imu, etc)
		static constexpr float compareMatchBValue_ms = 5.0f; // Imu 
		static constexpr float compareMatchCValue_ms = 200.0f; // Send data
		static constexpr float compareMatchCValueFaster_ms = 20.0; // Save data
		static constexpr float compareMatchDValue_ms = 50.0f; // Sonar
		
		static volatile constexpr uint16_t compareMatchAValue = System :: CPU_CLOCK / preScaler * (compareMatchAValue_ms / 1000);
		static volatile constexpr uint16_t compareMatchBValue = System :: CPU_CLOCK / preScaler * (compareMatchBValue_ms / 1000);
		static volatile uint16_t compareMatchCValue;
		static volatile constexpr uint16_t compareMatchDValue = System :: CPU_CLOCK / preScaler * (compareMatchDValue_ms / 1000);
		
		static volatile bool checkUartAndSpi;
		static volatile bool sendData;
		
	public:
		static void Init();
		
		static void CompareMatchAHandler();
		
		static void CompareMatchBHandler();
		
		static void CompareMatchCHandler();

		static void CompareMatchDHandler();
		
		static void StartImuDataCommunication();

		static void StopImuDataCommunication();
		
		static void StartSonarMeasurements();
		
		static void StopSonarMeasurements();
		
		static void StopSendStatusPackets();
		
		static void StartSendStatusPackets();
		
		static void ChangeStatusPacketToFaster(bool b);	
};



#endif /* TIMERD0_H_ */