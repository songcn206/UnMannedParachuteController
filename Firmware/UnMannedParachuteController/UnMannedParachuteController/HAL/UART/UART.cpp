/*
 * UART.cpp
 *
 * Created: 07.01.2020 21:12:04
 *  Author: timot
 */ 

#include "UART.hpp"

ISR(USARTF0_RXC_vect) {
	ExtUart :: ExtRxInterruptHandler();
}

ISR(USARTD0_RXC_vect) {
	
	GpsUart :: GPSRxInterruptHandler();	
}