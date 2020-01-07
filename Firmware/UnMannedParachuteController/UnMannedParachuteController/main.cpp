/*
 * UnMannedParachuteController.cpp
 *
 * Created: 03.01.2020 17:29:02
 * Author : timot
 */ 

#include <avr/io.h>


#include "HAL/System/System.hpp"
#include "HAL/System/port.hpp"
#include "HAL/UART/UART.hpp"


int main(void) {
	PORTA.DIRSET = (PIN3_bm | PIN4_bm | PIN5_bm);
	
	Port :: InitPin(Port::IOPorts::A, 3, Port::PortDirection::Output, Port::PortOutputState::TotemPole);
	
	PORTF.OUTSET |= PIN3_bm;
	PORTF.DIRSET |= PIN3_bm;
	
	System :: Init();
	ExtUart :: Init();
	
	System :: EnableInterrupts();
	
    /* Replace with your application code */
    while (1) {
		_delay_ms(100);
		ExtUart :: SendString("Timo\n");
    }
}

