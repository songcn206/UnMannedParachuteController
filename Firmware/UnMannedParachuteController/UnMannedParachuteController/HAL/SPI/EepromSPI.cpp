/*
 * EepromSPI.cpp
 *
 * Created: 25/04/2020 12:46:25
 *  Author: Timo Tiirats
 */ 
#include "EepromSPI.hpp"

ISR(SPIC_INT_vect) {
	EepromSpi :: InterruptHandler();
}

volatile uint8_t EepromSpi :: addressWrites;
volatile EepromSpi :: SpiState EepromSpi :: state = EepromSpi :: SpiState :: Uninited;
volatile bool EepromSpi :: sendDummyData = false;
volatile uint8_t EepromSpi :: byteCount;
volatile uint8_t* EepromSpi :: arrayPointer;
volatile uint32_t EepromSpi :: startAddress;
