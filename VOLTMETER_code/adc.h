/*
 * adc.h
 *
 *  Created on: Oct 6, 2023
 *      Author: Boutros
 */

#ifndef ADC_H_
#define ADC_H_
#include"std_types.h"
#define ADC_MAXIMUM_VALUE    1023
#define ADC_REF_VOLT_VALUE   2.56
/*dynamic configuration of ADC, where we send a structure
 * that contains the reference voltage and the prescaler
 */
typedef enum{
	ADC_VCC,ADC_REF=3
}ADC_ReferenceVoltage;

typedef enum{
	ADC_PR2,ADC_2PR2,ADC_PR4,ADC_PR8,ADC_PR16,ADC_PR32,ADC_PR64,ADC_PR128
}ADC_Prescaler;

typedef struct{
	ADC_ReferenceVoltage ref_volt;
	ADC_Prescaler prescaler;
}ADC_ConfigType;
void ADC_init(const ADC_ConfigType*Config_Ptr);
uint16 ADC_readChannel(uint8 ch_num);


#endif /* ADC_H_ */
