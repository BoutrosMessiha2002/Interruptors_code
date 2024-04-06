/*
 * main.c
 *
 *  Created on: Nov 7, 2023
 *      Author: Boutros
 */
#include"gpio.h"
#include"keypad.h"
#include "adc.h"
#include"lcd.h"
#include"main.h"
#include"avr/io.h"
#include<util/delay.h>
#include"common_macros.h"
float voltage;
float adc_value;
uint8 num=0;
uint8 num2=0;
uint8 status;
int main()
{
	ADC_ConfigType config={
			ADC_VCC,ADC_PR64
	};
	ADC_init(&config);
	LCD_init();
    DDRC|=(1<<PC6)|(1<<PC7);
    DDRB|=(1<<PB4);
    PORTD|=(1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7);
while(1)
{
 MAIN_whichMode();
}
}
void MAIN_whichMode()
{
	LCD_displayString("DC=1,AC=2");
	num=KEYPAD_getPressedKey();
	if(num==1)
	{
		PORTB&=~(1<<PB4);
		MAIN_whichRange();
	}
	else if(num==2)
	{
		PORTB|=(1<<PB4);
		MAIN_whichRange();
	}
	LCD_clearScreen();
	if(num!=0||num!=1)
		MAIN_whichMode();
}
void MAIN_ammeter()
{
	LCD_clearScreen();
while(KEYPAD_getPressedKey()!='=')
{
	LCD_displayCharacter('g');
}
}
void MAIN_voltmeter()
{
MAIN_whichMode();
}

void MAIN_whichRange()
{
	LCD_displayString("Select Range: ");
	num2=KEYPAD_getPressedKey();
	if(num2==1)
	{
		PORTC&=~(1<<PC6);
		PORTC&=~(1<<PC7);
		MAIN_firstRange();
	}
	else if(num2==2)
	{
		PORTC|=(1<<PC6);
		PORTC&=~(1<<PC7);
		MAIN_secondRange();
	}
	else if(num2==3)
	{
		PORTC&=~(1<<PC6);
		PORTC|=(1<<PC7);
		MAIN_thirdRange();
	}
	else if(num2==5)
	{
		PORTC|=(1<<PC6);
		PORTC|=(1<<PC7);
		MAIN_fourthRange();
	}
	LCD_clearScreen();
	MAIN_whichRange();
}
void MAIN_firstRange()
{
	LCD_clearScreen();
	LCD_displayString("FIRST");
	while(KEYPAD_getPressedKey()!='#')
	{
		LCD_clearScreen();
		adc_value=ADC_readChannel(0);
		voltage=(adc_value*(5.0/1023))*2.59;
				if(voltage>=10)
					LCD_displayString("OUT OF RANGE");
				else
				{
				LCD_displayFloat(voltage,2);
				}
	}
	num=0;
	MAIN_whichMode();
}
void MAIN_secondRange()
{
	LCD_clearScreen();
	LCD_displayString("SECOND");
	while(KEYPAD_getPressedKey()!='#')
	{
		LCD_clearScreen();
	adc_value=ADC_readChannel(0);
	voltage=(adc_value*(5.0/1023))*11.11;
			if(voltage>=50)
				LCD_displayString("OUT OF RANGE");
			else
			{
			LCD_displayFloat(voltage,2);
			}
}
	num=0;
	MAIN_whichMode();
}
void MAIN_thirdRange()
{
	LCD_clearScreen();
	LCD_displayString("THIRD");
while(KEYPAD_getPressedKey()!='#')
{
	LCD_clearScreen();
	adc_value=ADC_readChannel(0);
	voltage=adc_value*(5.0/1023)*25.54;
			if(voltage>=100)
				LCD_displayString("OUT OF RANGE");
			else
			{
			LCD_displayFloat(voltage,2);
			}
}
num=0;
MAIN_whichMode();
}
void MAIN_fourthRange()
{
	LCD_clearScreen();
	LCD_displayString("FOURTH");
	while(KEYPAD_getPressedKey()!='#')
	{
	LCD_clearScreen();
	adc_value=ADC_readChannel(0);
	voltage=(adc_value*(5.0/1023))*53.6;
			if(voltage>200)
				LCD_displayString("OUT OF RANGE");
			else
			{
			LCD_displayFloat(voltage,2);
			}
}
	num=0;
	MAIN_whichMode();
}
