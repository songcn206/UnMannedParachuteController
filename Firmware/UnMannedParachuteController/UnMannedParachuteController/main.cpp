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
#include "Control/Servos/Servos.hpp"
#include "HAL/Timers/GenericTimer/TickTimer.hpp"
#include "HAL/SPI/EepromSPI.hpp"

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
	//ImuInt :: SetPinConf();
	SonarIn :: SetPinConf();
	BatteryLevel :: SetPinConf();
	EepromCS :: SetPinConf();
	EepromMISO :: SetPinConf();
	EepromMOSI :: SetPinConf();
	EepromCK :: SetPinConf();
	EepromnHold :: SetPinConf();
	EepromnWrite :: SetPinConf();
}

int main(void) {
	System :: Init();
	PwmTimer :: Init();
	InitPins();
	_delay_ms(1000);
	ExtUart :: Init();
	GpsUart :: Init();
	DebugUart :: Init();
	
	GenTimerD0 :: Init();
	GenTimerE0 :: Init();
	TickTimer :: Init();
	ExtUart :: SendString("START!\n");
	DebugUart :: SendString("START!\n");
	Sonar :: Init();
	AbsSpi :: Init();
	ImuSpi :: Init();
	I2cDiffBaro :: Init();
	EepromSpi :: Init();
	
	bool retryNeeded = false;
	
	System :: EnableAllInterrupts();

    while (1) {
		if (GenTimerD0 :: checkUartAndSpi) {
			led2 :: Toggle();
			ParseExtUart :: Parse();
			ParseGPSUart :: Parse();
			Imu :: CheckForUpdates();
			AbsoluteBaro :: CheckForUpdates();
			DiffBaro :: CheckForUpdates();
			
			if (EepromSpi :: GetState() == EepromSpi :: SpiState :: WriteInProgress) {
				EepromSpi :: CheckWriteProgress();
			}
			
			GenTimerD0 :: checkUartAndSpi = false;
		}
		
		if (GenTimerD0 :: sendData || retryNeeded) {
			retryNeeded = !DataPackets :: SendOrSaveData(retryNeeded);
			GenTimerD0 :: sendData = false;
		}
		
		if (Servos :: GetAutoControlMotors() && GenTimerE0 :: autoControl) {
			Servos :: AutoControlMotors();
			GenTimerE0 :: autoControl = false;
		}
    }
}

