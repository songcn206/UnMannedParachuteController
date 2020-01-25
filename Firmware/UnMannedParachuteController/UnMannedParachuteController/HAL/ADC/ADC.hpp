/*
 * ADC.h
 *
 * Created: 25.01.2020 18:47:45
 *  Author: timot
 */ 


#ifndef ADC_HPP_
#define ADC_HPP_

#include "HAL/UART/UART.hpp"

class Sonar {
	public:
	
	public:
		static void Init() {
			ADCA.CH0.CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc;
			ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;
			ADCA.CH0.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_MED_gc;
			
			ADCA.REFCTRL = ADC_REFSEL_AREFA_gc;
			ADCA.PRESCALER = ADC_PRESCALER_DIV32_gc;
			ADCA.CTRLA = ADC_ENABLE_bm;
		}
		
		static void Start() {
			ADCA.CH0.CTRL |= ADC_CH_START_bm;
		}
		
		static void InterruptHandler() {
			ExtUart :: SendUInt(ADCA.CH0RES);
			ExtUart :: SendString("\n");
		}
};



#endif /* ADC_H_ */