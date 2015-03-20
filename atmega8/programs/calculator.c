 /** 
  * File:    calculator.c 
  * 
  * Author1:  Anton Christensen (anton.christensen9700@gmail.com)
  * Date:     Winter 2014 
  * 
  * Description: 
  * 	Simple calculator, built for AT-Mega16 
  *		with 20x4 character display.
  * 	Can perform simple integer operations.
  *
  */

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "aatg/lcd.h"
#include "aatg/buttons.h"

void calcMain() {
	DDRB = 0xFF;
	DDRD &= 1<<3;

	int pos = 0, i;
	char* sign = "+";
	int vars[4];
	vars[0] = 0; // 1st
	vars[1] = 0; // sign
	vars[2] = 0; // 2nd
	vars[3] = 0; // result
	
	//+
	//-
	//x
	//%


	
	char str_num[20];
	bool changed = 1;
	while(1) {
		if(isPressed(6))
			break;
		if(pos <3) {
			if(isPressed(3)) {
				vars[pos]++;
				changed = 1;
			}
			else if(isPressed(4)) {
				vars[pos]--;
				changed = 1;
			}
			else if(isPressed(5)) {
				pos++;
				changed = 1;
			}
			
		}
		if(pos == 1) {
			if(vars[1] < 0)
				vars[1] = 3;
			vars[1] %= 4;
			//if(vars[1] < 0)
			//	vars[1] = 0;
			//else if(vars[1] > 3)
			//	vars[1] = 3;
		}
		switch(vars[1]) {//sign
			case 0:
				sign = "+\0";
				break;
			case 1:
				sign = "-\0";
				break;
			case 2:
				sign = "x\0";
				break;
			case 3:
				sign = "%\0";
				break;
		}
		if(pos == 3) {
			switch(vars[1]) {
				case 0:
					vars[pos] = vars[0] + vars[2];
					break;
				case 1:
					vars[pos] = vars[0] - vars[2];
					break;
				case 2:
					vars[pos] = vars[0] * vars[2];
					break;
				case 3:
					vars[pos] = vars[0] / vars[2];
					break;
			}
		}

		if(changed) {
			lcd_clear();
			for(i = 0; i <= pos; i++) {
				lcd_gotoxy(9,i);
				if(i == 0 || i == 2 || i == 3)
					lcd_puts(itoa(vars[i], str_num, 10));
				else
					lcd_puts(sign);
			}
		}
		PORTB = vars[pos];
		_delay_ms(100);
		changed = 0;
	}
	lcd_clear();
	return;
}

void Calculator() {
	lcd_init();
	while(1) {
		calcMain();
	}
	
}

