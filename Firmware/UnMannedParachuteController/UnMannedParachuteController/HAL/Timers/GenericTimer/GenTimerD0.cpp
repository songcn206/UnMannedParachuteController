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

volatile constexpr uint16_t GenTimerD0 :: compareMatchAValue;
volatile constexpr uint16_t GenTimerD0 :: compareMatchBValue;
volatile constexpr uint16_t GenTimerD0 :: compareMatchCValue;

ISR(TCD0_CCA_vect) {
	GenTimerD0 :: CompareMatchAHandler();
}

ISR(TCD0_CCB_vect) {
	GenTimerD0 :: CompareMatchBHandler();
}

ISR(TCD0_CCC_vect) {
	GenTimerD0 :: CompareMatchCHandler();
}

void  GenTimerD0 :: Init() {
	//ExtUart :: SendString("timer init\n");
	TCD0.CTRLB = TC_WGMODE_NORMAL_gc;
	TCD0.INTCTRLB = TC_CCAINTLVL_MED_gc | TC_CCCINTLVL_LO_gc;
	TCD0.PER = 65535;
	TCD0.CCA = GenTimerD0 :: compareMatchAValue;
	TCD0.CCB =  GenTimerD0 :: compareMatchBValue;
	TCD0.CCC = GenTimerD0 :: compareMatchCValue;
	
	TCD0.CTRLA = Timers :: GetPreScaler(GenTimerD0 :: preScaler);
}

void GenTimerD0 :: CompareMatchAHandler() {
	//ExtUart :: SendString("compareA\n");
	ParseExtUart :: Parse();
	ParseGPSUart :: Parse();
	Imu :: CheckForUpdates();
	TCD0.CCA +=  GenTimerD0 :: compareMatchAValue;
	TCD0.CTRLFSET = TC_CMD_UPDATE_gc;
}

void  GenTimerD0 :: CompareMatchBHandler() {
	//ExtUart :: SendString("compareB\n");
	ImuSpi :: UpdateMagnetometerRegisters();
	TCD0.CCB +=  GenTimerD0 :: compareMatchBValue;
	TCD0.CTRLFSET = TC_CMD_UPDATE_gc;
}

void  GenTimerD0 :: CompareMatchCHandler() {
	//ExtUart :: SendString("compareC\n");
	StateMachine :: sendStatus();
	TCD0.CCC +=  GenTimerD0 :: compareMatchCValue;
	TCD0.CTRLFSET = TC_CMD_UPDATE_gc;
}

void  GenTimerD0 :: StartImuDataCommunication() {
	//ExtUart :: SendString("startdatacom");
	TCD0.INTCTRLB |= TC_CCBINTLVL_MED_gc;
}