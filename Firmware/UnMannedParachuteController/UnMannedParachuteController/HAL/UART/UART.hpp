/*
 * UART.h
 *
 * Created: 04.01.2020 13:27:28
 *  Author: timot
 */ 


#ifndef UART_HPP_
#define UART_HPP_

#include "HAL/System/System.hpp"
#include "HAL/System/Pins.hpp"
#include <avr/interrupt.h>



#include <stdio.h>

template <typename conf>
class Uart {
	public:
		static constexpr uint8_t terminatingChar = conf :: terminatingChar;
		
	private:
		static constexpr uint32_t baudRate = conf :: baudRate;
		static constexpr USART_t* uart = conf :: uart;
		static constexpr uint8_t rxInterruptPriority = conf :: RxInterrupt;
		static volatile uint8_t reciveArray[conf :: rxArrayLength]; 
		static volatile uint8_t reciveArrayFreePos;
		static uint8_t reciveArrayOccupiedPos;

		
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
		
		static void SendByte(uint8_t byte) {
			while(!(uart -> STATUS & USART_DREIF_bm));
			uart -> DATA = byte;
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
		
		static void SendUIntWithZeros(uint16_t value ) {
			SendByte((uint8_t)(value / 10000) + '0');
			value = value % 10000;
			SendByte((uint8_t)(value / 1000) + '0');
			value = value % 1000;
			SendByte((uint8_t)(value / 100) + '0');
			value = value % 100;
			SendByte((uint8_t)(value / 10) + '0');
			SendByte((uint8_t)(value % 10) + '0');
		}
		
		static void SendInt(int16_t value) {
			if (value < 0) {
				SendByte('-');
				value = -value;
			}
			SendUInt((uint16_t)value);
		}
		
		static void SendFloat(float value) {
			/*char buff[20] = {0};
			sprintf(buff, "%f", (double)value);
			SendString(buff);
			*/
			
			uint16_t beforePoint = value;
			SendUInt(beforePoint);
			SendByte('.');
			value = value - (float)beforePoint;
			
			value = value * 10;
			SendByte((uint8_t)(value + '0'));
			
			value = (value-(uint8_t)value) * 10;
			SendByte((uint8_t)(value + '0'));
			
			value = (value-(uint8_t)value) * 10;
			SendByte((uint8_t)(value + '0'));
			
			value = (value-(uint8_t)value) * 10;
			SendByte((uint8_t)(value + '0'));
			
			value = (value-(uint8_t)value) * 10;
			SendByte((uint8_t)(value + '0'));
		}
		
		// Using cycling/running buffer
		static void GPSRxInterruptHandler() {
			uint8_t data = uart -> DATA;

			if (reciveArray[reciveArrayFreePos] != 0) {
				System :: Halt("GPSRX Buffer overflow\n");
			}
		
			reciveArray[reciveArrayFreePos] = data;
			if (reciveArrayFreePos == conf :: rxArrayLength - 1) {
				reciveArrayFreePos = 0;
			} else {
				reciveArrayFreePos++;
			}
		}
		
		static void ExtRxInterruptHandler() {
			uint8_t data = uart -> DATA;
				
			if (reciveArray[reciveArrayFreePos] != 0) {
				System :: Halt("ExtRX Buffer overflow\n");
			}
			//SendString("\n");
			//SendByte(data);
			//SendString("\n");
			
			// Used because removing and attaching UART dongle generated unwanted data flow.
			if (data == '\n' || data == 32 || (data > 96 && data < 123) || (data > 47 && data < 58)) {
				//SendString("\n");
				//SendByte(data);
				//SendString("\n");
				reciveArray[reciveArrayFreePos] = data;
				if (reciveArrayFreePos == conf :: rxArrayLength - 1) {
					reciveArrayFreePos = 0;
				} else {
					reciveArrayFreePos++;
				}
			}
		}


		static volatile uint8_t* GetRxBufferStart() {
			return &reciveArray[0]; 
		}
		
		static volatile uint8_t* GetRxBufferEnd() {
			return &reciveArray[conf :: rxArrayLength - 1];
		}
	
		static System :: IntLevel GetInterruptLevel() {
			switch (rxInterruptPriority) {
				case USART_RXCINTLVL_LO_gc:
					return System :: IntLevel :: Low;
				case USART_RXCINTLVL_MED_gc:
					return System :: IntLevel :: Med;
				case USART_RXCINTLVL_HI_gc:
					return System :: IntLevel :: High;
				default:
					System :: Halt("Wrong UARTRX Prio\n");
					return System :: IntLevel :: Low;
			}
		}
		
		static void DisableRX() {
			uart -> CTRLB &= (0xff ^ USART_RXEN_bm);
		}
		
		static void EnableRX() {
			uart -> CTRLB |= USART_RXEN_bm;
		}
		
	private:
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
		
		
		
		
};

template<typename conf>
uint8_t volatile Uart<conf> :: reciveArray[conf :: rxArrayLength] = {};
	
template<typename conf>
uint8_t volatile Uart<conf> :: reciveArrayFreePos = 0;


struct ExtUartConf {
	static constexpr uint32_t baudRate = 500000;
	static constexpr USART_t* uart = &USARTF0;
	static constexpr uint8_t RxInterrupt = USART_RXCINTLVL_HI_gc;
	static constexpr uint8_t rxArrayLength = 50;
	static constexpr uint8_t terminatingChar = '\n';
};
typedef Uart<ExtUartConf> ExtUart;

struct GpsUartConf {
	static constexpr uint32_t baudRate = 115200;
	static constexpr USART_t* uart = &USARTD0;
	static constexpr uint8_t RxInterrupt = USART_RXCINTLVL_HI_gc;
	static constexpr uint8_t rxArrayLength = 150;
	static constexpr uint8_t terminatingChar = '\n';
};
typedef Uart<GpsUartConf> GpsUart;

struct DebugUartConf {
	static constexpr uint32_t baudRate = 500000;
	static constexpr USART_t* uart = &USARTC1;
	static constexpr uint8_t RxInterrupt = USART_RXCINTLVL_OFF_gc;
	static constexpr uint8_t rxArrayLength = 0;
	static constexpr uint8_t terminatingChar = '\n';
};
typedef Uart<DebugUartConf> DebugUart;

#endif /* UART._H_ */