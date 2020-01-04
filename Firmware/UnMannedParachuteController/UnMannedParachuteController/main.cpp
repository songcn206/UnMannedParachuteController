/*
 * UnMannedParachuteController.cpp
 *
 * Created: 03.01.2020 17:29:02
 * Author : timot
 */ 

#include <avr/io.h>

#include "HAL/System/System.hpp"
#include "HAL/UART/UART.hpp"

int main(void) {
	PORTA.DIRSET = (PIN3_bm | PIN4_bm | PIN5_bm);
	
	System :: Init();
	Uart :: Init();
	
	
    /* Replace with your application code */
    while (1) {
		PORTA.OUTTGL = (PIN3_bm | PIN4_bm | PIN5_bm);
		_delay_ms(100);
		PORTA.OUTTGL = (PIN3_bm | PIN4_bm | PIN5_bm);
		_delay_ms(100);
		Uart :: SendString("Timo\n");
    }
}

