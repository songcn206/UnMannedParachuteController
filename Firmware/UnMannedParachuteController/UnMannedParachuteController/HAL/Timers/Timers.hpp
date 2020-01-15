/*
 * Timers.h
 *
 * Created: 09.01.2020 19:28:26
 *  Author: timot
 */ 


#ifndef TIMERS_HPP_
#define TIMERS_HPP_

#include "HAL/System/System.hpp"

class Timers {
	public:
		static uint8_t GetPreScaler(uint16_t preScaler) {
			switch (preScaler) {
				case 0:
					return TC_TC0_CLKSEL_OFF_gc;
				case 1:
					return TC_TC0_CLKSEL_DIV1_gc;
				case 2:
					return TC_TC0_CLKSEL_DIV2_gc;
				case 4:
					return TC_TC0_CLKSEL_DIV4_gc;
				case 8:
					return TC_TC0_CLKSEL_DIV8_gc;
				case 64:
					return TC_TC0_CLKSEL_DIV64_gc;
				case 256:
					return TC_TC0_CLKSEL_DIV256_gc;
				case 1024:
					return TC_TC0_CLKSEL_DIV1024_gc;
				default:
					System :: Halt("Timer prescaler\n");
					return 0;
			}
		}
};




#endif /* TIMERS_H_ */