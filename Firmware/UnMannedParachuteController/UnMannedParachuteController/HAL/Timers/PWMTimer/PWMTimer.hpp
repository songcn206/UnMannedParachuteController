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
		static constexpr float motorMax_ms = 2.35f;
		static constexpr float motorMin_ms = 0.77f;
		
		static constexpr uint16_t timerTopValue = (System :: CPU_CLOCK / (pwmFrequency_Hz * preScaler)) - 1;
		
	public:
		static constexpr uint16_t motorMax = timerTopValue / pwmTime_ms * motorMax_ms;
		static constexpr uint16_t motorMin = timerTopValue / pwmTime_ms * motorMin_ms;
		
	public:
		static void Init() {
			TCC0.CTRLB = TC0_CCDEN_bm | TC0_CCAEN_bm | TC_WGMODE_SINGLESLOPE_gc; // Add more if needed to
			TCC0.PER = timerTopValue;
			TCC0.CCD = motorMax;
			TCC0.CCA = motorMax;
			
			TCC0.CTRLA = Timers :: GetPreScaler(preScaler);
		}
		
		static void UpdateCCReg(char reg, uint16_t value) {
			switch (reg) {
				case 'A':
					TCC0.CCA = value; // Left2
					break;
				case 'B':
					TCC0.CCB = value; // Right2
					break;
				case 'C':
					TCC0.CCC = value; // Left1
					break;
				case 'D': 
					TCC0.CCD = value; // Right1
					break;
			}
		}
};



#endif /* TIMERC0_H_ */