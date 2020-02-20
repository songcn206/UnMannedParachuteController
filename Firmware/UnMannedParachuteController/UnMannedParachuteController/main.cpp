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
#include "HAL/UART/ParseUart.hpp"
#include "HAL/Timers/PWMTimer/PWMTimer.hpp"
#include "HAL/Timers/GenericTimer/GenTimerD0.hpp"
#include "HAL/SPI/ImuSPI.hpp"
#include "HAL/DataPackets/DataPackets.hpp"
#include "HAL/ADC/ADC.hpp"
#include "HAL/SPI/AbsSpi.hpp"
#include "Control/AbsBaro/AbsBaro.hpp"
#include "HAL/I2C/I2C.hpp"
#include "Control/DiffBaro/DiffBaro.hpp"
#include "HAL/EEPROM/EEPROM.hpp"

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
	//AbsBaroInt :: SetPinConf();
	ImuInt :: SetPinConf();
	//ImuInt :: ConfigInterrupt();
	SonarIn :: SetPinConf();
}

int main(void) {
	System :: Init();
	_delay_ms(1000);
	InitPins();
	ExtUart :: Init();
	GpsUart :: Init();
	PwmTimer :: Init();
	GenTimerD0 :: Init();
	GenTimerE0 :: Init();
	ExtUart :: SendString("START!\n");
	Sonar :: Init();
	AbsSpi :: Init();
	ImuSpi :: Init();
	I2cDiffBaro :: Init();
	
	System :: EnableAllInterrupts();
		
	//uint16_t i = 265;
	//bool sign = false;
	
    while (1) {
		if (GenTimerD0 :: checkUartAndSpi) {
			ParseExtUart :: Parse();
			ParseGPSUart :: Parse();
			Imu :: CheckForUpdates();
			AbsoluteBaro :: CheckForUpdates();
			DiffBaro :: CheckForUpdates();
			GenTimerD0 :: checkUartAndSpi = false;
			/*
			if (i >= 1180) {
				sign = true;
			} else if (i <= 290) {
				sign = false;
			}
			
			if (sign) {
				i -= 1;
			} else {
				i += 1;
			}
			PwmTimer :: UpdateCCReg('A', i);
			PwmTimer :: UpdateCCReg('D', i);*/
		}
		
		if (GenTimerD0 :: sendData) {
			DataPackets :: SendOrSaveData();
			GenTimerD0 :: sendData = false;
			/*if (sign) {
				PwmTimer :: UpdateCCReg('A', 500);
				PwmTimer :: UpdateCCReg('D', 500);
				sign = false;
			} else {
				PwmTimer :: UpdateCCReg('A', 300);
				PwmTimer :: UpdateCCReg('D', 300);
				sign = true;
			}*/
		}
		
	}
}

