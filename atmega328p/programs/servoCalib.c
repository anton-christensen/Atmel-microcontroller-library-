// Standard libs
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "../aatg/essentials.h"
#include "../aatg/interrupts.h"
#include "../aatg/serial.h"
#include "../aatg/adc.h"
#include "../aatg/timers.h"

int main() {

	unsigned int min = 800, max = 2000, pos = 50, width = 40000;
	usart_init();

	timer1_init(PWM_FAST_INPUT_CAPTURE, PWM_FAST16_NON_INVERT, PWM_FAST16_NON_INVERT, CLOCK_PRESCALER_8);
	timer1_set_input_capture(width);
	timer1_set_output_compare_registerA(linInterp(pos,min,max));

	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;
	
	char buffer[256];

	while(1 == 1) {
		if(pos <= 100 ) {
			timer1_set_output_compare_registerA(linInterp(pos,min,max));
		}
		if(pos <= 100 ) {
			timer1_set_output_compare_registerB(linInterp(pos,min,max));
		}
		scanf("%s", &buffer);
		printf(" => %s\r\n", buffer);
		switch(buffer[0]) {
			case 'm':
				sscanf(buffer+1, "%d", &min);
				printf("    MIN => %d\r\n", min);
				break;
			case 'M':
				sscanf(buffer+1, "%d", &max);
				printf("    MAX => %d\r\n", max);
				break;
			case 'p':
				sscanf(buffer+1, "%d", &pos);
				printf("    POS => %d\r\n", pos);
				break;
			case 'w':
				sscanf(buffer+1, "%d", &width);
				timer1_set_input_capture(width);
				printf("    WIDTH => %d\r\n", width);
				break;
		}
	}
	return 0;
}

