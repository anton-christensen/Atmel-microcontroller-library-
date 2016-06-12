 /** 
  * File:   interrupts.h 
  * 
  * Author: Anton Christensen (anton.christensen9700@gmail.com)
  * Date:   January 2014 
  * 
  * Description: 
  * 	Aims to ease the use of setting up and toggling internal and external interrupts on the ATMega16
  *
  * Usage:
  * 	External interrupts:
  * 		Start by enabeling the dessired interrupt,
  * 		set the edge mode as one of the four predefined, but keep in mind
  * 		that special cases may apply to some interrupts.
  * 		set the function to execute when the interrupt is triggered.
  * 		run 'enable_interrupts();' to enable the global interrupts flag,
  * 		interrupts will not trigger if this is not set!
  *
  * 	Timer interrupts:
  * 		Enable dessired interrupts.
  * 		Set functions to execute when the interrupts are triggered.
  * 		Enable global interrupts flag by running 'enable_interrupts()'! Without this flag set, no interrupts will trigger.
  * 	
  */

#ifndef __AATG_INTERRUPTS__
#define __AATG_INTERRUPTS__

#include <avr/io.h>
#include <avr/interrupt.h>

// Edge modes
#define INTERRUPT_LOW   0 // Interrupt pin low gennerates interrupt
#define ANY_EDGE 		1 // Any logical change gennerates interrupt
#define RISING_EDGE 	2 // LOW to HIGH gennerates interrupt
#define FALLING_EDGE	3 // HIGH to LOW gennerates interrupt

// 'void funtion pointer' array indexes by interrupt priority 
#define INDEX_INT0			0  // External interrupt 0
#define INDEX_INT1			1  // External interrupt 1
#define INDEX_TIMER2_OCA 	2  // Timer2 Output Compare Match
#define INDEX_TIMER2_OCB 	3  // Timer2 Output Compare Match
#define INDEX_TIMER2_OF 	4  // Timer2 Overflow
#define INDEX_TIMER1_IC 	5  // Timer1 Input capture
#define INDEX_TIMER1_OCA	6  // Timer1 Output Compare match A
#define INDEX_TIMER1_OCB	7  // Timer1 Output Compare match B
#define INDEX_TIMER1_OF		8  // Timer1 Overflow
#define INDEX_TIMER0_OCA	9 // timer0 output compare match
#define INDEX_TIMER0_OCB	10 // timer0 output compare match
#define INDEX_TIMER0_OF		11  // Timer0 Overflow
#define INDEX_USART_RXC		12  // recieve complete usart
#define INDEX_USART_DRE		13 // usart data registry empty
#define INDEX_USART_TXC		14 // transmit complete usart
// #define INDEX_INT2		15 // External interrupt 1

typedef void (*pVoidFunc)(void); 			// void function pointer type definition


void disable_global_interrupts();				// Disables interrupts until an enabeling interrupt function is called (init | enable)
void enable_global_interrupts();				// Enables interrupts. Will stay enabled untill disable_interrupts(); is called									

void int0_init(char edge, pVoidFunc interrupt_function); // Initializes interrupt-0 (PIN D2)
void int1_init(char edge, pVoidFunc interrupt_function); // Initializes interrupt-1 (PIN D3)

void int0_enable(); 							// Enables external interrupt 0. OBS! Does not enable global interrupt flag! call enable_interrupts() for this.
void int1_enable();								// Enables external interrupt 1. OBS! Does not enable global interrupt flag! call enable_interrupts() for this.
void timer0_output_compareA_interrupt_enable(); 	// Enables timer0's Output Compare Match interrupt. 	OBS! Does not set interrupt function OBS! Does not enable global interrupt flag! call enable_interrupts() for this.
void timer0_output_compareB_interrupt_enable(); 	// Enables timer0's Output Compare Match interrupt. 	OBS! Does not set interrupt function OBS! Does not enable global interrupt flag! call enable_interrupts() for this.
void timer0_overflow_interrupt_enable();		// Enables timer0's clock Overflow interrupt. 			OBS! Does not set interrupt function OBS! Does not enable global interrupt flag! call enable_interrupts() for this.
void timer1_input_capture_interrupt_enable();	// Enables timer1's Input Capture interrupt. 			OBS! Does not set interrupt function OBS! Does not enable global interrupt flag! call enable_interrupts() for this.
void timer1_output_compareA_interrupt_enable();	// Enables timer1's 'A' Output Compare Match interrupt. OBS! Does not set interrupt function OBS! Does not enable global interrupt flag! call enable_interrupts() for this.
void timer1_output_compareB_interrupt_enable();	// Enables timer1's 'B' Output Compare Match interrupt. OBS! Does not set interrupt function OBS! Does not enable global interrupt flag! call enable_interrupts() for this.
void timer1_overflow_interrupt_enable();
void timer2_output_compareA_interrupt_enable();
void timer2_output_compareB_interrupt_enable();
void timer2_overflow_interrupt_enable();	
void usart_recieve_interrupt_enable();	
void usart_transmit_interrupt_enable();	
void usart_data_reg_empty_interrupt_enable();	

void int0_disable();							// Disables external interrupt 0 but remembers interrupt function adress and sense control setting
void int1_disable();							// Disables external interrupt 1 but remembers interrupt function adress and sense control setting
void timer0_output_compareA_interrupt_disable(); // Disables timer0's Output Compare Match interrupt
void timer0_output_compareB_interrupt_disable(); // Disables timer0's Output Compare Match interrupt
void timer0_overflow_interrupt_disable();		// Disables timer0's clock Overflow interrupt.
void timer1_input_capture_interrupt_disable();
void timer1_output_compareA_interrupt_disable();
void timer1_output_compareB_interrupt_disable();
void timer1_overflow_interrupt_disable();
void timer2_output_compareA_interrupt_disable();
void timer2_output_compareB_interrupt_disable();
void timer2_overflow_interrupt_disable();
void usart_recieve_interrupt_disable();
void usart_transmit_interrupt_disable();
void usart_data_reg_empty_disable();

void int0_set_edge(char edge);				// Sets edge of external interrupt 0
void int1_set_edge(char edge);				// Sets edge of external interrupt 1

void int0_set_function(pVoidFunc func);				// Sets function to call when interrupt is triggered by INT0
void int1_set_function(pVoidFunc func);				// Sets function to call when interrupt is triggered by INT1
void timer0_set_output_compareA_interrupt_function(pVoidFunc func); 	// Sets function to call when interrupt is triggered by output compare match
void timer0_set_output_compareB_interrupt_function(pVoidFunc func); 	// Sets function to call when interrupt is triggered by output compare match
void timer0_set_overflow_interrupt_function(pVoidFunc func); 		// Sets function to call when interrupt is triggered by clock overflow
void timer1_set_input_capture_interrupt_function(pVoidFunc func);
void timer1_set_output_compareA_interrupt_function(pVoidFunc func);
void timer1_set_output_compareB_interrupt_function(pVoidFunc func);
void timer1_set_overflow_interrupt_function(pVoidFunc func);
void timer2_set_output_compareA_interrupt_function(pVoidFunc func);
void timer2_set_output_compareB_interrupt_function(pVoidFunc func);
void timer2_set_overflow_interrupt_function(pVoidFunc func);
void usart_set_recieve_interrupt_function(pVoidFunc func);
void usart_set_transmit_interrupt_function(pVoidFunc func);
void usart_set_data_reg_empty_interrupt_function(pVoidFunc func);
//-------------------------------------------------------------------------------------------------//
	ISR(INT0_vect);						// FOR INTERNAL USE ONLY! 
	ISR(INT1_vect);						// FOR INTERNAL USE ONLY! 
	ISR(TIMER2_COMPA_vect);				// FOR INTERNAL USE ONLY! 
	ISR(TIMER2_COMPB_vect);				// FOR INTERNAL USE ONLY! 
	ISR(TIMER2_OVF_vect);				// FOR INTERNAL USE ONLY! 
	ISR(TIMER1_CAPT_vect);				// FOR INTERNAL USE ONLY! 
	ISR(TIMER1_COMPA_vect);				// FOR INTERNAL USE ONLY! 
	ISR(TIMER1_COMPB_vect);				// FOR INTERNAL USE ONLY! 
	ISR(TIMER1_OVF_vect);				// FOR INTERNAL USE ONLY! 
	ISR(TIMER0_COMPA_vect);				// FOR INTERNAL USE ONLY! 
	ISR(TIMER0_COMPB_vect);				// FOR INTERNAL USE ONLY! 
	ISR(TIMER0_OVF_vect);				// FOR INTERNAL USE ONLY! 
	ISR(USART_RXC_vect);				// FOR INTERNAL USE ONLY!
	ISR(USART_UDRE_vect);				// FOR INTERNAL USE ONLY!
	ISR(USART_TXC_vect);				// FOR INTERNAL USE ONLY!

// Abbreviations table
/////////////////////////////////////////////
// MCUCSR : MCU Control and Status Register
// MCUCR  : MCU Control Register
// GICR   : General Interrupt Control Register - Contains external interrupt enable flags (INT0, INT1)
// GIFR   : General Interrupt Flag Register - When interrupt uccurs, the corrosponding bit is set HIGH (INTF0, INTF1, INTF2)
// SREG   : Status register - Notably contains the Global Interrupt Enable flag on bit 7
// ICP    : Timer/Counter Input Capture Pin
// INT    : External Interrupt Source
// DDR    : Data Direction Register
// ISC    : Interrupt Sense Control
// MCU 	  : MicroController Unit
// OC  	  : Timer/Counter Output Compare Match Output
// T   	  : Timer/Counter Source


pVoidFunc INTERRUPT_CALLFUNCTION[15];

void disable_global_interrupts() {
	cli(); // Disables global interrupts
}
void enable_global_interrupts() {
	sei(); // Enables global interrupts
}


void int0_init(char edge, pVoidFunc interrupt_function) {
	int0_set_function(interrupt_function);
	int0_enable();
	int0_set_edge(edge);
	DDRD &= ~(1<<INT0); 			// set Data direction for pin D2 to input
	SREG  =   1<<7;					// Set Global Interrupt Enable bit
}
void int1_init(char edge, pVoidFunc interrupt_function) {
	int1_set_function(interrupt_function);
	int1_enable();
	int1_set_edge(edge);
	DDRD &= ~(1<<INT1); 			// set Data direction for pin D3 to input
	SREG  =   1<<7;					// Set Global Interrupt Enable bit
}


void int0_enable()  							{EIMSK  |=  1<<INT0;}
void int1_enable()  							{EIMSK  |=  1<<INT1;}
void timer0_output_compareA_interrupt_enable() 	{TIMSK0 |=  1<<OCIE0A;}
void timer0_output_compareB_interrupt_enable() 	{TIMSK0 |=  1<<OCIE0B;}
void timer0_overflow_interrupt_enable() 		{TIMSK0 |=  1<<TOIE0;}
void timer1_input_capture_interrupt_enable()	{TIMSK1 |=  1<<ICIE1;}
void timer1_output_compareA_interrupt_enable()	{TIMSK1 |=  1<<OCIE1A;}
void timer1_output_compareB_interrupt_enable()	{TIMSK1 |=  1<<OCIE1B;}
void timer1_overflow_interrupt_enable() 		{TIMSK1 |=  1<<TOIE1;}
void timer2_output_compareA_interrupt_enable() 	{TIMSK2 |=  1<<OCIE2A;}
void timer2_output_compareB_interrupt_enable() 	{TIMSK2 |=  1<<OCIE2B;}
void timer2_overflow_interrupt_enable() 		{TIMSK2 |=  1<<TOIE2;}
void usart_recieve_interrupt_enable()			{UCSR0B |=  1<<RXCIE0;}
void usart_transmit_interrupt_enable()			{UCSR0B |=  1<<TXCIE0;}
void usart_data_reg_empty_interrupt_enable()	{UCSR0B |=  1<<UDRIE0;}


void int0_disable() 							{EIMSK  &= ~1<<INT0;}
void int1_disable() 							{EIMSK  &= ~1<<INT1;}
void timer0_output_compareA_interrupt_disable() {TIMSK0 &= ~1<<OCIE0A;}
void timer0_output_compareB_interrupt_disable() {TIMSK0 &= ~1<<OCIE0B;}
void timer0_overflow_interrupt_disable()		{TIMSK0 &= ~1<<TOIE0;}
void timer1_input_capture_interrupt_disable()	{TIMSK1 &= ~1<<ICIE1;}
void timer1_output_compareA_interrupt_disable()	{TIMSK1 &= ~1<<OCIE1A;}
void timer1_output_compareB_interrupt_disable()	{TIMSK1 &= ~1<<OCIE1B;}
void timer1_overflow_interrupt_disable() 		{TIMSK1 &= ~1<<TOIE1;}
void timer2_output_compareA_interrupt_disable() {TIMSK2 &= ~1<<OCIE2A;}
void timer2_output_compareB_interrupt_disable() {TIMSK2 &= ~1<<OCIE2B;}
void timer2_overflow_interrupt_disable()		{TIMSK2 &= ~1<<TOIE2;}
void usart_recieve_interrupt_disable()			{UCSR0B &= ~1<<RXCIE0;}
void usart_transmit_interrupt_disable()			{UCSR0B &= ~1<<TXCIE0;}
void usart_data_reg_empty_interrupt_disable()	{UCSR0B &= ~1<<UDRIE0;}


void int0_set_edge(char edge) {
	switch(edge) {					// Set EICRA
		case INTERRUPT_LOW:			// set to interrupt if pin is LOW
			EICRA &= ~(1<<ISC01 | 1<<ISC00);
			break;
		case ANY_EDGE:				// Set to interrupt on any logical change
			EICRA &= ~(1<<ISC01);
			EICRA |=   1<<ISC00;
			break;
		case RISING_EDGE:			// Set to interrupt on rising edge
			EICRA |=   1<<ISC01;
			EICRA |=   1<<ISC00;
			break;
		case FALLING_EDGE:			// Set to interrupt on falling edge
			EICRA |=   1<<ISC01;
			EICRA &= ~(1<<ISC00); 
			break;
	}
}
void int1_set_edge(char edge) {
	switch(edge) {					// Set EICRA
		case INTERRUPT_LOW:			// set to interrupt if pin is LOW
			EICRA &= ~(1<<ISC11 | 1<<ISC10);
			break;
		case ANY_EDGE:				// Set to interrupt on any logical change
			EICRA &= ~(1<<ISC11);
			EICRA |=   1<<ISC10;
			break;
		case RISING_EDGE:			// Set to interrupt on rising edge
			EICRA |=   1<<ISC11 | 1<<ISC10;
			break;
		case FALLING_EDGE:			// Set to interrupt on falling edge
			EICRA |=   1<<ISC11;
			EICRA &= ~(1<<ISC10); 
			break;
	}
}


void int0_set_function(pVoidFunc func) {
	INTERRUPT_CALLFUNCTION[INDEX_INT0] = func; // assign void function to call on interrupt
}
void int1_set_function(pVoidFunc func) {
	INTERRUPT_CALLFUNCTION[INDEX_INT1] = func; // assign void function to call on interrupt
}
void timer0_set_output_compareA_interrupt_function(pVoidFunc func) {
	INTERRUPT_CALLFUNCTION[INDEX_TIMER0_OCA] = func; // assign void function to call on interrupt
}
void timer0_set_output_compareB_interrupt_function(pVoidFunc func) {
	INTERRUPT_CALLFUNCTION[INDEX_TIMER0_OCB] = func; // assign void function to call on interrupt
}
void timer0_set_overflow_interrupt_function(pVoidFunc func) {
	INTERRUPT_CALLFUNCTION[INDEX_TIMER0_OF] = func; // assign void function to call on interrupt
}
void timer1_set_input_capture_interrupt_function(pVoidFunc func) {
	INTERRUPT_CALLFUNCTION[INDEX_TIMER1_IC] = func; // assign void function to call on interrupt
}
void timer1_set_output_compareA_interrupt_function(pVoidFunc func) {
	INTERRUPT_CALLFUNCTION[INDEX_TIMER1_OCA] = func; // assign void function to call on interrupt
}
void timer1_set_output_compareB_interrupt_function(pVoidFunc func) {
	INTERRUPT_CALLFUNCTION[INDEX_TIMER1_OCB] = func; // assign void function to call on interrupt
}
void timer1_set_overflow_interrupt_function(pVoidFunc func) {
	INTERRUPT_CALLFUNCTION[INDEX_TIMER1_OF] = func; // assign void function to call on interrupt
}
void timer2_set_output_compareA_interrupt_function(pVoidFunc func) {
	INTERRUPT_CALLFUNCTION[INDEX_TIMER2_OCA] = func; // assign void function to call on interrupt
}
void timer2_set_output_compareB_interrupt_function(pVoidFunc func) {
	INTERRUPT_CALLFUNCTION[INDEX_TIMER2_OCB] = func; // assign void function to call on interrupt
}
void timer2_set_overflow_interrupt_function(pVoidFunc func) {
	INTERRUPT_CALLFUNCTION[INDEX_TIMER2_OF] = func; // assign void function to call on interrupt
}
void usart_set_recieve_interrupt_function(pVoidFunc func) {
	INTERRUPT_CALLFUNCTION[INDEX_USART_RXC] = func; // assign void function to call on interrupt
}
void usart_set_transmit_interrupt_function(pVoidFunc func) {
	INTERRUPT_CALLFUNCTION[INDEX_USART_TXC] = func; // assign void function to call on interrupt
}
void usart_set_data_reg_empty_interrupt_function(pVoidFunc func) {
	INTERRUPT_CALLFUNCTION[INDEX_USART_DRE] = func; // assign void function to call on interrupt
}


// The following are ordered by interrupt priority
ISR(INT0_vect){				INTERRUPT_CALLFUNCTION[INDEX_INT0](); }
ISR(INT1_vect){				INTERRUPT_CALLFUNCTION[INDEX_INT1](); }
ISR(TIMER2_COMPA_vect) {	INTERRUPT_CALLFUNCTION[INDEX_TIMER2_OCA](); }
ISR(TIMER2_COMPB_vect) {	INTERRUPT_CALLFUNCTION[INDEX_TIMER2_OCB](); }
ISR(TIMER2_OVF_vect) {		INTERRUPT_CALLFUNCTION[INDEX_TIMER2_OF](); }
ISR(TIMER1_CAPT_vect) {		INTERRUPT_CALLFUNCTION[INDEX_TIMER1_IC](); }
ISR(TIMER1_COMPA_vect) {	INTERRUPT_CALLFUNCTION[INDEX_TIMER1_OCA](); }
ISR(TIMER1_COMPB_vect) {	INTERRUPT_CALLFUNCTION[INDEX_TIMER1_OCB](); }
ISR(TIMER1_OVF_vect) {		INTERRUPT_CALLFUNCTION[INDEX_TIMER1_OF](); }
ISR(TIMER0_COMPA_vect){		INTERRUPT_CALLFUNCTION[INDEX_TIMER0_OCA](); }
ISR(TIMER0_COMPB_vect){		INTERRUPT_CALLFUNCTION[INDEX_TIMER0_OCB](); }
ISR(TIMER0_OVF_vect) {		INTERRUPT_CALLFUNCTION[INDEX_TIMER0_OF](); }
ISR(USART_RX_vect) {		INTERRUPT_CALLFUNCTION[INDEX_USART_RXC](); }
ISR(USART_UDRE_vect) {		INTERRUPT_CALLFUNCTION[INDEX_USART_DRE](); }
ISR(USART_TX_vect) {		INTERRUPT_CALLFUNCTION[INDEX_USART_TXC](); }


#endif