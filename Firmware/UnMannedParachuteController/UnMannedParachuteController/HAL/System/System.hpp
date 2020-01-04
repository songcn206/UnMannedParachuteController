/*
 * System.h
 *
 * Created: 04.01.2020 12:19:30
 *  Author: timot
 */ 


#ifndef SYSTEM_HPP_
#define SYSTEM_HPP_

#include <avr/io.h>

#define F_CPU (32000000UL)
#include <util/delay.h>


class System {
	public:
		static constexpr uint32_t CPU_CLOCK = F_CPU;
	private:
	
	public:
		static void Init();
		
	private:
	
};



#endif /* SYSTEM_H_ */