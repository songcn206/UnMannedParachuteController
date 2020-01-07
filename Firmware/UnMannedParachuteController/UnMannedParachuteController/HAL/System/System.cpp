/*
 * System.cpp
 *
 * Created: 04.01.2020 12:19:54
 *  Author: timot
 */ 

#include "System.hpp"

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

void System :: EnableInterrupts() {
	PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
	sei();
}