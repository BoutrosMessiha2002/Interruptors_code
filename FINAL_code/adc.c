/*
 * adc.c
 *
 *  Created on: Oct 6, 2023
 *      Author: Boutros
 */
#include"common_macros.h"
#include "adc.h"
#include<avr/io.h>
void ADC_init(const ADC_ConfigType*Config_Ptr)
{
	ADMUX=(ADMUX&0x3F)|((Config_Ptr->ref_volt)<<6);
	SET_BIT(ADCSRA,ADEN);
	CLEAR_BIT(ADCSRA,ADIE);
	ADCSRA=(ADCSRA&0xF8)|(Config_Ptr->prescaler);
}
uint16 ADC_readChannel(uint8 ch_num)
{
	ADMUX=(ADMUX&0xE0)|(ch_num&0x1F);
	SET_BIT(ADCSRA,ADSC);
	while(BIT_IS_CLEAR(ADCSRA,ADIF));
	CLEAR_BIT(ADCSRA,ADIF);
	return ADC;
}

