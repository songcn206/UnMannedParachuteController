/*
 * TickTimer.h
 *
 * Created: 04.03.2020 19:59:40
 *  Author: timot
 */ 


#ifndef TICKTIMER_HPP_
#define TICKTIMER_HPP_

#include "HAL/System/System.hpp"
#include "HAL/Timers/Timers.hpp"

class TickTimer {
	public:
	
	private:
		static constexpr uint16_t preScaler = 1;
		static volatile uint32_t ticks;
	
	public:
		static void Init();	
		static void InterruptHandler();
		static uint32_t GetTicks();
};



#endif /* TICKTIMER_H_ */