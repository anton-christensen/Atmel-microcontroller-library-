#ifndef __AATG_ESSENTIALS__
#define __AATG_ESSENTIALS__

#include <avr/io.h>
#include <util/delay.h>

int  norm(int n);						// returns input normalized
void delay_ms(unsigned long long n); 	// same as _delay_ms but accepts non constant input
void delay_us(unsigned long long n); 	// same as _delay_us but accepts non constant input

// normalize
int norm(int n) { return n < 0 ? -n : n; }

// variable delay function
void delay_ms(unsigned long long n) {
	while(n--)
		_delay_ms(1);
}

// variable delay function
void delay_us(unsigned long long n) {
	while(n--)
		_delay_us(1);
}

#endif