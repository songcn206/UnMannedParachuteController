/*
 * TimerD0.cpp
 *
 * Created: 09.01.2020 19:11:32
 *  Author: timot
 */ 
#include "GenTimerD0.hpp"

volatile constexpr uint16_t GenTimerD0 :: compareMatchAValue;
volatile constexpr uint16_t GenTimerD0 :: compareMatchBValue;

ISR(TCD0_CCA_vect) {
	GenTimerD0 :: CompareMatchAHandler();
}

ISR(TCD0_CCB_vect) {
	GenTimerD0 :: CompareMatchBHandler();
}