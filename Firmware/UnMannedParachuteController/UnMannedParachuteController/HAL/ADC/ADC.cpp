/*
 * ADC.cpp
 *
 * Created: 25.01.2020 18:47:56
 *  Author: timot
 */ 
#include "ADC.hpp"

uint16_t volatile Sonar :: distance_mm;

ISR(ADCA_CH0_vect) {
	Sonar :: InterruptHandler();
}