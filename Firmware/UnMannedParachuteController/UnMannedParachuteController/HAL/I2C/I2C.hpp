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
		static constexpr uint32_t baudRate_Hz = 100000;
		static constexpr uint8_t baud = (System :: CPU_CLOCK / (2.0f * baudRate_Hz)) - 5;
		
	public:
		static void Init() {
			PR.PRPC	&= ~PR_TWI_bm;
			TWIE.CTRL = TWI_SDAHOLD_OFF_gc; // või 50ns viivitust
			TWIE.MASTER.BAUD = baud;
			TWIE.MASTER.CTRLA = /*TWI_MASTER_INTLVL_HI_gc | TWI_MASTER_RIEN_bm | TWI_MASTER_WIEN_bm | */TWI_MASTER_ENABLE_bm; // kas kõik
			TWIE.MASTER.CTRLB = TWI_MASTER_SMEN_bm | TWI_MASTER_TIMEOUT_200US_gc; // quick command
			//TWIE.MASTER.CTRLC = ... Ack ja cmd
			TWIE.MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
		}
		
		static void InterruptHandler() {
			ExtUart :: SendString("Katkestus ");
			ExtUart :: SendUInt(TWIE.MASTER.STATUS);
			ExtUart :: SendString("\n");		
		}
		
		static void WriteAddr(uint8_t addr, bool read) {
			asm volatile ("nop");
			/*TWIE.MASTER.ADDR = (addr << 1) | read;
			while ((TWIE.MASTER.STATUS & TWI_MASTER_WIF_bm) == 0);
			asm volatile ("nop");
			TWIE.MASTER.DATA = 0x36;
			while ((TWIE.MASTER.STATUS & TWI_MASTER_WIF_bm) == 0);
			asm volatile ("nop");
			TWIE.MASTER.ADDR = (addr << 1) | true;
			_delay_us(200);
			asm volatile ("nop");
			ExtUart :: SendUInt(TWIE.MASTER.DATA);
			asm volatile ("nop");
			//ExtUart :: SendUInt(TWIE.MASTER.DATA);
			//ExtUart :: SendUInt(TWIE.MASTER.DATA);
			_delay_us(500);

			_delay_us(500);
			asm volatile ("nop");*/
		}

	
};



#endif /* I2C_H_ */