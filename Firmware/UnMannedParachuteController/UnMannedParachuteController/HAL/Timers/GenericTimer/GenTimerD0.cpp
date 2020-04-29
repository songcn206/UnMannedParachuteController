/*
 * TimerD0.cpp
 *
 * Created: 09.01.2020 19:11:32
 *  Author: timot
 */ 
#include "GenTimerD0.hpp"
#include "HAL/SPI/ImuSPI.hpp"
#include "Control/Imu/Imu.hpp"
#include "HAL/ADC/ADC.hpp"
#include "HAL/I2C/I2C.hpp"
#include "HAL/UART/ParseUart.hpp"

volatile constexpr uint16_t GenTimerD0 :: compareMatchAValue;
volatile constexpr uint16_t GenTimerD0 :: compareMatchBValue;
volatile uint16_t GenTimerD0 :: compareMatchCValue = System :: CPU_CLOCK / GenTimerD0 :: preScaler * (compareMatchCValue_ms / 1000);
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

void  GenTimerD0 :: CompareMatchBHandler() {
	ImuSpi :: UpdateMagnetometerRegisters();
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

void GenTimerD0 :: StartSonarMeasurements() {
	TCD0.INTCTRLB |= TC_CCDINTLVL_LO_gc;
}

void GenTimerD0 :: StopImuDataCommunication() {
	TCD0.INTCTRLB &= (0xff ^ (3 << 2));
}

void GenTimerD0 :: StopSonarMeasurements() {
	TCD0.INTCTRLB &= (0xff ^ (3 << 6));
}

void GenTimerD0 :: StopSendStatusPackets() {
	TCD0.INTCTRLB &= (0xff ^ (3 << 4));
}

void GenTimerD0 :: StartSendStatusPackets() {
	TCD0.INTCTRLB |= TC_CCCINTLVL_LO_gc;
}

void GenTimerD0 :: ChangeStatusPacketToFaster(bool b) {
	System :: DisableInterruptsByPriority(System::IntLevel::Low);
	if (b) {
		compareMatchCValue = System :: CPU_CLOCK / GenTimerD0 :: preScaler * (compareMatchCValueFaster_ms / 1000);
	} else {
		compareMatchCValue = System :: CPU_CLOCK / GenTimerD0 :: preScaler * (compareMatchCValue_ms / 1000);
	}
	System :: EnableInterruptsByPriority(System::IntLevel::Low);
}