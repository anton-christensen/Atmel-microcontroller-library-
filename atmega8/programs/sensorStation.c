#include <stdio.h>

#include <avr/io.h>
#include <util/delay.h>

#include "../aatg/adc.h"
#include "../aatg/interrupts.h"
#include "../aatg/lcd.h"
#include "../aatg/serial.h"
#include "../aatg/timers.h"
#include "../aatg/buttons.h"

#include "splashDLL.c"

#define LUMEN_SENSOR_PIN 	0
#define THERMO_SENSOR_PIN 	1

typedef struct sensor_data {
	unsigned int thermo_sensor;
	unsigned int lumen_sensor;
} sensor_data;


void SensorStation();
void interr_iddle_led();
void get_sensor_data();
void render();
void user_input();

sensor_data data;
signed char counter0 = 0;
signed char counter1 = 0;
signed char counter0_dir = 1;
char fan_speed = 0;
char fan_on = 0;
char buffer[256];

void SensorStation() {
	// flash idle led
	DDRD |= 1<<OC2;
	DDRB |= 1<<OC0;
	DDRA |= 0b11111100;

	timer2_init(PWM_FAST, PWM_FAST_NON_INVERT, CLOCK2_PRESCALER_1024);
	timer2_overflow_interrupt_enable();
	timer2_set_overflow_interrupt_function(interr_iddle_led);
	enable_global_interrupts();

	// wait for start signal from client program
	usart_init();
	do {
		gets(buffer);
	}
	while(strcmp(buffer, "start") != 0);
	// send respons that signal was recieved
	printf("ready\n");
	
	// continous light in run led
	timer2_init(0,0,0); // disable timer2
	timer2_overflow_interrupt_disable();
	// light LED
	DDRD  |= 1<<7;	
	PORTD |= 1<<7;

	// boot LCD and show splash screen
	lcd_init();
	Splash();

	// set render clock interrupt
	timer1_init(CLEAR_ON_COMPARE16_OC, 0,0, CLOCK_PRESCALER_1024);
	timer1_set_output_compare_registerA(3125*2); // = sec*((CPU)ticks/sec)/Prescaler
	timer1_output_compareA_interrupt_enable();
	timer1_set_output_compareA_interrupt_function(render);

	timer0_init(PWM_FAST, PWM_FAST_NON_INVERT, CLOCK_PRESCALER_256);  	// Timer 0 controls the fan speed
	timer2_init(CLEAR_ON_COMPARE, 0, CLOCK2_PRESCALER_1024);			// Timer 2 manages button input
	timer2_overflow_interrupt_enable();
	timer2_set_overflow_interrupt_function(user_input);

	adc_enable();
	adc_set_ref(ADC_REF_AVCC);
	while(1) {
		// Read data from sensors
		get_sensor_data(&data);
		printf("T\n");
		printf("%i\n", data.thermo_sensor);
		printf("L\n");
		printf("%i\n", data.lumen_sensor);
		//_delay_ms(100);
		// show indicators measuring sensor values
		// send sensor input through serial
	}

}

void interr_iddle_led() {
	counter0 += counter0_dir;
	if(counter0 == -25 || counter0 == 75)
		counter0_dir *= -1;

	timer2_set_output_compare_register(duty_cycle_8bit(counter0 > 0 ? counter0 : 0));
}

void get_sensor_data(sensor_data* p_data) {
	p_data->thermo_sensor = adc_read(THERMO_SENSOR_PIN);
	p_data->lumen_sensor  = adc_read(LUMEN_SENSOR_PIN);
}

void render() {
	lcd_clear();
	lcd_printf("Lux:   %i\n%s\nTherm: %i - %i\nFan: %s - Fspee: %i", 
				data.lumen_sensor, (data.lumen_sensor <= 600  ? "DAYTIME!" : "nighttime..."),
				data.thermo_sensor, ((data.thermo_sensor/2) -273),
				(fan_on ? "ON" : "OFF"), fan_speed);
}

void user_input() {
	if(is_pressed(6)) {
		fan_on = !fan_on;
	}
	if(is_pressed(4) && fan_speed < 100) {fan_speed++;}
	if(is_pressed(5) && fan_speed > 0)   {fan_speed--;}
	if(((data.thermo_sensor/2) -273) >= 23) {
		timer0_set_output_compare_register(duty_cycle_8bit(100));
	}
	else
		timer0_set_output_compare_register(duty_cycle_8bit(0));
}

// thermo sensor
//    0 -    4.9 Volts
//    0 -  490 Kelvin
// -273 -  267 Celcius
//    0 - 1023 Decimal

// 2.73 -  4.9 Volts
//  273 -  490 Kelvin
//    0 -  277 Celcius
//  571 - 1023 Decimal


// 1    Volts
// 100  Kelvin
// 209  Decimal

// 0.01 Volts
// 1    Kelvin
// 2.09 Decimal

// 0.005 Volts
// 0.478 Kelvin
// 1     Decimal