/*
 * Pins.cpp
 *
 * Created: 11.01.2020 19:01:43
 *  Author: timot
 */ 
#include "Pins.hpp"
#include "System.hpp"

ISR(PORTF_INT0_vect) {
	led2 :: SetHigh();
}