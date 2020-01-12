/*
 * UnMannedParachuteController.cpp
 *
 * Created: 03.01.2020 17:29:02
 * Author : timot
 */ 


// ===================================================
// 1) Kas praegune viis kuidas kontrollitakse uartrx t�ituvust on okei, just katkestuste v�rk?
// 2) Kuidas toimetada IMU-st tulevate andmetega nii, et uuendamine ja nende k�simine ei teeks andmeid katki
// 3) IMU magnetomeeter
// ====================================================


#include <avr/io.h>


#include "HAL/System/System.hpp"
#include "HAL/System/Pins.hpp"
#include "HAL/UART/UART.hpp"
#include "Control/StateMachine/StateMachine.hpp"
#include "HAL/UART/ParseUart.hpp"
#include "HAL/Timers/PWMTimer/PWMTimer.hpp"
#include "HAL/Timers/GenericTimer/GenTimerD0.hpp"
#include "HAL/SPI/ImuSPI.hpp"

void InitPins() {
	led1 :: SetPinConf();
	led2 :: SetPinConf();
	led3 :: SetPinConf();
	ExtUartTx :: SetPinConf();
	ExtUartRx :: SetPinConf();
	GpsUartTx :: SetPinConf();
	GpsUartRx :: SetPinConf();
	//DebugUartTx :: SetPinConf();
	//DebugUartRx :: SetPinConf();
	Left2ServoPWM :: SetPinConf();
	//Right2ServoPWM :: SetPinConf();
	//Left1ServoPWM :: SetPinConf();
	Right1ServoPWM :: SetPinConf();
	//AbsBaroCS :: SetPinConf();
	//AbsBaroMOSI :: SetPinConf();
	//AbsBaroMISO :: SetPinConf();
	//AbsBaroCK :: SetPinConf();
	ImuCS :: SetPinConf();
	ImuMOSI :: SetPinConf();
	//ImuMISO :: SetPinConf();
	ImuCK :: SetPinConf();
	//DiffBaroSDA :: SetPinConf();
	//DiffBaroSCL :: SetPinConf();
	//AbsBaroInt :: SetPinConf();
	ImuInt :: SetPinConf();
	ImuInt :: ConfigInterrupt();
	//SonarIn :: SetPinConf();
}

int main(void) {
	System :: Init();
	_delay_ms(1000);
	
	InitPins();
	ExtUart :: Init();
	GpsUart :: Init();
	PwmTimer :: Init();
	GenTimerD0 :: Init();
	ExtUart :: SendString("START!\n");
	
	
	System :: EnableAllInterrupts();
	
	ImuSpi :: Init();
	//ExtUart :: SendString("Before enable");
	
	//ExtUart :: SendString("after enable");
    /* Replace with your application code */
    while (1) {
		//ExtUart :: SendString("main\n");
		//ExtUartParse :: Parse();
		led3 :: Toggle();
	}
}

