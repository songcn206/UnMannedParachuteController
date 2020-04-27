/*
 * TimerC0.h
 *
 * Created: 09.01.2020 19:07:13
 *  Author: timot
 */ 


#ifndef PWMTIMER_HPP_
#define PWMTIMER_HPP_

#include "HAL/System/System.hpp"
#include "HAL/Timers/Timers.hpp"

class PwmTimer {
	private:
		static constexpr float pwmFrequency_Hz = 50.0f;
		static constexpr float pwmTime_ms = 1000 / pwmFrequency_Hz;
		static constexpr uint16_t preScaler = 64;
		static constexpr float rightMotorMax_ms = 2.5f;
		static constexpr float rightMotorMin_ms = 0.76f;
		//static constexpr float leftMotorMin_ms = 2.4f;
		//static constexpr float leftMotorMax_ms = 0.78f;
		
		static constexpr uint16_t timerTopValue = (System :: CPU_CLOCK / (pwmFrequency_Hz * preScaler)) - 1;
		
	public:
		static constexpr uint16_t rightMotorMax = timerTopValue / pwmTime_ms * rightMotorMax_ms;
		static constexpr uint16_t rightMotorMin = timerTopValue / pwmTime_ms * rightMotorMin_ms;
		//static constexpr uint16_t leftMotorMax = timerTopValue / pwmTime_ms * leftMotorMax_ms;
		//static constexpr uint16_t leftMotorMin = timerTopValue / pwmTime_ms * leftMotorMin_ms;
		
	public:
		static void Init() {
			TCC0.CTRLB = TC0_CCAEN_bm | TC0_CCBEN_bm | TC_WGMODE_SINGLESLOPE_gc; // Add more if needed to
			TCC0.PER = timerTopValue;
			//TCC0.CCD = rightMotorMin;
			//TCC0.CCA = leftMotorMin;
			
			TCC0.CTRLA = Timers :: GetPreScaler(preScaler);
		}
		
		static void UpdateCCReg(char reg, uint16_t value) {
			switch (reg) {
				case 'A':
					TCC0.CCA = value; // Right1
					break;
				case 'B':
					TCC0.CCB = value; // Left2
					break;
				case 'C':
					TCC0.CCC = value; // Right2
					break;
				case 'D': 
					TCC0.CCD = value; // Left1
					break;
			}
		}
};



#endif /* TIMERC0_H_ */