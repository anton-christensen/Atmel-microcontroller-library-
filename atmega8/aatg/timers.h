 /** 
  * File:   timers.h 
  * 
  * Author: Anton Christensen (anton.christensen9700@gmail.com)
  * Date:   January 2014 
  * 
  * Description: 
  * 	Aims to ease the use of setting up and usage of the three timers on the ATMega16
  *
  * Usage:
  * 	choose a timer to use.
  * 	set its WGM (Waveform Generation mode),
  * 			OC (Output Compare) mode and
  * 			CS (Clock Select) mode.
  *    	use the predefined modes below, 
  *    	check the comments to ensure that the mode is compatible with the timer mode you intend to use
  *    	if relevant, set the output compare register using the below defined functions.
  * 
  */

#ifndef __AATG_TIMERS__
#define __AATG_TIMERS__

#include <avr/io.h>

// Abbreviations table
/////////////////////////////////////////////
// COM   : Compare Output Match (mode)
// CS    : Clock Select
// CTC   : Clear Timer on Compare (WGM mode)
// FOC   : Force Output Compare (bit)
// ICNC  : Input Capture Noise Canceler
// ICR   : Input Capture Register
// ICSE  : Input Capture Edge Select
// OC    : Output Compare (pin)
// OCF   : Output Compare Flag / Interrupt Request
// OCIE  : Output Compare Match Interrupt Enable
// OCR   : Output Compare Register
// PWM   : Pulse Width Modulation (WGM mode)
// SFIOR : Special Function IO Register (register)
// T     : Clock Source (pin)
// TCCR  : Timer Counter Control Register
// TCNT  : Timer/Counter Counter Value 
// TIFR  : Timer/Counter Interrupt Flag Register - When an interrupt is gennerated it is stored here
// TIMSK : Timer Interrupt Mask Register - used to enable triggering of interrupts when a OCF is HIGH (OCIE0)
// TOSC  : Timer Oscillator (pin)
// TOV   : Timer/Counter OVerflow
// WGM   : Waveform Generation Mode (mode)

// Wave generating modes (For 8 bit timers)
#define NORMAL_MODE 		0 	// Counts from 0x00 to 0xFF (0-255)
#define PWM_PHASE_CORRECT 	1 	// Counts from 0x00 to 0xFF to 0 (0-255-0)
#define CLEAR_ON_COMPARE 	2 	// Counts from 0x00 to OCR value
#define PWM_FAST 			3 	// Counts from 0x00 to 0xFF (0-255)

// Wave generating modes (For 16 bit timers) see page 112 of ATMega16 Datasheet - Table 47
#define NORMAL16_MODE				0 	// Goes from 0x0000 to 0xFFFF (0-65535)
#define PWM_PHASE_CORRECT16_8BIT	1 	// Goes from 0x0000 to 0x00FF to 0x0000 (0-255-0)
#define PWM_PHASE_CORRECT16_9BIT	2 	// Goes from 0x0000 to 0x01FF to 0x0000 (0-511-0)
#define PWM_PHASE_CORRECT16_10BIT	3 	// Goes from 0x0000 to 0x03FF to 0x0000 (0-1023-0)
#define CLEAR_ON_COMPARE16_OC		4 	// Goes from 0x0000 to OCRA value
#define PWM_FAST16_8BIT				5 	// Goes from 0x0000 to 0x00FF (0-255)
#define PWM_FAST16_9BIT				6 	// Goes from 0x0000 to 0x01FF (0-511)
#define PWM_FAST16_10BIT			7 	// Goes from 0x0000 to 0x03FF (0-1023)
#define PWM_PHASE_FREQ_CORRECT16_IC	8 	// Goes from 0x0000 to ICR value to 0x0000 - Always finishes a period before updating ICR 
#define PWM_PHASE_FREQ_CORRECT16_OC	9 	// Goes from 0x0000 to OCRA value to 0x0000 - Always finishes a period before updating OCR
#define PWM_PHASE_CORRECT16_IC 		10 	// Goes from 0x0000 to ICR value to 0x0000
#define PWM_PHASE_CORRECT16_OC 		11 	// Goes from 0x0000 to OCRA value to 0x0000
#define CLEAR_ON_COMPARE16_IC 		12  // Goes from 0x0000 to ICR value
//#define RESERVED					13  // NOT IN USE
#define PWM_FAST_INPUT_CAPTURE		14 	// Goes from 0x0000 to ICR value  OBS! - min value of ICR is 0x0003 here
#define PWM_FAST_OUTPUT_COMPARE		15 	// Goes from 0x0000 to OCRA value OBS! - min value of OCRA is 0x0003 here

// Output compare modes (For 8 bit timers)
#define NON_PWM_NORMAL			0   // OC disconnected (normal pin operation)
#define NON_PWM_OC_TOGGLE		1   // Toggles OC pin on Compare match
#define NON_PWM_OC_CLEAR		2   // Sets OC pin LOW on Compare match
#define NON_PWM_OC_SET			3   // Sets OC pin HIGH on Compare match
#define PWM_FAST_NORMAL			4   // OC disconnected (normal pin operation)
//#define RESERVED				5	// NOT IN USE
#define PWM_FAST_NON_INVERT		6 	// Clears OC pin on compare match and sets OC pin when counter = 0
#define PWM_FAST_INVERT			7	// Sets OC pin on compare match and clears OC pin when counter = 0
#define PWM_PHASE_NORMAL		8 	// OC disconnected (normal pin operation)
//#define RESERVED				9	// NOT IN USE
#define PWM_PHASE_OC_CLEAR_UP	10 	// Clears OC pin on Compare match and counting up ... sets OC on Compare match and counting down
#define PWM_PHASE_OC_CLEAR_DOWN	11	// Clears OC pin on Compare match and counting down ... sets OC on Compare match and counting up

// Output compare modes (For 16 bit Timer/Counter) see page 110-111 of ATMega16 Datasheet
#define NON_PWM16_NORMAL			0   // OCA/OCB disconnected  (normal pin operation)
#define NON_PWM16_OC_TOGGLE			1   // Toggles OCA/OCB pin on Compare match
#define NON_PWM16_OC_CLEAR			2   // Sets OCA/OCB pin LOW on Compare match
#define NON_PWM16_OC_SET			3   // Sets OCA/OCB pin HIGH on Compare match
#define PWM_FAST16_NORMAL			4   // OCA/OCB disconnected (normal pin operation)
#define PWM_FAST16_OCA_TOGGLE		5	// Toggles OCA pin on Compare match, OCB is disconnected - OBS! WORKS ONLY FOR PWM_FAST_OUTPUT_COMPARE WGM MODE!
#define PWM_FAST16_NON_INVERT		6 	// Clears OCA/OCB on compare match and sets OCA/OCB when counter = 0
#define PWM_FAST16_INVERT			7	// Sets OCA/OCB on compare match and clears OCA/OCB when counter = 0
#define PWM_PHASE16_NORMAL			8 	// OCA/OCB disconnected (normal pin operation)
#define PWM_PHASE16_TOGGLE_OCA		9	// Toggles OCA pin on Compare match, OCB is disconnected - OBS! WORKS ONLY FOR PWM_FAST_INPUT_CAPTURE AND PWM_PHASE_FREQ_CORRECT_OC WGM MODES!
#define PWM_PHASE16_OC_CLEAR_UP		10 	// Clears OCA/OCB pin on Compare match and counting up ... sets OCA/OCB on Compare match and counting down
#define PWM_PHASE16_OC_CLEAR_DOWN	11	// Clears OCA/OCB pin on Compare match and counting down ... sets OCA/OCB on Compare match and counting up

// Prescaler modes / Clock select modes - Applies to timer0 and timer1
#define CLOCK_STOP					0 	// No clock source - timer/counter not counting
#define CLOCK_PRESCALER_1			1 	// Normal operation - no prescaler, +1 to counter for each system clock tick
#define CLOCK_PRESCALER_8			2 	// prescaler 8, +1 to counter for every 8 system clock ticks
#define CLOCK_PRESCALER_64			3
#define CLOCK_PRESCALER_256			4
#define CLOCK_PRESCALER_1024		5
#define CLOCK_EXTERNAL_FALLING_EDGE	6
#define CLOCK_EXTERNAL_RISING_EDGE	7

// Prescaler modes / Clock select modes - Applies to timer2 only
#define CLOCK2_STOP					0 	// No clock source - timer/counter not counting
#define CLOCK2_PRESCALER_1			1 	// Normal operation - no prescaler, +1 to counter for each system clock tick
#define CLOCK2_PRESCALER_8			2 	// prescaler 8, +1 to counter for every 8 system clock ticks
#define CLOCK2_PRESCALER_32			3
#define CLOCK2_PRESCALER_64			4
#define CLOCK2_PRESCALER_128		5
#define CLOCK2_PRESCALER_256		6
#define CLOCK2_PRESCALER_1024		7

// timer2 asynchronous modes
#define ASYNC_MODE_INTERNAL_OSCILLATOR 0 	// Default setting, use system oscillator
#define ASYNC_MODE_EXTERNAL_OSCILLATOR 1 	// Asynchronous setting, use oscillator connected to the TOSC1 pin

// Duty cycle calculators
unsigned int duty_cycle(int percentage, unsigned int max); 	// maps 0-100 to 0x00-max
unsigned int duty_cycle_8bit(int percentage); 	// maps 0-100 to 0x00-0xFF
unsigned int duty_cycle_9bit(int percentage); 	// maps 0-100 to 0x00-0x1FF
unsigned int duty_cycle_10bit(int percentage); 	// maps 0-100 to 0x00-0x3FF
unsigned int duty_cycle_16bit(int percentage); 	// maps 0-100 to 0x00-0xFFFF

// shorthand initializers
void timer0_init(int wave_mode, int oc_mode, int clock_mode);
void timer1_init(int wave16_mode, int ocA16_mode, int ocB16_mode, int clock_mode);
void timer2_init(int wave_mode, int oc_mode, int clock_mode);

// waveform generator mode setters
void timer0_set_wave_mode(int wave_mode);
void timer1_set_wave_mode(int wave16_mode);
void timer2_set_wave_mode(int wave_mode);

// output compare mode setters
void timer0_set_output_compare_mode(int oc_mode);
void timer1_set_output_compareA_mode(int ocA16_mode);
void timer1_set_output_compareB_mode(int ocB16_mode);
void timer2_set_output_compare_mode(int oc_mode);

// clock/counter/prescaler mode setters
void timer0_set_clock_mode(int clock_mode);
void timer1_set_clock_mode(int clock_mode);
void timer2_set_clock_mode(int clock_mode);

// output compare register value setters
void timer0_set_output_compare_register(unsigned int oc_value);
void timer1_set_output_compare_registerA(unsigned int oc_value);
void timer1_set_output_compare_registerB(unsigned int oc_value);
void timer2_set_output_compare_register(int oc_value);

// force output compare functions
void timer0_force_output_compare();  // Forces a compare match state, where waveform and OC will update. read more in ATMega16 Datasheet page 83. DO NOT USE IN PWM MODES!
void timer1A_force_output_compare(); // Forces a compare match state, where waveform and OC will update. read more in ATMega16 Datasheet page 83. DO NOT USE IN PWM MODES!
void timer1B_force_output_compare(); // Forces a compare match state, where waveform and OC will update. read more in ATMega16 Datasheet page 83. DO NOT USE IN PWM MODES!
void timer2_force_output_compare();  // Forces a compare match state, where waveform and OC will update. read more in ATMega16 Datasheet page 83. DO NOT USE IN PWM MODES!

// counter getters
int 		 timer0_get_counter();	// gets value of timer/counter0
unsigned int timer1_get_counter(); 	// gets value of timer/counter1 (16 bit value)
int 		 timer2_get_counter(); 	// gets value of timer/counter2

// input capture setter and getter
unsigned int timer1_get_input_capture(); 	// gets IC (input capture) value of timer 1 (16 bit value)
void 		 timer1_set_input_capture(unsigned int val); // sets IC (input capture) value of timer 1 (16 bit value)

// counter setters
void timer0_set_counter(int val); 			// Set timer/counter0 value OBS! introduces risk of skipping a single compare
void timer1_set_counter(unsigned int val);	// Set timer/counter1 value (16 bit value) OBS! introduces risk of skipping a single compare
void timer2_set_counter(int val);			// Set timer/counter2 value OBS! introduces risk of skipping a single compare

// prescaler resetter
void timer01_reset_prescaler();		// Synchronizes timer 1 and 0 prescalers by ressetting it
void timer2_reset_prescaler();		// resets the prescaler value of timer/counter2

// async mode/clock source select setter
void timer2_set_async_mode(char async_mode);	// Set clock source of timer/counter2 (system clock or external oscillator)



//---------------------------------------------------------------------//

#define OC0  3
#define OC1A 5
#define OC1B 4
#define OC2  7
#define T0   0
#define T1   1
#define ICP1 6

unsigned int duty_cycle(int percentage, unsigned int max)  { return (max*(unsigned long)percentage)/100; }
unsigned int duty_cycle_8bit(int percentage)  { return (0xFF  *percentage)/100; }
unsigned int duty_cycle_9bit(int percentage)  { return (0x1FF *(long)percentage)/100; }
unsigned int duty_cycle_10bit(int percentage) { return (0x3FF *(long)percentage)/100; }
unsigned int duty_cycle_16bit(int percentage) { return (0xFFFF*(long)percentage)/100; }

void timer0_init(int wave_mode, int oc_mode, int clock_mode) {
	timer0_set_wave_mode(wave_mode);
	timer0_set_output_compare_mode(oc_mode);
	timer0_set_clock_mode(clock_mode);
}
void timer1_init(int wave16_mode, int ocA16_mode, int ocB16_mode, int clock_mode) {
	timer1_set_wave_mode(wave16_mode);
	timer1_set_output_compareA_mode(ocA16_mode);
	timer1_set_output_compareB_mode(ocB16_mode);
	timer1_set_clock_mode(clock_mode);
}
void timer2_init(int wave_mode, int oc_mode, int clock_mode) {
	timer2_set_wave_mode(wave_mode);
	timer2_set_output_compare_mode(oc_mode);
	timer2_set_clock_mode(clock_mode);
}


void timer0_set_wave_mode(int wave_mode) {
	switch(wave_mode) {
		case NORMAL_MODE:
			TCCR0 &= ~(1<<WGM01 | 1<<WGM00);
			break;
		case PWM_PHASE_CORRECT:
			TCCR0 &= ~(1<<WGM01);
			TCCR0 |=   1<<WGM00;
			break;
		case CLEAR_ON_COMPARE:
			TCCR0 |=   1<<WGM01;
			TCCR0 &= ~(1<<WGM00);
			break;
		case PWM_FAST:
			TCCR0 |=   1<<WGM01 | 1<<WGM00;
			break;
	}
}
void timer1_set_wave_mode(int wave16_mode) {
	switch(wave16_mode) {
		case NORMAL16_MODE: 				TCCR1B &= ~(1<<WGM13 | 1<<WGM12); TCCR1A &= ~(1<<WGM11 | 1<<WGM10); break; 
		case PWM_PHASE_CORRECT16_8BIT: 		TCCR1B &= ~(1<<WGM13 | 1<<WGM12); TCCR1A &= ~(1<<WGM11); TCCR1A |= 1<<WGM10; break; 
		case PWM_PHASE_CORRECT16_9BIT: 		TCCR1B &= ~(1<<WGM13 | 1<<WGM12); TCCR1A |= 1<<WGM11; TCCR1A &= ~(1<<WGM10); break; 
		case PWM_PHASE_CORRECT16_10BIT: 	TCCR1B &= ~(1<<WGM13 | 1<<WGM12); TCCR1A |= 1<<WGM11 | 1<<WGM10; break;
		case CLEAR_ON_COMPARE16_OC:			TCCR1B &= ~(1<<WGM13); TCCR1B |= 1<<WGM12; TCCR1A &= ~(1<<WGM11 | 1<<WGM10); break;
		case PWM_FAST16_8BIT:				TCCR1B &= ~(1<<WGM13); TCCR1B |= 1<<WGM12; TCCR1A &= ~(1<<WGM11); TCCR1A |= 1<<WGM10; break; 
		case PWM_FAST16_9BIT:				TCCR1B &= ~(1<<WGM13); TCCR1B |= 1<<WGM12; TCCR1A |= 1<<WGM11; TCCR1A &= ~(1<<WGM10); break; 
		case PWM_FAST16_10BIT:				TCCR1B &= ~(1<<WGM13); TCCR1B |= 1<<WGM12; TCCR1A |= 1<<WGM11 | 1<<WGM10; break;
		case PWM_PHASE_FREQ_CORRECT16_IC: 	TCCR1B |= 1<<WGM13; TCCR1B &= ~(1<<WGM12); TCCR1A &= ~(1<<WGM11 | 1<<WGM10); break;  
		case PWM_PHASE_FREQ_CORRECT16_OC: 	TCCR1B |= 1<<WGM13; TCCR1B &= ~(1<<WGM12); TCCR1A &= ~(1<<WGM11); TCCR1A |= 1<<WGM10; break; 
		case PWM_PHASE_CORRECT16_IC: 		TCCR1B |= 1<<WGM13; TCCR1B &= ~(1<<WGM12); TCCR1A |= 1<<WGM11; TCCR1A &= ~(1<<WGM10); break; 
		case PWM_PHASE_CORRECT16_OC: 		TCCR1B |= 1<<WGM13; TCCR1B &= ~(1<<WGM12); TCCR1A |= 1<<WGM11 | 1<<WGM10; break;
		case CLEAR_ON_COMPARE16_IC:			TCCR1B |= 1<<WGM13 | WGM12; TCCR1A &= ~(1<<WGM11 | 1<<WGM10); break; 
		//case RESERVED:					TCCR1B |= 1<<WGM13 | WGM12; TCCR1A &= ~(1<<WGM11); TCCR1A |= 1<<WGM10; break; 
		case PWM_FAST_INPUT_CAPTURE:		TCCR1B |= 1<<WGM13 | WGM12; TCCR1A |= 1<<WGM11; TCCR1A &= ~(1<<WGM10); break; 
		case PWM_FAST_OUTPUT_COMPARE:		TCCR1B |= 1<<WGM13 | WGM12; TCCR1A |= 1<<WGM11 | 1<<WGM10; break;
	}
}
void timer2_set_wave_mode(int wave_mode) {
	switch(wave_mode) {
		case NORMAL_MODE:
			TCCR2 &= ~(1<<WGM21 | 1<<WGM20);
			break;
		case PWM_PHASE_CORRECT:
			TCCR2 &= ~(1<<WGM21);
			TCCR2 |=   1<<WGM20;
			break;
		case CLEAR_ON_COMPARE:
			TCCR2 |=   1<<WGM21;
			TCCR2 &= ~(1<<WGM20);
			break;
		case PWM_FAST:
			TCCR2 |=   1<<WGM21 | 1<<WGM20;
			break;
	}
}


void timer0_set_output_compare_mode(int oc_mode) {
	switch(oc_mode%4) {
		case NON_PWM_NORMAL:
			TCCR0 &= ~(1<<COM01 | 1<<COM00);
			break;
		case NON_PWM_OC_TOGGLE:
			TCCR0 &= ~(1<<COM01);
			TCCR0 |=   1<<COM00;
			break;
		case NON_PWM_OC_CLEAR:
			TCCR0 |=   1<<COM01;
			TCCR0 &= ~(1<<COM00);
			break;
		case NON_PWM_OC_SET:
			TCCR0 |=   1<<COM01 | 1<<COM00;
			break;
	}
}
void timer1_set_output_compareA_mode(int ocA16_mode) {
	switch(ocA16_mode%4) {
		case NON_PWM16_NORMAL: 		TCCR1A &= ~(1<<COM1A1 | 1<<COM1A0); break;
		case NON_PWM16_OC_TOGGLE:	TCCR1A &= ~(1<<COM1A1); TCCR1A |=   1<<COM1A0;  break;
		case NON_PWM16_OC_CLEAR: 	TCCR1A |=   1<<COM1A1;  TCCR1A &= ~(1<<COM1A0); break;
		case NON_PWM16_OC_SET: 		TCCR1A |=   1<<COM1A1 | 1<<COM1A0;  break;
	}
}
void timer1_set_output_compareB_mode(int ocB16_mode) {
	switch(ocB16_mode%4) {
		case NON_PWM16_NORMAL: 		TCCR1A &= ~(1<<COM1B1 | 1<<COM1B0); break;
		case NON_PWM16_OC_TOGGLE: 	TCCR1A &= ~(1<<COM1B1); TCCR1A |=   1<<COM1B0;  break;
		case NON_PWM16_OC_CLEAR: 	TCCR1A |=   1<<COM1B1;  TCCR1A &= ~(1<<COM1B0); break;
		case NON_PWM16_OC_SET: 		TCCR1A |=   1<<COM1B1 | 1<<COM1B0;  break;
	}
}
void timer2_set_output_compare_mode(int oc_mode) {
	switch(oc_mode%4) {
		case NON_PWM_NORMAL:
			TCCR2 &= ~(1<<COM21 | 1<<COM20);
			break;
		case NON_PWM_OC_TOGGLE:
			TCCR2 &= ~(1<<COM21);
			TCCR2 |=   1<<COM20;
			break;
		case NON_PWM_OC_CLEAR:
			TCCR2 |=   1<<COM21;
			TCCR2 &= ~(1<<COM20);
			break;
		case NON_PWM_OC_SET:
			TCCR2 |=   1<<COM21 | 1<<COM20;
			break;
	}
}


void timer0_set_clock_mode(int clock_mode) {
	if(clock_mode >= 0 && clock_mode <= 7) {
		TCCR0 &= ~(0b111);
		TCCR0 |= clock_mode; 
	}
}
void timer1_set_clock_mode(int clock_mode) {
	if(clock_mode >= 0 && clock_mode <= 7) {
		TCCR1B &= ~(0b111);
		TCCR1B |= clock_mode; 
	}
}
void timer2_set_clock_mode(int clock_mode) {
	if(clock_mode >= 0 && clock_mode <= 7) {
		TCCR2 &= ~(0b00000111);
		TCCR2 |= clock_mode; 
	}
}


void timer0_set_output_compare_register(unsigned int oc_value) {OCR0 = oc_value;}
void timer1_set_output_compare_registerA(unsigned int oc_value) {
	// turn off interrupts while doing 16 bit read
	unsigned int sreg;
	sreg = SREG; //Save global interrypt flag
	SREG &= ~1<<7; // disable global interrupt flag
	OCR1A = oc_value;
	SREG = sreg; // restore global interrupt flag state
}
void timer1_set_output_compare_registerB(unsigned int oc_value) {
	// turn off interrupts while doing 16 bit read
	unsigned int sreg;
	sreg = SREG; //Save global interrypt flag
	SREG &= ~1<<7; // disable global interrupt flag
	OCR1B = oc_value;
	SREG = sreg; // restore global interrupt flag state
}
void timer2_set_output_compare_register(int oc_value) {OCR2 = oc_value;}


void timer0_force_output_compare() { TCCR0 |= 1<<FOC0; }
void timer1A_force_output_compare() { TCCR1A |= 1<<FOC1A; }
void timer1B_force_output_compare() { TCCR1A |= 1<<FOC1B; }
void timer2_force_output_compare() { TCCR2 |= 1<<FOC2; }


int timer0_get_counter() {return TCNT0;}
unsigned int timer1_get_counter() { 
	// turn off interrupts while doing 16 bit read
	unsigned int sreg;
	unsigned int i;
	sreg = SREG; //Save global interrypt flag
	SREG &= ~1<<7; // disable global interrupt flag
	i = TCNT1;
	SREG = sreg; // restore global interrupt flag state
	return i;
}
int timer2_get_counter() {return TCNT2;}


unsigned int timer1_get_input_capture() {
	// turn off interrupts while doing 16 bit read
	unsigned int sreg;
	unsigned int i;
	sreg = SREG; //Save global interrypt flag
	SREG &= ~1<<7; // disable global interrupt flag
	i = ICR1;
	SREG = sreg; // restore global interrupt flag state
	return i;
}
void timer1_set_input_capture(unsigned int val) {
	// turn off interrupts while doing 16 bit write
	unsigned int sreg;
	sreg = SREG; //Save global interrypt flag
	SREG &= ~1<<7; // disable global interrupt flag
	ICR1 = val;
	SREG = sreg; // restore global interrupt flag state
}


void timer0_set_counter(int val) {TCNT0 = val;}
void timer1_set_counter(unsigned int val) { 
	// turn off interrupts while doing 16 bit write
	unsigned int sreg;
	sreg = SREG; //Save global interrypt flag
	SREG &= ~1<<7; // disable global interrupt flag
	TCNT1 = val;
	SREG = sreg; // restore global interrupt flag state
}
void timer2_set_counter(int val) {TCNT2 = val;}


void timer01_reset_prescaler() {SFIOR |= 1<<PSR10;}
void timer2_reset_prescaler()  {SFIOR |= 1<<PSR2;}

void timer2_set_async_mode(char async_mode) {
	if(async_mode == ASYNC_MODE_INTERNAL_OSCILLATOR)
		ASSR &= ~1<<AS2;
	else if(async_mode == ASYNC_MODE_EXTERNAL_OSCILLATOR)
		ASSR |=  1<<AS2;
}

#endif
