/*
 * GenTimerE0.h
 *
 * Created: 28.01.2020 20:53:03
 *  Author: timot
 */ 


#ifndef GENTIMERE0_HPP_
#define GENTIMERE0_HPP_

#include "HAL/System/System.hpp"
#include "HAL/Timers/Timers.hpp"

class GenTimerE0 {
	public:
		static constexpr uint16_t preScaler = 1024;
		
		static constexpr float compareMatchAValue_ms = 20.0f; // Absolute Barometer
		static constexpr float compareMatchBValue_ms = 100.0f; // Auto Control 
		static constexpr float compareMatchCValue_ms = 100.0f; // Differential barometer
		//static constexpr float compareMatchDValue_ms = 50.0f;
		
		static volatile constexpr uint16_t compareMatchAValue = System :: CPU_CLOCK / preScaler * (compareMatchAValue_ms / 1000);
		static volatile constexpr uint16_t compareMatchBValue = System :: CPU_CLOCK / preScaler * (compareMatchBValue_ms / 1000);
		static volatile constexpr uint16_t compareMatchCValue = System :: CPU_CLOCK / preScaler * (compareMatchCValue_ms / 1000);
		//static volatile constexpr uint16_t compareMatchDValue = System :: CPU_CLOCK / preScaler * (compareMatchDValue_ms / 1000);

		static volatile bool autoControl;
		
	public:
		static void Init();
		
		static void CompareMatchAHandler();
		
		static void CompareMatchBHandler();
		
		static void CompareMatchCHandler();
		
		//static void CompareMatchDHandler();
		
		static void StartAbsBaroCommunication();
		
		static void StopAbsBaroCommunication();
		
		static void StartDiffBaroCommunication();

		static void StopDiffBaroCommunication();

};



#endif /* GENTIMERE0_H_ */