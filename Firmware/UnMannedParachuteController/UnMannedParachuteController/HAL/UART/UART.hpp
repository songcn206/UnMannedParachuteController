/*
 * UART.h
 *
 * Created: 04.01.2020 13:27:28
 *  Author: timot
 */ 


#ifndef UART_HPP_
#define UART_HPP_

#include "HAL/System/System.hpp"
#include <avr/interrupt.h>

template <typename conf>
class Uart {
	public:
	
	private:
		static constexpr uint32_t baudRate = conf :: baudRate;
		static constexpr USART_t* uart = conf :: uart;
		static constexpr uint8_t rxInterruptPriority = conf :: RxInterrupt;
	
	public:
		static void Init() {
			InitBaudRegisters();			

			uart -> CTRLC = USART_CHSIZE_8BIT_gc;
			uart -> CTRLA = USART_TXCINTLVL_OFF_gc | rxInterruptPriority;
			if (rxInterruptPriority) {
				uart -> CTRLB = USART_TXEN_bm | USART_RXEN_bm;
			} else {
				uart -> CTRLB = USART_TXEN_bm;
			}
		}
		
		static void InitBaudRegisters() {
			// Used microchip own example code for calculating baud rate			
			uint32_t limit = 0xfffU >> 4;
			uint32_t clock_hz = System :: CPU_CLOCK;
			uint32_t ratio = clock_hz / baudRate;
			uint32_t baud = baudRate * 2; 
			uint32_t div;
			int8_t exp;

			for (exp = -7; exp < 7; exp++) {
				if (ratio < limit) {
					break;
				}
				limit <<= 1;
				if (exp < -3) {
					limit |= 1;
				}
			}
			
			if (exp < 0) {
				clock_hz -= 8 * baud;
				if (exp <= -3) {
					div = ((clock_hz << (-exp - 3)) + baud / 2) / baud;
				} else {
					baud <<= exp + 3;
					div = (clock_hz + baud / 2) / baud;
				}
			} else {
				baud <<= exp + 3;
				div = (clock_hz + baud / 2) / baud -1;
			}
			
			uart -> BAUDCTRLB = (uint8_t) (((div >> 8) & 0x0f) | (exp << 4));
			uart -> BAUDCTRLA = (uint8_t) div;  
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
		
		static void SendUInt(uint16_t value) {
			bool b = false;
			if (value >= 10000) {
				SendByte((uint8_t)(value / 10000) + '0');
				value = value % 10000;
				b = true;
			}
			if (value >= 1000 || b) {
				SendByte((uint8_t)(value / 1000) + '0');
				value = value % 1000;
				b = true;
			}
			if (value >= 100 || b) {
				SendByte((uint8_t)(value / 100) + '0');
				value = value % 100;
				b = true;
			}
			if (value >= 10 || b) {
				SendByte((uint8_t)(value / 10) + '0');
			}
			SendByte((uint8_t)(value % 10) + '0');
		}
		
		static void SendInt(int16_t value) {
			if (value < 0) {
				SendByte("-");
				value = -value;
			}
			SendUInt((uint16_t)value);
		}
		
		static void RxInterruptHandler() { // TODO!
			PORTA.OUTTGL = (PIN3_bm);
			uint8_t data = uart -> DATA;
			SendByte(data);
		}
		
};

struct ExtUartConf {
	static constexpr uint32_t baudRate = 500000;
	static constexpr USART_t* uart = &USARTF0;
	static constexpr uint8_t RxInterrupt = USART_RXCINTLVL_MED_gc;
};
typedef Uart<ExtUartConf> ExtUart;

struct GpsUartConf {
	static constexpr uint32_t baudRate = 500000;
	static constexpr USART_t* uart = &USARTD0;
	static constexpr uint8_t RxInterrupt = USART_RXCINTLVL_OFF_gc;
};
typedef Uart<GpsUartConf> GpsUart;

struct DebugUartConf {
	static constexpr uint32_t baudRate = 500000;
	static constexpr USART_t* uart = &USARTC1;
	static constexpr uint8_t RxInterrupt = USART_RXCINTLVL_OFF_gc;
};

#endif /* UART._H_ */