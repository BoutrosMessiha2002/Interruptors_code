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
#include"std_types.h"

/*General*/
float voltage;
float adc_value;
uint8 num=0;

/*Voltmeter*/
uint8 status;

/*Ammeter*/
float amp;

/***********************__AC Ammeter__****************************/
//float max;

/*Ohmmeter*/
float resistance;

int main()
{
	ADC_ConfigType config={
			ADC_VCC,ADC_PR64
	};
	ADC_init(&config);

	LCD_init();

    /*Keypad internal pull up*/
    PORTD|=(1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7);

	/*Mode selection*/
	DDRA|=(1<<PA6);
	DDRB|=(1<<PB7);

	/*Voltmeter*/
    DDRC|=(1<<PC6)|(1<<PC7);
    DDRB|=(1<<PB4);

    /*Ammeter*/
    SET_BIT(DDRB,PIN5);
    SET_BIT(DDRB,PIN6);
    SET_BIT(DDRB,PIN7);


while(1)
{
 MAIN_whichMode();
}
}
void MAIN_whichMode()
{


	LCD_clearScreen();
	LCD_displayString("V=1 A=2 O=3");
		while(num==0 || num==4 || num == '#')
		{
			num=KEYPAD_getPressedKey();
		}


	if(num==1) //Voltmeter
	{
		num=0;
		LCD_clearScreen();
		LCD_displayString("DC=1,AC=2");
		while(num==0 || num==4)
		{
			num=KEYPAD_getPressedKey();
		}
		if(num==1)
		{
			//Mode MUX
			num=0;
			PORTB&=~(1<<PB7);
			PORTA&=~(1<<PA6);

			PORTB&=~(1<<PB4);
			MAIN_whichRange();
		}
		else if(num==2)
		{
			//Mode MUX
			num=0;
			PORTB|=(1<<PB7);
			PORTA&=~(1<<PA6);

			PORTB|=(1<<PB4);
			MAIN_whichRange();
		}

		if(num!=0||num!=1)
			MAIN_whichMode();
	}  //Voltmeter

	if (num==2) //Ammeter
	{
		num=0;
		AmmeterMode();
	}

if (num==3) //Ohmmeter
	{
		num=0;
		//Mode MUX
		PORTA|=(1<<PA6);
		PORTB&=~(1<<PB7);
		OhmmeterRange();
	}

}





void MAIN_whichRange()
{
	LCD_clearScreen();
	LCD_displayString("Select Range ");
	while(num==0 || num==4 || num == '#')
	{
	num=KEYPAD_getPressedKey();
	}
	if(num==1)
	{
		num=0;
		PORTC&=~(1<<PC6);
		PORTC&=~(1<<PC7);
		MAIN_firstRange();
	}
	else if(num==2)
	{
		num=0;
		PORTC|=(1<<PC6);
		PORTC&=~(1<<PC7);
		MAIN_secondRange();
	}
	else if(num==3)
	{
		num=0;
		PORTC&=~(1<<PC6);
		PORTC|=(1<<PC7);
		MAIN_thirdRange();
	}
	else if(num==5)
	{
		num=0;
		PORTC|=(1<<PC6);
		PORTC|=(1<<PC7);
		MAIN_fourthRange();
	}
	MAIN_whichRange();
}
void MAIN_firstRange()
{
	LCD_clearScreen();
	LCD_displayString("FIRST");
	_delay_ms(200);
	LCD_clearScreen();
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
			_delay_ms(150);

		}
	}
	num=0;
	MAIN_whichMode();
}
void MAIN_secondRange()
{
	LCD_clearScreen();
	LCD_displayString("SECOND");
	_delay_ms(200);
	LCD_clearScreen();
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
			_delay_ms(150);
			}
}
	num=0;
	MAIN_whichMode();
}
void MAIN_thirdRange()
{
	LCD_clearScreen();
	LCD_displayString("THIRD");
	_delay_ms(200);
	LCD_clearScreen();
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
		_delay_ms(150);
	}
}
num=0;
MAIN_whichMode();
}
void MAIN_fourthRange()
{
	LCD_clearScreen();
	LCD_displayString("FOURTH");
	_delay_ms(200);
	LCD_clearScreen();
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
				_delay_ms(150);

			}
}
	num=0;
	MAIN_whichMode();
}



void AmmeterMode(){

	//Reset keypad value
	num=0;

	//LCD_clearScreen();
	//LCD_displayString("A: DC=1 AC=2");
	//while(num==0 || num==4)
	//{
	//	num=KEYPAD_getPressedKey();
	//}

	//if(num==1)
	//{
	LCD_clearScreen();
	LCD_displayString("Choose Range");
	while(num==0 || num==4)
		{
			num=KEYPAD_getPressedKey();
		}
	if(num==3)
		{
			GPIO_writePin(PORTB_ID, PIN5_ID, 0);
			GPIO_writePin(PORTB_ID, PIN6_ID, 0);
			GPIO_writePin(PORTB_ID, PIN7_ID, 1);
			_delay_ms(300);

				LCD_clearScreen();
				LCD_displayString("3rd range");
				_delay_ms(500);
				LCD_clearScreen();
				while(KEYPAD_getPressedKey()!='#' )
				{
					adc_value=ADC_readChannel(1);
					voltage = adc_value*(5.0/1023);
					amp=voltage/(10.0*0.4); //Range 100mA:1A (Rs=0.4)
					LCD_clearScreen();
					LCD_displayString("Amp=");
					LCD_displayFloat(amp,5);
					_delay_ms(150);
					LCD_clearScreen();
				}
				num=0;
			}
			else if (num==2)
			{

				GPIO_writePin(PORTB_ID, PIN5_ID, 0);
				GPIO_writePin(PORTB_ID, PIN6_ID, 1);
				GPIO_writePin(PORTB_ID, PIN7_ID, 0);
				_delay_ms(300);


					LCD_clearScreen();
					LCD_displayString("2nd range");
					_delay_ms(200);

					while(KEYPAD_getPressedKey()!='#')
					{
						adc_value=ADC_readChannel(1);
						voltage = adc_value*(5.0/1023);
						amp=voltage/(10.0*4.5); //Range 10:100mA (Rs=4.5)
						LCD_clearScreen();
						LCD_displayString("Amp=");
						LCD_displayFloat(amp,5);
						_delay_ms(150);
						LCD_clearScreen();
					}
					num=0;
				}
				else if(num==1)
				{
					LCD_clearScreen();
					LCD_displayString("1st range");
					_delay_ms(200);

					GPIO_writePin(PORTB_ID, PIN5_ID, 1);
					GPIO_writePin(PORTB_ID, PIN6_ID, 0);
					GPIO_writePin(PORTB_ID, PIN7_ID, 0);
					_delay_ms(300);


					while(KEYPAD_getPressedKey()!='#')
					{
						adc_value=ADC_readChannel(1);
						voltage = adc_value*(5.0/1023);
						amp=voltage/(10.0*45.0); //Range 0:10mA (Rs=45)
						LCD_clearScreen();
						LCD_displayString("Amp=");
						LCD_displayFloat(amp,5);
						_delay_ms(150);
						LCD_clearScreen();
					}
					num=0;
				}
	MAIN_whichMode();
			}

	//}

/***********************************__AC_Ammeter__*****************************************/
	/*
	else if(num==2)
	{
		num=0;
		LCD_clearScreen();
		LCD_displayString("Choose Range");
		while(num==0 || num==4){
				num=KEYPAD_getPressedKey();
			}
		LCD_clearScreen();
			LCD_displayString("out&=");
			LCD_displayFloat(num,2);
			_delay_ms(500);
			LCD_clearScreen();

			if(num==1){
				LCD_displayString("1st range");
				_delay_ms(500);
				LCD_clearScreen();
				GPIO_writePin(PORTB_ID, PIN0_ID, 1);
				GPIO_writePin(PORTB_ID, PIN1_ID, 0);
				_delay_ms(100);

				adc_value=ADC_readChannel(1);
				max=adc_value;
				for(int i=0;i<500;i++)
				{
					adc_value=ADC_readChannel(1);
					_delay_ms(1);
					if(adc_value>max)
					{
					max=adc_value;
					}
				}
				adc_value=max;
				LCD_displayFloat(adc_value,2);
				_delay_ms(500);
				LCD_clearScreen();

				voltage = adc_value*(5.0/1023);
				amp=voltage/(10.0*45.1); //Range 0:10mA (Rs=45)
				LCD_displayString("Amp=");
				LCD_displayFloat(amp,5);
				_delay_ms(500);
				LCD_clearScreen();
				num=0;
			}

			if(num==2){
					LCD_displayString("2nd range");
					_delay_ms(500);
					LCD_clearScreen();

					GPIO_writePin(PORTB_ID, PIN0_ID, 0);
					GPIO_writePin(PORTB_ID, PIN1_ID, 1);
					_delay_ms(100);

					adc_value=ADC_readChannel(1);
									max=adc_value;
									for(int i=0;i<50;i++)
									{
										adc_value=ADC_readChannel(1);
										if(adc_value>max)
										{
										max=adc_value;
										}
									}
									adc_value=max;
					LCD_displayString("ADC=");
					LCD_displayFloat(adc_value,2);
					_delay_ms(500);
					LCD_clearScreen();

					voltage = adc_value*(5.0/1023);
					amp=voltage/(10.0*4.6); //Range 10:100mA (Rs=4.5)
					LCD_displayString("Amp=");
					LCD_displayFloat(amp,5);
					_delay_ms(500);
					LCD_clearScreen();
					num=0;
				}

			if(num==3){
					LCD_displayString("3rd range");
					_delay_ms(500);
					LCD_clearScreen();

					GPIO_writePin(PORTB_ID, PIN0_ID, 1);
					GPIO_writePin(PORTB_ID, PIN1_ID, 1);
					_delay_ms(100);

					adc_value=ADC_readChannel(1);
									max=adc_value;
									for(int i=0;i<50;i++)
									{
										adc_value=ADC_readChannel(1);
										if(adc_value>max)
										{
										max=adc_value;
										}
									}
									adc_value=max;
					LCD_displayString("ADC=");
					LCD_displayFloat(adc_value,2);
					_delay_ms(500);
					LCD_clearScreen();

					voltage = adc_value*(5.0/1023);
					amp=voltage/(10.0*0.54); //Range 100mA:1A (Rs=45)
					LCD_displayString("Amp=");
					LCD_displayFloat(amp,5);
					_delay_ms(500);
					LCD_clearScreen();
					num=0;

				}
	}
	*/

void OhmmeterRange(){
	num=0;
	LCD_clearScreen();
	LCD_displayString("Choose Range");
	while((num==0)||(num==4)){

		num=KEYPAD_getPressedKey();
	}

	if(num==1){
		LCD_clearScreen();
		LCD_displayString("1st Range");

		GPIO_writePin(PORTB_ID,PIN5_ID,0);
		GPIO_writePin(PORTB_ID,PIN6_ID,0);
		_delay_ms(500);

		while(KEYPAD_getPressedKey()!='#')
		{
			adc_value=ADC_readChannel(2);
			voltage = adc_value*(5.0/1023);
			resistance = (((voltage/5.0)*(2200+1050)-700)/(1-voltage/5.0));//Rmux=350 Rs=2.2k Rmax=10k
			LCD_clearScreen();
			LCD_displayFloat(resistance,2);
			LCD_displayString("Ohm");
			_delay_ms(150);
		}
		num=0;
	}
	else if(num==2){
		LCD_clearScreen();
		LCD_displayString("2nd Range");


		GPIO_writePin(PORTB_ID,PIN5_ID,1);
		GPIO_writePin(PORTB_ID,PIN6_ID,0);
		_delay_ms(500);

		while(KEYPAD_getPressedKey()!='#')
		{
			adc_value=ADC_readChannel(2);

			voltage = adc_value*(5.0/1023);
			resistance = (((voltage/5.0)*(22+1.05)-0.7)/(1-voltage/5.0));//Rmux=350 Rs=22k Rmax=89k
			LCD_clearScreen();
			LCD_displayFloat(resistance,2);
			LCD_displayString("Kohm");
			_delay_ms(150);
		}
			num=0;
	}
	else if(num==3){
		LCD_clearScreen();
		LCD_displayString("3rd Range");

		GPIO_writePin(PORTB_ID,PIN5_ID,0);
		GPIO_writePin(PORTB_ID,PIN6_ID,1);
		_delay_ms(500);

		while(KEYPAD_getPressedKey()!='#')
		{
			adc_value=ADC_readChannel(2);
			voltage = adc_value*(5.0/1023);
			resistance =(((voltage/5.0)*(150+1.05)-0.7)/(1-voltage/5.0));//Rmux=350 Rs=150k Rmax=600k
			LCD_clearScreen();
			LCD_displayFloat(resistance,2);
			LCD_displayString("Kohm");
			_delay_ms(150);
		}
		num=0;
	}
	MAIN_whichMode();
}
