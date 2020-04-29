/*
 * EEPROM.cpp
 *
 * Created: 25.04.2020 20:12:46
 *  Author: timot
 */ 
#include "EEPROM.hpp"

uint32_t ExternalEeprom :: currentAddress = 0;
uint16_t ExternalEeprom :: currentPage = 0;
uint8_t ExternalEeprom :: writesWithinPage = 0;
uint16_t ExternalEeprom :: dataPacketNr = 0;
uint8_t ExternalEeprom :: dataToBeSaved[bytesPerPacket];
uint32_t ExternalEeprom :: lastPacketNr = 0;
volatile uint8_t ExternalEeprom :: dataToBeSent[bytesPerPacket];
bool ExternalEeprom :: firstPacket = true;