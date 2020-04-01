/*
 * EEPROM.h
 *
 * Created: 11/02/2020 13:42:12
 *  Author: Timo Tiirats
 */ 


#ifndef EEPROM_HPP_
#define EEPROM_HPP_

#include <avr/eeprom.h>
#include "HAL/System/System.hpp"

class Eeprom {
	public:
		enum class EepromAddress : uint16_t {
			LastServoPWM,		
		};
		
	private:
		static constexpr uint16_t eepromStart = EEPROM_START;
		static constexpr uint16_t eepromEnd = EEPROM_END;
	
	public:
		static uint16_t ReadUint16(uint16_t pos) {
			return eeprom_read_word((uint16_t*)eepromStart + pos);
		}
		
		static uint8_t ReadUint8(uint16_t pos) {
			return eeprom_read_byte((uint8_t*)eepromStart + pos);
		}
		
		static void WriteUint16(uint16_t pos, uint16_t value) {
			if (eepromStart + pos >= eepromEnd) {
				System :: Halt("EEPROM OVERFLOW\n");
			}
			eeprom_write_word((uint16_t*)(eepromStart + pos), value);
		}
		
		static void WriteUint8(uint16_t pos, uint8_t value) {
			if (eepromStart + pos >= eepromEnd) {
				System :: Halt("EEPROM OVERFLOW\n");
			}
			eeprom_write_byte((uint8_t*)(eepromStart + pos), value);
		}
};



#endif /* EEPROM_H_ */