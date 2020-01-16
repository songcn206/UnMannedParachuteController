/*
 * ParseUart.cpp
 *
 * Created: 16.01.2020 16:26:20
 *  Author: timot
 */ 

#include "ParseUart.hpp"

uint8_t ParseExtUart :: parseBufferPos = 0;
uint8_t ParseExtUart :: parseBuffer[ParseExtUart :: parseBufferSize] = {};
volatile uint8_t* ParseExtUart :: arrayStartPointer = ExtUart :: GetRxBufferStart();
volatile uint8_t* ParseExtUart :: arrayEndPointer = ExtUart :: GetRxBufferEnd();
volatile uint8_t* ParseExtUart :: runningPointer = ParseExtUart :: arrayStartPointer;

uint8_t ParseGPSUart :: parseBufferPos = 0;
uint8_t ParseGPSUart :: parseBuffer[ParseGPSUart :: parseBufferSize] = {};
volatile uint8_t* ParseGPSUart :: arrayStartPointer = GpsUart :: GetRxBufferStart();
volatile uint8_t* ParseGPSUart :: arrayEndPointer = GpsUart :: GetRxBufferEnd();
volatile uint8_t* ParseGPSUart :: runningPointer = ParseGPSUart :: arrayStartPointer;