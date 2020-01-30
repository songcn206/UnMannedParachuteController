/*
 * I2C.h
 *
 * Created: 30.01.2020 12:11:48
 *  Author: timot
 */ 


#ifndef I2C_HPP_
#define I2C_HPP_

#include "HAL/System/System.hpp"
#include "HAL/UART/UART.hpp"

class I2cDiffBaro {
	public:
	
	private:
		static constexpr uint32_t baudRate_Hz = 125000;
		static constexpr uint8_t baud = (System :: CPU_CLOCK / (2.0f * baudRate_Hz)) - 5;
		
		static volatile uint8_t counter;
		
	public:
		static void Init() {
			//PR.PRPC	&= ~PR_TWI_bm;
			TWIE.CTRL = TWI_SDAHOLD_50NS_gc; // või 50ns viivitust
			TWIE.MASTER.BAUD = baud;
			 // kas kõik
			TWIE.MASTER.CTRLB = TWI_MASTER_TIMEOUT_DISABLED_gc; // quick command
			TWIE.MASTER.CTRLA = TWI_MASTER_INTLVL_HI_gc | TWI_MASTER_RIEN_bm | TWI_MASTER_WIEN_bm | TWI_MASTER_ENABLE_bm;
			//TWIE.MASTER.CTRLC = ... Ack ja cmd
			TWIE.MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;

			TWIE.SLAVE.CTRLA = 0;
		}
		
		static void InterruptHandler() {
			asm volatile ("nop");
			if ((TWIE.MASTER.STATUS & TWI_MASTER_RXACK_bm) && (counter == 0)) {

				TWIE.MASTER.DATA = 0x36;
				asm volatile ("nop");
				counter = 1;
			} else if ((TWIE.MASTER.STATUS & TWI_MASTER_RXACK_bm) && (counter == 1)) {
				asm volatile ("nop");
				TWIE.MASTER.ADDR = (0x6c << 1) | true;
				counter = 2;
			} else {
				asm volatile ("nop");
				ExtUart :: SendUInt(TWIE.MASTER.DATA);
			}
			asm volatile ("nop");
		}
		
		static void WriteAddr(uint8_t addr, bool read) {
			asm volatile ("nop");
			TWIE.MASTER.ADDR = (addr << 1) | read;
			//while ((TWIE.MASTER.STATUS & TWI_MASTER_WIF_bm) == 0);
			asm volatile ("nop");
			/*
			TWIE.MASTER.DATA = 0x36;
			//while ((TWIE.MASTER.STATUS & TWI_MASTER_WIF_bm) == 0);
			asm volatile ("nop");
			TWIE.MASTER.ADDR = (addr << 1) | true;
			_delay_us(200);
			asm volatile ("nop");
			ExtUart :: SendUInt(TWIE.MASTER.DATA);
			asm volatile ("nop");
			//ExtUart :: SendUInt(TWIE.MASTER.DATA);
			//ExtUart :: SendUInt(TWIE.MASTER.DATA);
			//_delay_us(500);

			//_delay_us(500);
			asm volatile ("nop");*/
		}

	
};



#endif /* I2C_H_ */