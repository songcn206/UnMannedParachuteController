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
#include "HAL/UART/ParseUart.hpp"



class GenTimerD0 {
	public:
		static constexpr uint16_t preScaler = 1024;
		
		static constexpr float compareMatchAValue_ms = 10.0f;
		static constexpr float compareMatchBValue_ms = 100.0f;
		static constexpr float compareMatchCValue_ms = 100.0f;
		
		static volatile constexpr uint16_t compareMatchAValue = System :: CPU_CLOCK / preScaler * (compareMatchAValue_ms / 1000);
		static volatile constexpr uint16_t compareMatchBValue = System :: CPU_CLOCK / preScaler * (compareMatchBValue_ms / 1000);
		static volatile constexpr uint16_t compareMatchCValue = System :: CPU_CLOCK / preScaler * (compareMatchCValue_ms / 1000);
		
	public:
		static void Init() {
			TCD0.CTRLB = TC_TC0_WGMODE_NORMAL_gc;
			TCD0.INTCTRLB = TC_TC0_CCAINTLVL_LO_gc | TC_TC0_CCBINTLVL_LO_gc;
			TCD0.PER = 65535;
			TCD0.CCA = compareMatchAValue;
			TCD0.CCB = compareMatchBValue;
			
		static void CompareMatchAHandler();
		
		static void CompareMatchAHandler() {
			ExtUartParse :: Parse();
			TCD0.CCA += compareMatchAValue;
			TCD0.CTRLFSET = TC_TC0_CMD_UPDATE_gc;
		}
		
		static void CompareMatchCHandler() {

		static void StartImuDataCommunication();
}


};



#endif /* TIMERD0_H_ */