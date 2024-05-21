/*
 * main.c
 *
 *  Created on: Nov 7, 2023
 *      Author: Boutros, Tadros, Mina
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
float adc_value; //Input value of ADC
float voltage;   //Actual voltage on ADC
uint8 num=0;     //Number of pressed key on keypad


/*Ammeter*/
float amp;

/*Ohmmeter*/
float resistance;

int main()
{
	/*ADC initialization*/
	ADC_ConfigType config={
			ADC_VCC,ADC_PR64
	};
	ADC_init(&config);

	/*LCD initialization*/
	LCD_init();

    /*Keypad internal pull up activation*/
    PORTD|=(1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7);

	/*Mode selection pins configuration*/
	DDRA|=(1<<PA6);
	DDRB|=(1<<PB7);

	/*Voltmeter MUX pins configuration*/
    DDRC|=(1<<PC6)|(1<<PC7);
    DDRB|=(1<<PB4);

    /*Ammeter pins configuration*/
    SET_BIT(DDRB,PIN5);
    SET_BIT(DDRB,PIN6);


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

	if(num==1) //Voltmeter selected
	{
		/*Restart keypad value*/
		num=0;

		/*Set LCD display*/
		LCD_clearScreen();
		LCD_displayString("DC=1,AC=2");

		/*Internal polling*/
		while(num==0 || num==4)
		{
			num=KEYPAD_getPressedKey();
		}

		if(num==1)	//DC
		{
			/*Restart keypad value*/
			num=0;

			/*Set values for mode MUX selection pins*/
			PORTB&=~(1<<PB7);
			PORTA&=~(1<<PA6);
			PORTB&=~(1<<PB4);

			MAIN_whichRange();
		}
		else if(num==2)	 //AC selected
		{
			/*Restart keypad value*/
			num=0;
			/*Set values for mode MUX selection pins*/
			PORTB|=(1<<PB7);
			PORTA&=~(1<<PA6);
			PORTB|=(1<<PB4);

			MAIN_whichRange();
		}

		/*Default status*/
		if(num!=0||num!=1)
			MAIN_whichMode();
	}

	if (num==2) //Ammeter selected
	{
		/*Restart keypad value*/
		num=0;

		AmmeterMode();
	}

if (num==3) //Ohmmeter selected
	{
		/*Restart keypad value*/
		num=0;
		/*Set values for mode MUX selection pins*/
		PORTA|=(1<<PA6);
		PORTB&=~(1<<PB7);

		OhmmeterRange();
	}

}




/*Voltmeter Ranges*/
void MAIN_whichRange()
{
	/*Set LCD display*/
	LCD_clearScreen();
	LCD_displayString("Select Range ");

	/*Internal polling*/
	while(num==0 || num==4 || num == '#')
	{
		num=KEYPAD_getPressedKey();
	}

	if(num==1) //First range selected
	{
		/*Restart keypad value*/
		num=0;

		/*Set values for ranges MUX selection pins*/
		PORTC&=~(1<<PC6);
		PORTC&=~(1<<PC7);

		MAIN_firstRange();
	}
	else if(num==2) //Second range selected
	{
		/*Restart keypad value*/
		num=0;

		/*Set values for ranges MUX selection pins*/
		PORTC|=(1<<PC6);
		PORTC&=~(1<<PC7);

		MAIN_secondRange();
	}
	else if(num==3) //Third range selected
	{
		/*Restart keypad value*/
		num=0;

		/*Set values for ranges MUX selection pins*/
		PORTC&=~(1<<PC6);
		PORTC|=(1<<PC7);

		MAIN_thirdRange();
	}
	else if(num==5) //Fourth range selected
	{
		/*Restart keypad value*/
		num=0;

		/*Set values for ranges MUX selection pins*/
		PORTC|=(1<<PC6);
		PORTC|=(1<<PC7);

		MAIN_fourthRange();
	}
	/*Default status*/
	MAIN_whichRange();
}

/*Voltmeter first range (measuring from 0 to less than 10V)*/
void MAIN_firstRange()
{
	/*Set LCD display*/
	LCD_clearScreen();
	LCD_displayString("FIRST");
	_delay_ms(200);
	LCD_clearScreen();


	while(KEYPAD_getPressedKey()!='#') //Condition to exit Voltmeter
	{
		/*Clear screen to view results*/
		LCD_clearScreen();

		/*Read ADC value*/
		adc_value=ADC_readChannel(0);

		/*Calculate actual voltage*/
		voltage=(adc_value*(5.0/1023))*2.59;

		if(voltage>=10) //Check range is correct
			LCD_displayString("OUT OF RANGE");
		else
		{
			LCD_displayFloat(voltage,2);
			_delay_ms(150);
		}
	}
	/*Restart keypad value*/
	num=0;
	/*ٌReturn to mode view*/
	MAIN_whichMode();
}

/*Voltmeter second range (measuring from 10V to less than 50V)*/
void MAIN_secondRange()
{
	/*Set LCD display*/
	LCD_clearScreen();
	LCD_displayString("SECOND");
	_delay_ms(200);
	LCD_clearScreen();

	while(KEYPAD_getPressedKey()!='#') //Condition to exit Voltmeter
	{
		/*Clear screen to view results*/
		LCD_clearScreen();

		/*Read ADC value*/
		adc_value=ADC_readChannel(0);

		/*Calculate actual voltage*/
		voltage=(adc_value*(5.0/1023))*11.11;

		if(voltage>=50) //Check range is correct
			LCD_displayString("OUT OF RANGE");
		else
		{
		LCD_displayFloat(voltage,2);
		_delay_ms(150);
		}
}
	/*Restart keypad value*/
	num=0;
	/*ٌReturn to mode view*/
	MAIN_whichMode();
}

/*Voltmeter third range (measuring from 50V to less than 100V)*/
void MAIN_thirdRange()
{
	/*Set LCD display*/
	LCD_clearScreen();
	LCD_displayString("THIRD");
	_delay_ms(200);
	LCD_clearScreen();

	while(KEYPAD_getPressedKey()!='#') //Condition to exit Voltmeter
	{
		/*Clear screen to view results*/
		LCD_clearScreen();

		/*Read ADC value*/
		adc_value=ADC_readChannel(0);

		/*Calculate actual voltage*/
		voltage=adc_value*(5.0/1023)*25.54;

		if(voltage>=100) //Check range is correct
			LCD_displayString("OUT OF RANGE");
		else
		{
			LCD_displayFloat(voltage,2);
			_delay_ms(150);
		}
	}
	/*Restart keypad value*/
	num=0;
	/*ٌReturn to mode view*/
	MAIN_whichMode();
}

/*Voltmeter third range (measuring from 100V to 200V)*/
void MAIN_fourthRange()
{
	/*Set LCD display*/
	LCD_clearScreen();
	LCD_displayString("FOURTH");
	_delay_ms(200);
	LCD_clearScreen();

	while(KEYPAD_getPressedKey()!='#')//Condition to exit Voltmeter
	{
		/*Clear screen to view results*/
		LCD_clearScreen();

		/*Read ADC value*/
		adc_value=ADC_readChannel(0);

		/*Calculate actual voltage*/
		voltage=(adc_value*(5.0/1023))*53.6;

		if(voltage>200) //Check range is correct
			LCD_displayString("OUT OF RANGE");
		else
		{
			LCD_displayFloat(voltage,2);
			_delay_ms(150);
		}
	}
	/*Restart keypad value*/
	num=0;
	/*ٌReturn to mode view*/
	MAIN_whichMode();
}


/*Determining ammeter range*/
void AmmeterMode()
{
	/*Restart keypad value*/
	num=0;

	/*Set LCD display*/
	LCD_clearScreen();
	LCD_displayString("Choose Range");

	/*Internal polling*/
	while(num==0 || num==4)
	{
		num=KEYPAD_getPressedKey();
	}

	if(num==3) /*Ammeter third range (measuring from 100mA to 1A)*/
	{
		/*Set relays values*/
		GPIO_writePin(PORTB_ID, PIN5_ID, 0);
		GPIO_writePin(PORTB_ID, PIN6_ID, 0);
		GPIO_writePin(PORTB_ID, PIN7_ID, 1);

		/*Delay for inductor to work probarly*/
		_delay_ms(300);

		/*Set LCD display*/
		LCD_clearScreen();
		LCD_displayString("3rd range");
		_delay_ms(500);
		LCD_clearScreen();

		while(KEYPAD_getPressedKey()!='#' ) //Condition to exit Ammeter
		{
			/*Read ADC value*/
			adc_value=ADC_readChannel(1);

			/*Calculate actual voltage*/
			voltage = adc_value*(5.0/1023);

			/*Calculate Current value*/
			amp=voltage/(10.0*0.4);

			/*Set LCD display*/
			LCD_clearScreen();
			LCD_displayString("Amp=");
			LCD_displayFloat(amp,5);
			_delay_ms(150);
			LCD_clearScreen();
		}
		/*Restart keypad value*/
		num=0;
	}
	else if (num==2) /*Ammeter second range (measuring from 10mA to 100mA)*/
	{
		/*Set relays values*/
		GPIO_writePin(PORTB_ID, PIN5_ID, 0);
		GPIO_writePin(PORTB_ID, PIN6_ID, 1);
		GPIO_writePin(PORTB_ID, PIN7_ID, 0);

		/*Delay for inductor to work probarly*/
		_delay_ms(300);

		/*Set LCD display*/
		LCD_clearScreen();
		LCD_displayString("2nd range");
		_delay_ms(200);

		while(KEYPAD_getPressedKey()!='#') //Condition to exit Ammeter
		{
			/*Read ADC value*/
			adc_value=ADC_readChannel(1);

			/*Calculate actual voltage*/
			voltage = adc_value*(5.0/1023);

			/*Calculate Current value*/
			amp=voltage/(10.0*4.5);

			/*Set LCD display*/
			LCD_clearScreen();
			LCD_displayString("Amp=");
			LCD_displayFloat(amp,5);
			_delay_ms(150);
			LCD_clearScreen();
		}
		/*Restart keypad value*/
		num=0;
	}
	else if(num==1) /*Ammeter second range (measuring from 0A to 10mA)*/
	{
		/*Set LCD display*/
		LCD_clearScreen();
		LCD_displayString("1st range");
		_delay_ms(200);

		/*Set relays values*/
		GPIO_writePin(PORTB_ID, PIN5_ID, 1);
		GPIO_writePin(PORTB_ID, PIN6_ID, 0);
		GPIO_writePin(PORTB_ID, PIN7_ID, 0);

		/*Delay for inductor to work probarly*/
		_delay_ms(300);


		while(KEYPAD_getPressedKey()!='#') //Condition to exit Ammeter
		{
			/*Read ADC value*/
			adc_value=ADC_readChannel(1);

			/*Calculate actual voltage*/
			voltage = adc_value*(5.0/1023);

			/*Calculate Current value*/
			amp=voltage/(10.0*45.0);

			/*Set LCD display*/
			LCD_clearScreen();
			LCD_displayString("Amp=");
			LCD_displayFloat(amp,5);
			_delay_ms(150);
			LCD_clearScreen();
		}
		/*Restart keypad value*/
		num=0;
	}

	/*ٌReturn to mode view*/
	MAIN_whichMode();

}



/*Determining ohmmeter range*/
void OhmmeterRange()
{
	/*Restart keypad value*/
	num=0;

	/*Set LCD display*/
	LCD_clearScreen();
	LCD_displayString("Choose Range");

	/*Internal polling*/
	while((num==0)||(num==4))
	{

		num=KEYPAD_getPressedKey();
	}

	if(num==1) /*Ohmmeter first range (measuring from up to 10Kohm)*/
	{
		/*Set LCD display*/
		LCD_clearScreen();
		LCD_displayString("1st Range");

		/*Set values for ranges MUX selection pins*/
		GPIO_writePin(PORTB_ID,PIN5_ID,0);
		GPIO_writePin(PORTB_ID,PIN6_ID,0);
		_delay_ms(500);

		while(KEYPAD_getPressedKey()!='#') //Condition to exit Ohmmeter
		{
			/*Read ADC value*/
			adc_value=ADC_readChannel(2);

			/*Calculate actual voltage*/
			voltage = adc_value*(5.0/1023);

			/*Calculate resistance value*/
			resistance = (((voltage/5.0)*(2200+1050)-700)/(1-voltage/5.0));

			/*Set LCD display*/
			LCD_clearScreen();
			LCD_displayFloat(resistance,2);
			LCD_displayString("Ohm");
			_delay_ms(150);
		}
		/*Restart keypad value*/
		num=0;
	}
	else if(num==2) /*Ohmmeter second range (measuring from up to 89Kohm)*/
	{
		/*Set LCD display*/
		LCD_clearScreen();
		LCD_displayString("2nd Range");

		/*Set values for ranges MUX selection pins*/
		GPIO_writePin(PORTB_ID,PIN5_ID,1);
		GPIO_writePin(PORTB_ID,PIN6_ID,0);
		_delay_ms(500);

		while(KEYPAD_getPressedKey()!='#') //Condition to exit Ohmmeter
		{
			/*Read ADC value*/
			adc_value=ADC_readChannel(2);

			/*Calculate actual voltage*/
			voltage = adc_value*(5.0/1023);

			/*Calculate resistance value*/
			resistance = (((voltage/5.0)*(22+1.05)-0.7)/(1-voltage/5.0));

			/*Set LCD display*/
			LCD_clearScreen();
			LCD_displayFloat(resistance,2);
			LCD_displayString("Kohm");
			_delay_ms(150);
		}
		/*Restart keypad value*/
		num=0;
	}
	else if(num==3) /*Ohmmeter third range (measuring from up to 600Kohm)*/
	{
		/*Set LCD display*/
		LCD_clearScreen();
		LCD_displayString("3rd Range");

		/*Set values for ranges MUX selection pins*/
		GPIO_writePin(PORTB_ID,PIN5_ID,0);
		GPIO_writePin(PORTB_ID,PIN6_ID,1);
		_delay_ms(500);

		while(KEYPAD_getPressedKey()!='#') //Condition to exit Ohmmeter
		{
			/*Read ADC value*/
			adc_value=ADC_readChannel(2);

			/*Calculate actual voltage*/
			voltage = adc_value*(5.0/1023);

			/*Calculate resistance value*/
			resistance =(((voltage/5.0)*(150+1.05)-0.7)/(1-voltage/5.0));

			/*Set LCD display*/
			LCD_clearScreen();
			LCD_displayFloat(resistance,2);
			LCD_displayString("Kohm");
			_delay_ms(150);
		}
		/*Restart keypad value*/
		num=0;
	}
	/*ٌReturn to mode view*/
	MAIN_whichMode();
}
