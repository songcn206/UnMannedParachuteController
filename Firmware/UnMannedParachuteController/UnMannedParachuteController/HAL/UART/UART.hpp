/*
 * UART.h
 *
 * Created: 04.01.2020 13:27:28
 *  Author: timot
 */ 


#ifndef UART_HPP_
#define UART_HPP_

#include "HAL/System/System.hpp"

class Uart {
	public:
	
	private:
	
	public:
		static void Init() {
			PORTF.OUTSET |= PIN3_bm;
			PORTF.DIRSET |= PIN3_bm;
			
			
			// baud and frame BSEL 131 BSCALE -3
			// BSEL 96 bSCALE -5
			USARTF0.BAUDCTRLA = 96;
			USARTF0.BAUDCTRLB = (-5 << USART_BSCALE_gp);
			USARTF0.CTRLC = USART_CHSIZE_8BIT_gc;
			USARTF0.CTRLA = USART_TXCINTLVL_OFF_gc;
			USARTF0.CTRLB = USART_TXEN_bm;
		}
		
		static void SendByte(uint8_t byte) {
			while(!(USARTF0.STATUS & USART_DREIF_bm));
			USARTF0.DATA = byte;
		}
		
		static void SendString(const char *string) {
			while (*string != 0) {
				SendByte(*string);
				string++;
			}
		}
	
	private:


	
};



#endif /* UART._H_ */