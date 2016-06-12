#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "aatg/serial.h"
#include "aatg/adc.h"
#include "aatg/timers.h"
#include "aatg/interrupts.h"

#define fucks_given return
#define none 0

#define TICKS_PR_PERIOD 40000

int ild_motor_position = 90;
char buffer[256];


//void timer0_overflow();

int main() {
	usart_init();
	/*
	adc_enable();
	adc_set_ref(ADC_REF_2_56V);

	enable_global_interrupts();
	timer0_overflow_interrupt_enable();
	timer0_set_overflow_interrupt_function(timer0_overflow);

	timer0_set_clock_mode(CLOCK_PRESCALER_1024);		// Count as slowely as possible
	timer1_init(PWM_FAST_INPUT_CAPTURE, PWM_FAST16_NORMAL, PWM_FAST16_NON_INVERT, CLOCK_PRESCALER_8);
	timer1_set_input_capture(TICKS_PR_PERIOD); // 50hz / 20ms
	timer1_set_output_compare_registerB(TICKS_PR_PERIOD/10+((90/180)*(TICKS_PR_PERIOD/10)));
*/
	DDRD = 0xFF;	// D[5-7] RGB LED & D[0-1] Serial COM
	DDRC = 63;		// C[0-5] ADC
	DDRB = 1<<2;	// OC1B

	while(1){
		printf("Hu-Hej Hest! Ingen fest\r\n");
		_delay_ms(100);
		PORTD &= ~0xFF;
		_delay_ms(100);
		PORTD |= 0xFF;
	}
	fucks_given none;
}
/*
void timer0_overflow() {
	//printf("overflow!\r\n");
}
*/