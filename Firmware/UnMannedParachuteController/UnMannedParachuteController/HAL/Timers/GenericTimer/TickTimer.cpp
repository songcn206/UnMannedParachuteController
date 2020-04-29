/*
 * TickTimer.cpp
 *
 * Created: 04.03.2020 19:59:29
 *  Author: timot
 */ 
#include "TickTimer.hpp"

uint32_t volatile TickTimer :: ticks = 0;

ISR (TCF0_CCA_vect) {
	TickTimer :: InterruptHandler();
}

void TickTimer :: Init() {
	TCF0.CTRLB = TC_WGMODE_FRQ_gc;
	TCF0.INTCTRLB = TC_CCAINTLVL_HI_gc;
	TCF0.CCA = 32000;
	TCF0.CTRLA = Timers :: GetPreScaler(TickTimer :: preScaler);
}

void TickTimer :: InterruptHandler() {
	ticks++;
}

uint32_t TickTimer :: GetTicks() {
	System :: DisableInterruptsByPriority(System::IntLevel::High);
	uint32_t tempTicks = ticks;
	System :: EnableInterruptsByPriority(System::IntLevel::High);
	return tempTicks;
}