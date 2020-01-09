/*
 * UnMannedParachuteController.cpp
 *
 * Created: 03.01.2020 17:29:02
 * Author : timot
 */ 

#include <avr/io.h>


#include "HAL/System/System.hpp"
#include "HAL/System/Pins.hpp"
#include "HAL/UART/UART.hpp"
#include "Control/StateMachine/StateMachine.hpp"

void InitPins() {
	led1 :: SetPinConf();
	led2 :: SetPinConf();
	led3 :: SetPinConf();
	ExtUartTx :: SetPinConf();
	ExtUartRx :: SetPinConf();
	GpsUartTx :: SetPinConf();
	GpsUartRx :: SetPinConf();
	DebugUartTx :: SetPinConf();
	DebugUartRx :: SetPinConf();
	Left2ServoPWM :: SetPinConf();
	Right2ServoPWM :: SetPinConf();
	Left1ServoPWM :: SetPinConf();
	Right1ServoPWM :: SetPinConf();
	AbsBaroCS :: SetPinConf();
	AbsBaroMOSI :: SetPinConf();
	AbsBaroMISO :: SetPinConf();
	AbsBaroCK :: SetPinConf();
	ImuCS :: SetPinConf();
	ImuMOSI :: SetPinConf();
	ImuMISO :: SetPinConf();
	ImuCK :: SetPinConf();
	DiffBaroSDA :: SetPinConf();
	DiffBaroSCL :: SetPinConf();
	AbsBaroInt :: SetPinConf();
	ImuInt :: SetPinConf();
	SonarIn :: SetPinConf();
}

void CheckUarts() {
	if (ExtUart :: IsByteRecived()) {
		// ParseUart :: ParseExtUart();
	}	
}


int main(void) {
	
	System :: Init();
	InitPins();
	ExtUart :: Init();

	System :: EnableAllInterrupts();
	
    /* Replace with your application code */
    while (1) {
		CheckUarts();
    }
}

