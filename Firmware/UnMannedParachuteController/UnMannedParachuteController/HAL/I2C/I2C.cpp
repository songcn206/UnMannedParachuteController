/*
 * I2C.cpp
 *
 * Created: 30.01.2020 12:12:03
 *  Author: timot
 */ 

#include "I2C.hpp"

ISR(TWIE_TWIM_vect ) {
	I2cDiffBaro :: InterruptHandler();
}

volatile uint8_t I2cDiffBaro :: counter = 0;