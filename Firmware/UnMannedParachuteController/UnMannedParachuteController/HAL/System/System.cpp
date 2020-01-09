/*
 * System.cpp
 *
 * Created: 04.01.2020 12:19:54
 *  Author: timot
 */ 

#include "System.hpp"
#include "HAL/UART/UART.hpp"
#include "HAL/System/Pins.hpp"

void System :: Init() {
	OSC.XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
	OSC.CTRL = OSC_XOSCEN_bm;
	while(!(OSC.STATUS & OSC_XOSCRDY_bm));
	
	OSC.PLLCTRL = OSC_PLLSRC_XOSC_gc | OSC_PLLFAC1_bm;
	OSC.CTRL |= OSC_PLLEN_bm;
	while(!(OSC.STATUS & OSC_PLLRDY_bm));
	
	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_PLL_gc;
}

void System :: EnableAllInterrupts() {
	PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
	sei();
}

void System :: DisableAllInterrupts() {
	PMIC.CTRL = (0 << PMIC_HILVLEN_bp) | (0 << PMIC_MEDLVLEN_bp) | (0 << PMIC_LOLVLEN_bp);
	cli();
}

void System :: EnableInterruptsByPriority(System :: IntLevel level) {
	switch(level) {
		case System :: IntLevel :: Low:
			PMIC.CTRL |= PMIC_LOLVLEN_bm;
			break;
		case System :: IntLevel :: Med:
			PMIC.CTRL |= PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
			break;
		case System :: IntLevel :: High:
			PMIC.CTRL |= PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
			break;
	}
}

void System :: DisableInterruptsByPriority(System :: IntLevel level) {
	switch(level) {
		case System :: IntLevel :: Low:
			PMIC.CTRL &= 0xff ^ (PMIC_LOLVLEN_bm);
			break;
		case System :: IntLevel :: Med:
			PMIC.CTRL &= 0xff ^ (PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm);
			break;
		case System :: IntLevel :: High:
			PMIC.CTRL &= 0xff ^ (PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm);
			break;
	}
}

void System :: Halt(const char* msg) {
	DisableAllInterrupts();
	while (1) {
		ExtUart :: SendString(msg);
		led1 :: Toggle();
		_delay_ms(25);
	}
}