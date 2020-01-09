/*
 * System.h
 *
 * Created: 04.01.2020 12:19:30
 *  Author: timot
 */ 


#ifndef SYSTEM_HPP_
#define SYSTEM_HPP_

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU (32000000UL)
#include <util/delay.h>



class System {
	public:
		static constexpr uint32_t CPU_CLOCK = F_CPU;
		
		enum class IntLevel {
			Low,
			Med,
			High
			};
		
	private:
	
	public:
		static void Init();
		static void EnableAllInterrupts();
		static void DisableAllInterrupts();
		static void EnableInterruptsByPriority(IntLevel level);
		static void DisableInterruptsByPriority(IntLevel level);
		static void Halt(const char* msg);
		
	private:
	
};



#endif /* SYSTEM_H_ */