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
		static constexpr float compareMatchBValue_ms = 50.0f; // Imu and Differential barometer
		static constexpr float compareMatchCValue_ms = 200.0f; // Save data
		static constexpr float compareMatchDValue_ms = 50.0f; // Sonar
		
		static volatile constexpr uint16_t compareMatchAValue = System :: CPU_CLOCK / preScaler * (compareMatchAValue_ms / 1000);
		static volatile constexpr uint16_t compareMatchBValue = System :: CPU_CLOCK / preScaler * (compareMatchBValue_ms / 1000);
		static volatile constexpr uint16_t compareMatchCValue = System :: CPU_CLOCK / preScaler * (compareMatchCValue_ms / 1000);
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

		static void StopImuSonarDiffBaro();
		
		static void StartImuSonarDiffBaro();
		
		static void StopSendStatusPackets();
		
		static void StartSendStatusPackets();
};



#endif /* TIMERD0_H_ */