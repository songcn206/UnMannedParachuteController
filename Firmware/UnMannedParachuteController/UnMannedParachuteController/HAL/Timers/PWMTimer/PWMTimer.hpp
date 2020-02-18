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
		static constexpr float motorMaxLeft_ms = 2.0f;
		static constexpr float motorMaxRight_ms = 1.0f;
		
		static constexpr uint16_t timerTopValue = (System :: CPU_CLOCK / (pwmFrequency_Hz * preScaler)) - 1;
		static constexpr uint16_t motorMaxLeft = timerTopValue / pwmTime_ms * motorMaxLeft_ms;
		static constexpr uint16_t motorMaxRight = timerTopValue / pwmTime_ms * motorMaxLeft_ms;
		
	public:
		static void Init() {
			TCC0.CTRLB = TC0_CCDEN_bm | TC0_CCAEN_bm | TC_WGMODE_SINGLESLOPE_gc; // Add more if needed to
			TCC0.PER = timerTopValue;
			TCC0.CCD = 290; 
			TCC0.CCA = 290;
			
			TCC0.CTRLA = Timers :: GetPreScaler(preScaler);
		}
		
		static void UpdateCCReg(char reg, uint16_t value) {
			switch (reg) {
				case 'A':
					TCC0.CCA = value;
					break;
				case 'B':
					TCC0.CCB = value;
					break;
				case 'C':
					TCC0.CCC = value;
					break;
				case 'D':
					TCC0.CCD = value;
					break;
			}
		}
};



#endif /* TIMERC0_H_ */