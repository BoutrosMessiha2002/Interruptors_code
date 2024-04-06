/*
 * main.h
 *
 *  Created on: Feb 19, 2024
 *      Author: Boutros
 */

#ifndef MAIN_H_
#define MAIN_H_
#define FIRST_RANGE_ACTIVE PINB&(1<<PB0)
#define SECOND_RANGE_ACTIVE PINB&(1<<PB1)
#define THIRD_RANGE_ACTIVE PINB&(1<<PB2)
#define FOURTH_RANGE_ACTIVE PINB&(1<<PB3)
void MAIN_whichRange();
void MAIN_whichMode();
void MAIN_firstRange();
void MAIN_secondRange();
void MAIN_thirdRange();
void MAIN_fourthRange();
void MAIN_voltmeter();
void MAIN_ammeter();
void MAIN_whichMode();


#endif /* MAIN_H_ */
