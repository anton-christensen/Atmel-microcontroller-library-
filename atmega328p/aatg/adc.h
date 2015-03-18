/** 
* File:    adc.h 
* 
* Author:   Anton Christensen (anton.christensen9700@gmail.com)
* Date:     January 2014 
* 
* Description: 
* 	implements a software layer to simplify analogue to digital conversion with ATMega16
*
*/

#ifndef __adc_h__
#define __adc_h__

#include <avr/io.h>

#define ADC_REF_AREF 0 			// Default
#define ADC_REF_AVCC 1
//#define ADC_REF_RESERVED 2
#define ADC_REF_2_56V 3

void adc_enable();						// enables ADC
void adc_disable();						// disables ADC
void adc_set_ref(char refference_mode); // sets the refference mode
unsigned int adc_read(char pin_select); // read value from ADC input on pin #pin_select as a 10 bit value


///////////////////////////////////////////////


// enable adc, set auto trigger enable and set prescaler 16 (1/16 f system clock as sample rate)
void adc_enable()  {ADCSRA |=  1<<ADEN | 1<<ADATE | 1<<ADPS2;}
void adc_disable() {ADCSRA &= ~1<<ADEN;}

void adc_set_ref(char refference_mode) {
	if(refference_mode < 4) {
		ADMUX &=  0b00111111;					// clear refference selection bits
		ADMUX |=  (refference_mode << REFS0);	// set refference selection
	}
}

unsigned int adc_read(char pin_select) {
	pin_select %= 8;
	ADMUX &= 0b11100000; 				// Clear the pin select register 
	ADMUX |= pin_select; 				// Set pin to read from (single ended mode) 
	_delay_us(100); 						// Delay needed for the stabilization of the ADC input voltage
	ADCSRA|=(1<<ADSC); 					// Initiates the conversion of selected AD pin
	while ((ADCSRA & (1<<ADIF)) == 0); 	// Wait for the AD conversion to complete
	ADCSRA|=(1<<ADIF);					// Clears pending interrupt               
	
	return ADCW;
}

#endif