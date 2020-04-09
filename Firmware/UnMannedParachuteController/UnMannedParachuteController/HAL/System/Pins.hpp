/*
 * port.h
 *
 * Created: 07.01.2020 23:03:05
 *  Author: timot
 */ 


#ifndef PINS_HPP_
#define PINS_HPP_

#include <avr/io.h>

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

enum class StartingValue {
	High,
	Low	
};

template <IOPorts ioPort, uint8_t pinNr, PortDirection dir, PortOutputState state = PortOutputState::TotemPole, 
			StartingValue start = StartingValue :: Low>
class Pins {
	private:
		static constexpr PORT_t* port = (PORT_t*)ioPort;
		static constexpr uint8_t mask = 1 << pinNr;
	
	public:		
		static void SetPinConf() {
			switch(dir) {
				case PortDirection :: Input:
					port -> DIRCLR = mask;
					break;
				case PortDirection :: Output:
					port -> DIRSET = mask;
					break;
			}
			PORTCFG.MPCMASK = mask;
			port -> PIN0CTRL = (uint8_t) state;
			
			if (start == StartingValue :: High && dir == PortDirection::Output) {
				port -> OUTSET = mask;
			}
		}
		
		static void SetHigh() {
			port -> OUTSET = mask;
		}
		
		static void SetLow() {
			port -> OUTCLR = mask;
		}
		
		static void Toggle() {
			port -> OUTTGL = mask;
		}
		
		static void ConfigInterrupt() {
			port -> INTCTRL = PORT_INT0LVL_HI_gc;
			port -> INT0MASK = mask;
		}
};

// LEDs
typedef Pins<IOPorts :: A, 3, PortDirection :: Output, PortOutputState :: TotemPole, StartingValue :: Low> led1;
typedef Pins<IOPorts :: A, 4, PortDirection :: Output, PortOutputState :: TotemPole, StartingValue :: Low> led2;
typedef Pins<IOPorts :: A, 5, PortDirection :: Output, PortOutputState :: TotemPole, StartingValue :: Low> led3;

// UARTs
typedef Pins<IOPorts :: F, 3, PortDirection :: Output, PortOutputState :: TotemPole, StartingValue :: High> ExtUartTx;
typedef Pins<IOPorts :: F, 2, PortDirection :: Input, PortOutputState :: PullUp, StartingValue :: High> ExtUartRx;
typedef Pins<IOPorts :: D, 3, PortDirection :: Output, PortOutputState :: TotemPole, StartingValue :: High> GpsUartTx;
typedef Pins<IOPorts :: D, 2, PortDirection :: Input, PortOutputState :: PullUp, StartingValue :: High> GpsUartRx;
typedef Pins<IOPorts :: C, 7, PortDirection :: Output, PortOutputState :: TotemPole, StartingValue :: High> DebugUartTx;
typedef Pins<IOPorts :: C, 6, PortDirection :: Input, PortOutputState :: PullUp, StartingValue :: High> DebugUartRx;

// Servos
typedef Pins<IOPorts :: C, 0, PortDirection :: Output, PortOutputState :: TotemPole, StartingValue :: Low> Left2ServoPWM;
typedef Pins<IOPorts :: C, 1, PortDirection :: Output, PortOutputState :: TotemPole, StartingValue :: Low> Right2ServoPWM;
typedef Pins<IOPorts :: C, 2, PortDirection :: Output, PortOutputState :: TotemPole, StartingValue :: Low> Left1ServoPWM;
typedef Pins<IOPorts :: C, 3, PortDirection :: Output, PortOutputState :: TotemPole, StartingValue :: Low> Right1ServoPWM;

// SPIs
typedef Pins<IOPorts :: D, 4, PortDirection :: Output, PortOutputState :: TotemPole, StartingValue :: High> AbsBaroCS;
typedef Pins<IOPorts :: D, 5, PortDirection :: Output, PortOutputState :: TotemPole, StartingValue :: High> AbsBaroMOSI;
typedef Pins<IOPorts :: D, 6, PortDirection :: Input, PortOutputState :: PullUp> AbsBaroMISO;
typedef Pins<IOPorts :: D, 7, PortDirection :: Output, PortOutputState :: TotemPole, StartingValue :: High> AbsBaroCK;

typedef Pins<IOPorts :: E, 4, PortDirection :: Output, PortOutputState :: TotemPole, StartingValue :: High> ImuCS;
typedef Pins<IOPorts :: E, 5, PortDirection :: Output, PortOutputState :: TotemPole, StartingValue :: High> ImuMOSI;
typedef Pins<IOPorts :: E, 6, PortDirection :: Input, PortOutputState :: PullUp> ImuMISO;
typedef Pins<IOPorts :: E, 7, PortDirection :: Output, PortOutputState :: TotemPole, StartingValue :: High> ImuCK;

// I2C
typedef Pins<IOPorts :: E, 0, PortDirection :: Output, PortOutputState :: TotemPole> DiffBaroSDA;
typedef Pins<IOPorts :: E, 1, PortDirection :: Output, PortOutputState :: TotemPole> DiffBaroSCL;

// Interrupt pins
typedef Pins<IOPorts :: D, 0, PortDirection :: Input> AbsBaroInt;
typedef Pins<IOPorts :: F, 0, PortDirection :: Input> ImuInt;

// Sonar
typedef Pins<IOPorts :: A, 1, PortDirection :: Input> SonarIn;

#endif /* PORT_H_ */