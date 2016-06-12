// Standard libs
#include <avr/io.h>
#include <util/delay.h>

#include "../aatg/essentials.h"
#include "../aatg/interrupts.h"
#include "../aatg/serial.h"
#include "../aatg/adc.h"
#include "../aatg/timers.h"

#define INACTIVE_LOOPS 4
#define Ts 2
#define Ss 3

#define RGBR 7
#define RGBG 6
#define RGBB 5

// #define SERVO_MIN 758 //ŕelease
#define SERVO_MIN 648 //development
// #define SERVO_MAX 2478 //release
#define SERVO_MAX 2650 //development
#define SERVO_WIDTH 40000


//
// pin configurations
// 
// R D7
// G D6
// B D5
// T1 C0
// T2 C1
// D1 C2
// POWER Level C3 - TOP = 9.2V
// S1 B1
// S2 B2
// S3 B2

//
// thermometer calibration values
//
// 0 degrees = 1.5625 V = 463 readVal
// 700 degrees = 2.56 V = 1023 readVal
// 1 readVal = 1.14 degrees
// 1 degree = 0.8772 readVal
// 2.56 = 1023


//
// servo motor calibration values
//
// HEXTRONIK HXT900 9gr servo
// from 850 - 2700 (40000)
// 200 degrees

// Hitec HS-300
// From 758 - 2478
// 200 degrees

// buffer used for bluetooth input
char inputBuffer[256];
// index indicating next available spot in inputBuffer
int bufferIndex = 0;
// keeps track of if the connection is lost
int inactiveLoops = 0;
// Counter which increments on loop run through
int mainLoops = 0;
// temperature storage
int T[Ts];
int S[Ss];
int P = 0;

//int readTemp(int pin) {return (adc_read(pin)-624)*114/100;}
int readTemp(int pin) {return (adc_read(pin));}

void catchRX() {
	inactiveLoops = INACTIVE_LOOPS;
	char c;
	c = getchar();
	if(c == ' ') { // end of command
		if(bufferIndex < 2) {
			bufferIndex = 0;
			return;
		}
		inputBuffer[bufferIndex] = '\0';

		// parse input buffer
		// get function and index
		int i = inputBuffer[1]-48; // ascii single digit charecter to int conversion
		char f = inputBuffer[0];
		int val;
		switch(f) {
			case 'S':
				sscanf(inputBuffer+3, "%d", &val);
				if(i >= 0 && i < Ss)
					S[i] = val;
				if(inactiveLoops >= 0) {
					if(i == 1 && val > -1 && val <= 100) {
						timer1_set_output_compare_registerA(linInterp(val,SERVO_MIN,SERVO_MAX));
					}
					else if(i == 2 && val > -1 && val <= 100 ) {
						timer1_set_output_compare_registerB(linInterp(val,SERVO_MIN,SERVO_MAX));
					}
				}

				break;
		}
	
		bufferIndex = 0;
	}
	else {
		inputBuffer[bufferIndex] = c;
		bufferIndex++;
		bufferIndex %= 256;
	}
}

int main() {
	usart_init();
	adc_enable();
	adc_set_ref(ADC_REF_2_56V);

	usart_set_recieve_interrupt_function(catchRX);
	usart_recieve_interrupt_enable();
	/*
	timer0_set_clock_mode(CLOCK_PRESCALER_1024);
	timer0_set_overflow_interrupt_function(blink);
	timer0_overflow_interrupt_enable();
	*/
	timer1_init(PWM_FAST_INPUT_CAPTURE, PWM_FAST16_NON_INVERT, PWM_FAST16_NON_INVERT, CLOCK_PRESCALER_8);
	timer1_set_input_capture(SERVO_WIDTH);
	timer1_set_output_compare_registerA(linInterp(50,SERVO_MIN,SERVO_MAX));
	/*	
	timer2_init(PWM_PHASE_CORRECT, PWM_PHASE_NORMAL, CLOCK2_PRESCALER_1024);
	timer2_overflow_interrupt_enable();
	timer2_set_overflow_interrupt_function(flipPWM);
	*/
	enable_global_interrupts();

	DDRB = 0b00111111;
	DDRD = 7<<5; // rgb

	while(1 == 1) {
		mainLoops++;
		P = adc_read(3);
		if(inactiveLoops <= 0) {
			// reset
			timer1_set_output_compare_registerA(linInterp(50,SERVO_MIN,SERVO_MAX));
			timer1_set_output_compare_registerB(linInterp(25,SERVO_MIN,SERVO_MAX));
			int rgbcolor = (P <= 664 ? RGBR : RGBB);
			PORTD = ((mainLoops%2)<<rgbcolor)&(7<<rgbcolor);
		}
		else {
			inactiveLoops--;
			PORTD = 1<<(P <= 664 ? RGBR : RGBB);

			// set servo position
			if(S[1] > -1 && S[1] <= 100 ) {
				timer1_set_output_compare_registerA(linInterp(S[1],SERVO_MIN,SERVO_MAX));
			}
			if(S[2] > -1 && S[2] <= 100 ) {
				timer1_set_output_compare_registerB(linInterp(S[2],SERVO_MIN,SERVO_MAX));
			}
			// send thermo value
			printf("S1:%d\n", S[1]);
			printf("S2:%d\n", S[2]);
			printf("T1:%d\n", readTemp(0));
			printf("T2:%d\n", readTemp(1));
			printf("D1:%d\n", adc_read(2));
			printf("P1:%d\n", P);
		}
		_delay_ms(250);
	}
	return 0;
}

