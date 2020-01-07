/*
 * port.h
 *
 * Created: 07.01.2020 23:03:05
 *  Author: timot
 */ 


#ifndef PORT_HPP_
#define PORT_HPP_

#include <avr/io.h>


class Port {
	public:
		enum class IOPorts {
			A = (uint32_t)&PORTA,
			B = (uint32_t)&PORTB,
			C = (uint32_t)&PORTC,
			D = (uint32_t)&PORTD,
			E = (uint32_t)&PORTE,
			F = (uint32_t)&PORTF,
		};
	
	
		enum class PortOutputState {
			TotemPole =		PORT_OPC_TOTEM_gc,			
			BusKeeper =		PORT_OPC_BUSKEEPER_gc,		
			PullDown =		PORT_OPC_PULLDOWN_gc,		
			PullUp =		PORT_OPC_PULLUP_gc,			
			WierdOr =		PORT_OPC_WIREDOR_gc,		
			WiredAnd =		PORT_OPC_WIREDAND_gc,		
			WiredOrPull =	PORT_OPC_WIREDORPULL_gc,	
			WiredAndPull =	PORT_OPC_WIREDANDPULL_gc
		};
		
		enum class PortDirection {
			Input,
			Output
		};
	
	private:
	
	public:
		static void InitPin(const IOPorts ioPort, uint8_t pinNr ,const PortDirection dir, const PortOutputState output) {
			PORT_t* port = (PORT_t*)ioPort;
			uint8_t mask = (1 << pinNr);
			switch(dir) {
				case PortDirection :: Input:
					port -> DIRCLR = mask;
					break;
				case PortDirection :: Output:
					port -> DIRSET = mask;
					break;
			}

		}
		
	private:
	
	
};



#endif /* PORT_H_ */