/*
 * AbsImu.cpp
 *
 * Created: 28.01.2020 18:38:23
 *  Author: timot
 */ 
#include "AbsSpi.hpp"

ISR(SPID_INT_vect) {
	AbsSpi :: InterruptHandler();
}

volatile bool AbsSpi :: dataWritten;
volatile AbsSpi :: SpiState AbsSpi :: state = AbsSpi :: SpiState :: Uninited;
volatile uint8_t AbsSpi :: data[3] = {};
volatile uint8_t AbsSpi :: dataPointer = 0;
volatile uint8_t AbsSpi :: settingsPointer = 0;

volatile uint8_t AbsSpi :: DataAddr[3] = {
0x28,
0x29,
0x2a
};

struct AbsSpi :: ImuSettings set0 = {.addr = 0x10, .value = 0b00100000}; 
	
volatile AbsSpi :: ImuSettings AbsSpi :: settings[1] = {
set0
};
