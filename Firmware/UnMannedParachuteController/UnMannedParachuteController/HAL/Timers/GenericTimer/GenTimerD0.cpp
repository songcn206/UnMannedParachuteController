/*
 * TimerD0.cpp
 *
 * Created: 09.01.2020 19:11:32
 *  Author: timot
 */ 
#include "GenTimerD0.hpp"
#include "HAL/SPI/ImuSPI.hpp"
#include "Control/Imu/Imu.hpp"
#include "Control/StateMachine/StateMachine.hpp"
#include "HAL/ADC/ADC.hpp"
#include "HAL/I2C/I2C.hpp"

volatile constexpr uint16_t GenTimerD0 :: compareMatchAValue;
volatile constexpr uint16_t GenTimerD0 :: compareMatchBValue;
volatile constexpr uint16_t GenTimerD0 :: compareMatchCValue;
volatile constexpr uint16_t GenTimerD0 :: compareMatchDValue;

volatile bool GenTimerD0 :: checkUartAndSpi = false;
volatile bool GenTimerD0 :: sendData = false;

ISR(TCD0_CCA_vect) {
	GenTimerD0 :: CompareMatchAHandler();
}

ISR(TCD0_CCB_vect) {
	GenTimerD0 :: CompareMatchBHandler();
}

ISR(TCD0_CCC_vect) {
	GenTimerD0 :: CompareMatchCHandler();
}

ISR(TCD0_CCD_vect) {
	GenTimerD0 :: CompareMatchDHandler();
}

void  GenTimerD0 :: Init() {
	//ExtUart :: SendString("timer init\n");
	TCD0.CTRLB = TC_WGMODE_NORMAL_gc;
	TCD0.INTCTRLB = TC_CCAINTLVL_LO_gc | TC_CCCINTLVL_LO_gc | TC_CCDINTLVL_LO_gc;
	TCD0.PER = 65535;
	TCD0.CCA = GenTimerD0 :: compareMatchAValue;
	TCD0.CCB = GenTimerD0 :: compareMatchBValue;
	TCD0.CCC = GenTimerD0 :: compareMatchCValue;
	TCD0.CCD = GenTimerD0 :: compareMatchDValue;
	
	TCD0.CTRLA = Timers :: GetPreScaler(GenTimerD0 :: preScaler);
}

void GenTimerD0 :: CompareMatchAHandler() {
	checkUartAndSpi = true;
	TCD0.CCA +=  GenTimerD0 :: compareMatchAValue;
	TCD0.CTRLFSET = TC_CMD_UPDATE_gc;
}

// IMU start comm
void  GenTimerD0 :: CompareMatchBHandler() {
	ImuSpi :: UpdateMagnetometerRegisters();
	I2cDiffBaro :: GetData();
	TCD0.CCB +=  GenTimerD0 :: compareMatchBValue;
	TCD0.CTRLFSET = TC_CMD_UPDATE_gc;
}

void  GenTimerD0 :: CompareMatchCHandler() {
	sendData = true;
	TCD0.CCC +=  GenTimerD0 :: compareMatchCValue;
	TCD0.CTRLFSET = TC_CMD_UPDATE_gc;
}

void  GenTimerD0 :: CompareMatchDHandler() {
	Sonar :: Start();
	TCD0.CCD +=  GenTimerD0 :: compareMatchDValue;
	TCD0.CTRLFSET = TC_CMD_UPDATE_gc;
}

void  GenTimerD0 :: StartImuDataCommunication() {
	TCD0.INTCTRLB |= TC_CCBINTLVL_MED_gc;
}