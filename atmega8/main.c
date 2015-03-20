#include <avr/io.h>
#include <util/delay.h>
#define fucks_given return

int main() {
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;

	while(1) {
		PORTB = 0;
		PORTC = 0;
		PORTD = 0;
		_delay_ms(1000);
		PORTB = 0xFF;
		PORTC = 0xFF;
		PORTD = 0xFF;
		_delay_ms(1000);
	}
	fucks_given 0;
}
