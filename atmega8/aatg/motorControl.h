#ifndef __MOTOR_CONTROL__
#define __MOTOR_CONTROL__

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "essentials.h"
#include "lcd.h"

#define WAVE_DRIVE 0
#define FULL_STEP  1
#define HALF_STEP  2
#define CLOCKWISE         1
#define COUNTERCLOCKWISE -1
#define DEFAULT_DELAY 2

#define maxDelay 250
#define MOTORPORT PORTA
#define nCoils 4

typedef struct Stepmotor {
	int angle10;
	int stepSize10;
	volatile uint8_t* port;
	int currPhase;
	unsigned char tmp; // for use when half-stepping
	unsigned char nPhases;
	unsigned char* phaseIndex;
	unsigned char steppingScheme;
} Stepmotor;

void _modulatePhase(Stepmotor* motor);
Stepmotor* stepmotor_init(int stepSize10, unsigned char steppingScheme, volatile uint8_t* port, unsigned char nPhases, unsigned char* phaseIndex);
void stepmotor_set_angle(Stepmotor* motor, int angle);
void stepmotor_step(Stepmotor* motor, long int steps);
void stepmotor_step_wave_drive(Stepmotor* motor, char direction);
void stepmotor_step_full_step(Stepmotor* motor, char direction);
void stepmotor_step_half_step(Stepmotor* motor, char direction);
//--------------------------------------------------------------//
typedef struct Dcmotor {
	volatile uint8_t* PWM_port;
	volatile uint8_t* direction_port;
	unsigned char PWM_pin;
	unsigned char direction_pin;

} Dcmotor;

void dcmotor_set_speed(int speed);

void _modulatePhase(Stepmotor* motor) {
	if(motor->currPhase < 0)
		motor->currPhase += motor->nPhases;
	else if(motor->currPhase >= motor->nPhases)
		motor->currPhase %= motor->nPhases;
}

Stepmotor* stepmotor_init(int stepSize10, unsigned char steppingScheme, volatile uint8_t* port, unsigned char nPhases, unsigned char* phaseIndex) {
	Stepmotor* motor = (Stepmotor*)malloc(sizeof(Stepmotor));
	motor->tmp = 0;
	motor->angle10 = 0;
	motor->currPhase = 0;
	motor->port = port;
	motor->nPhases = nPhases;
	motor->stepSize10 = stepSize10;
	motor->phaseIndex = phaseIndex;
	motor->steppingScheme = steppingScheme;
	return motor;
}


/**
 * Get the current angle of the stepmotor
 * @param {Stepmotor*} motor: pointer to Stepmotor structure
 * @return {int} angle
 */
int stepMotor_get_angle(Stepmotor* motor) {
	return motor->angle10 / 10;
}

/**
 * Set a stepmotor to a certain angle
 * @param {Stepmotor*} motor: motor pointer structure
 * @param {int} angle: angle to set the motor to
 */
void stepmotor_set_angle(Stepmotor* motor, int angle) {
	
	while(angle <= -360)
		angle += 360;
	if(angle >= 360)
		angle %= 360;
	// angle is now between -360 and +360
	angle = angle * 10; // make angle to 10*angle format
	if ((angle - motor->angle10 + 3600) % 3600 <= 1800) {
		// clockwise
		while(norm(angle - motor->angle10) > motor->stepSize10)
			stepmotor_step(motor, CLOCKWISE);
	}
	else {
		// anti-clockwise
		while(norm(angle - motor->angle10) > motor->stepSize10)
			stepmotor_step(motor, COUNTERCLOCKWISE);
	}
}

/**
 * move stepmotor x steps in positive or negative direction
 * @param {Stepmotor*} motor: motor to execute on
 * @param {long int} steps: number of steps, if negative, run counterclockwise
 */
void stepmotor_step(Stepmotor* motor, long int steps) {
	char direction = CLOCKWISE;
	if(steps < 0)
		direction = COUNTERCLOCKWISE;
	
	steps = norm(steps);

	switch(motor->steppingScheme) {
		case WAVE_DRIVE:
			while(steps--)
				stepmotor_step_wave_drive(motor, direction);
			break;
		case FULL_STEP:
			while(steps--)
				stepmotor_step_full_step(motor, direction);
			break;
		case HALF_STEP:
			while(steps--)
				stepmotor_step_half_step(motor, direction);
			break;
	}
}

void stepmotor_step_wave_drive(Stepmotor* motor, char direction) {
	// turn off 0, 
	// turn on  1

	// turn off current phase
	*motor->port &= ~(1<<motor->phaseIndex[motor->currPhase]);
	
	// change current phase
	motor->currPhase += direction;
	_modulatePhase(motor);
	
	// turn on next phase
	*motor->port |=  (1<<motor->phaseIndex[motor->currPhase]);

	// set angle
	motor->angle10 += motor->stepSize10*(direction);
	if(motor->angle10 < -1800)
		motor->angle10 += 3600;
	else if(motor->angle10 > 1800)
		motor->angle10 -= 3600;

	delay_ms(DEFAULT_DELAY);
}

void stepmotor_step_full_step(Stepmotor* motor, char direction) {
	// turn off -1
	// turn on   1

	// change current phase
	motor->currPhase -= direction;
	_modulatePhase(motor);
	
	// turn off current phase
	*motor->port &= ~(1<<motor->phaseIndex[motor->currPhase]);

	// change current phase
	motor->currPhase += 2*direction;
	_modulatePhase(motor);

	// turn on next phase
	*motor->port |=  (1<<motor->phaseIndex[motor->currPhase]);

	// set angle
	motor->angle10 += motor->stepSize10*(direction*CLOCKWISE);
	if(motor->angle10 < -3600)
		motor->angle10 += 3600*2;
	else if(motor->angle10 > 3600)
		motor->angle10 -= 3600*2;
	delay_ms(DEFAULT_DELAY);
}

void stepmotor_step_half_step(Stepmotor* motor, char direction) {
	// if tmp = 0: turn on next one
	// if tmp = 1: turn off previus one

	if(motor->tmp == 0) {
		// change current phase
		motor->currPhase += direction;
		_modulatePhase(motor);

		// turn on next phase
		*motor->port |=  (1<<motor->phaseIndex[motor->currPhase]);
	}
	else {
		// change current phase
		motor->currPhase -= direction;
		_modulatePhase(motor);

		// turn on next phase
		*motor->port &= ~(1<<motor->phaseIndex[motor->currPhase]);

		// change current phase
		motor->currPhase += direction;
		_modulatePhase(motor);

		// set angle
		motor->angle10 += motor->stepSize10*(direction*CLOCKWISE);
		if(motor->angle10 < -3600)
			motor->angle10 += 3600*2;
		else if(motor->angle10 > 3600)
			motor->angle10 -= 3600*2;
			

	}
	motor->tmp++;
	motor->tmp %= 2;

	delay_ms(DEFAULT_DELAY/2);
}

//-----------------------------------------------------------------//

// antager PIN-A = PB3
// antager PIN-B = PB2

void dcmotor_init() {
	pwm0_init();
	DDRB |= 1<<2;
}

void dcmotor_set_speed(int speed) {
	// CounterClockwise
	if(speed < 0)
		PORTB |= 1<<2; 
	// Clockwise
	else
		PORTB &= ~(1<<2);

	pwm0_set(speed);
}


//////////////////////////////////////////////

unsigned char stepmotor_run(int speed, unsigned char currPhase, unsigned char motorIndex) {
	
	MOTORPORT &= ~(1<<currPhase);
	currPhase += speed/norm(speed);
	if(currPhase < nCoils*motorIndex)
		currPhase = nCoils-1;
	else {
		currPhase = (currPhase%nCoils)+nCoils*motorIndex;
	}
	MOTORPORT |=  (1<<currPhase);
	delay_ms(maxDelay-norm(speed) );
	return currPhase;
}


#endif