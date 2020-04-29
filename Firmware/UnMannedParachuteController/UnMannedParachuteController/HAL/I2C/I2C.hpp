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
#include "HAL/Timers/GenericTimer/GenTimerE0.hpp"

class I2cDiffBaro {
	public:
		enum class Action {
			Write,
			Read	
		};
		
		static constexpr uint8_t I2cDiffBaroInterruptLevel = (uint8_t)System :: IntLevel :: Med;
	
	private:
		static constexpr uint32_t baudRate_Hz = 100000;
		static constexpr uint8_t baud = (System :: CPU_CLOCK / (2.0f * baudRate_Hz)) - 5;
		
		static volatile uint8_t dataCounter;
		static volatile Action action;
		static volatile uint8_t pressure[2];
		static volatile bool measureReady;
		
	public:
		static void Init() {
			TWIE.CTRL = TWI_SDAHOLD_50NS_gc;
			TWIE.MASTER.BAUD = baud;
			TWIE.MASTER.CTRLB = TWI_MASTER_TIMEOUT_DISABLED_gc;
			TWIE.MASTER.CTRLA = TWI_MASTER_INTLVL_MED_gc | TWI_MASTER_RIEN_bm | TWI_MASTER_WIEN_bm | TWI_MASTER_ENABLE_bm;
			TWIE.MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
			TWIE.SLAVE.CTRLA = 0;
			
			GenTimerE0 :: StartDiffBaroCommunication();
		}
		
		static void InterruptHandler() {
			if (TWIE.MASTER.STATUS & TWI_MASTER_WIF_bm) {
				WriteInterruptHandler();
			} else if (TWIE.MASTER.STATUS & TWI_MASTER_RIF_bm) {
				ReadInterruptHandler();
			}
		}
		
		static void GetData() {
			System :: DisableInterruptsByPriority((System :: IntLevel)I2cDiffBaroInterruptLevel);
			TWIE.MASTER.ADDR = 0x6c << 1;
			action = Action :: Write;
			dataCounter = 0;
			System :: EnableInterruptsByPriority((System :: IntLevel)I2cDiffBaroInterruptLevel);
		}
		
		static void WriteInterruptHandler() {
			if (TWIE.MASTER.STATUS & TWI_MASTER_RXACK_bm) {
				TWIE.MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
				ExtUart :: SendString("NACK\n");
			} else if (action == Action :: Write) {
				TWIE.MASTER.DATA = 0x30;
				action = Action :: Read;
			} else if (action == Action :: Read) {
				TWIE.MASTER.ADDR = (0x6c << 1) | 1;
			} else {
				TWIE.MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
				ExtUart :: SendString("WRITESTOP\n");
			}
		}
		
		static void ReadInterruptHandler() {
			pressure[dataCounter] = TWIE.MASTER.DATA;
			if (dataCounter == 0) {
				TWIE.MASTER.CTRLC = 0 | TWI_MASTER_CMD_RECVTRANS_gc;
				dataCounter++;
			} else {
				measureReady = true;
				TWIE.MASTER.CTRLC = (1 << 2) | TWI_MASTER_CMD_STOP_gc;
			}
		}
		
		
		static bool GetMeasureReady() {
			return measureReady; // Interrupts disabled at reading
		}
		
		static void SetMeasureReady(bool b) {
			measureReady = b; // Interrupts disabled at writing
		}
		
		static volatile uint8_t* GetPointer() {
			return &pressure[0]; // Interrupts disabled at reading
		}
};



#endif /* I2C_H_ */