/*
 * GenTimerE0.cpp
 *
 * Created: 28.01.2020 20:54:04
 *  Author: timot
 */ 

#include "GenTimerE0.hpp"
#include "HAL/SPI/AbsSpi.hpp"

volatile constexpr uint16_t GenTimerE0 :: compareMatchAValue;

ISR(TCE0_CCA_vect) {
	GenTimerE0 :: CompareMatchAHandler();
}

void  GenTimerE0 :: Init() {
	//ExtUart :: SendString("timer init\n");
	TCE0.CTRLB = TC_WGMODE_NORMAL_gc;
	TCE0.INTCTRLB = 0;
	TCE0.PER = 65535;
	TCE0.CCA = GenTimerE0 :: compareMatchAValue;
	
	TCE0.CTRLA = Timers :: GetPreScaler(GenTimerE0 :: preScaler);
}

void GenTimerE0 :: CompareMatchAHandler() {
	AbsSpi :: StartDataCommunication();
	TCE0.CCA +=  GenTimerE0 :: compareMatchAValue;
	TCE0.CTRLFSET = TC_CMD_UPDATE_gc;
}

void GenTimerE0 :: StartAbsBaroDataCommunication() {
	TCE0.INTCTRLB |= TC_CCAINTLVL_MED_gc;
}