 /** 
  * File:   lcd.h 
  * 
  * Authors: Anton Christensen (anton.christensen9700@gmail.com)
  * 		 Casper Skov Mathisen
  * 
  * Date:    		Winter 2014
  * last updated:   Jan.14 2015
  * 
  * Description: 
  * 	Custom built library functionality for using the school
  *		dev board with a 20x4 character display.
  *
  * Usage:
  * 	Start by calling 'lcd_init();', this will initialize the lcd and turn on the backlight
  * 	Set the position of the cursor with 'lcd_gotoxy(x,y)'
  * 	write to the screen with 'lcd_put( char );', 'lcd_puts( string );' or 'lcd_puti( integer );'
  * 	or printf like function lcd_printf
  *  	clear the screen with 'lcd_clear();'.
  *   	you can also write custom charecters to 8 memory locations in CGRAM (character generator random-access-memory)
  * 	
  */

#ifndef __lcd_h__
#define __lcd_h__

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#define LCDPORT PORTC
#define LCDPIN PINC
#define LCDDDR DDRC
#define RS 0
#define RW 1
#define EN 2
#define BL 3 // backlight switch
#define BF 7
#define D4 4
#define D5 5
#define D6 6
#define D7 7

#define true 1
#define false 0

typedef unsigned char byte;
typedef unsigned char bool;

bool lcd_gotoxy(unsigned char x, unsigned char y);				// Sets cursor position
void lcd_clear();												// Clears screen and resets cursor position to (0,0)
void lcd_printf(char* str, ...);								// Printf style formatted string function
void lcd_put(char c);											// Writes a char to screen
void lcd_puts(char* str);										// Writes a string to screen
void lcd_puti(int i);											// Writes integer to screen
void lcd_set_char(unsigned char addr, unsigned char* rows);		// Stores a custom charecter in Charecter Generator RAM - OBS! Resets cursor position to (0,0)
void lcd_init();												// Initializes the LCD screen. This should be called before any other LCD calls

void _lcd_flash();
void _lcd_check_busy();
void _lcd_write_nibble(unsigned char cmd, bool rs);
void _lcd_write_byte(unsigned char byte, unsigned char rs);
void _lcd_set_cgram_address(unsigned char addr);
void _lcd_set_ddram_address(unsigned char addr);

bool lcd_initialised = false;
int cursor_pos_x = 0;
int cursor_pos_y = 0;



bool lcd_gotoxy(unsigned char x, unsigned char y) {
	if(x >= 20 || y >= 4)
		return false;
	
	int addr = x;
	switch(y) {
		case 1:
			addr += 0x40;
			break;
		case 2:
			addr += 0x14;
			break;
		case 3:
			addr += 0x54;
			break;
	}
	cursor_pos_x = x;
	cursor_pos_y = y;
	_lcd_set_ddram_address(addr);
	return true;
}

void lcd_clear() {
	_lcd_write_byte(1,0);
	_delay_ms(2);
	lcd_gotoxy(0,0);
}

void lcd_printf(char* str, ...) {
   va_list args; 				// allocate va_list
   char buffer[81];				// Allocate char buffer
   va_start(args, str);			// initialize va_list
   vsprintf(buffer, str, args);	// write formatted string to buffer using args
   va_end(args);				// properly end use of va_list				
   lcd_puts(buffer);			// print string to screen
}

void lcd_put(char c) {
	char str[2];
	str[0] = c;
	str[1] = '\0';
	lcd_puts(str);
}

void lcd_puts(char* str) {
	int i = 0;
	while(str[i] != '\0') {
		if(cursor_pos_y > 3 || (cursor_pos_y == 3 && cursor_pos_x >= 20))
			return;
		
		if(str[i] == '\n') {
			if(lcd_gotoxy(0, ++cursor_pos_y)) {
				i++;
				continue;
			}
			else {
				cursor_pos_y++;
				cursor_pos_x = 0;
				return;
			}
		} 
		else if(cursor_pos_x >= 20) {
			if(!lcd_gotoxy(cursor_pos_x-20, ++cursor_pos_y)) {

				return;
			}
		}
		
		_lcd_write_byte(str[i],1);
		cursor_pos_x++;
		i++;
	}
}

void lcd_puti(int i) {
	char buffer[20];
	itoa(i, buffer, 10);
	lcd_puts(buffer);
}

void lcd_set_char(unsigned char addr, unsigned char* rows) {
	addr %= 8;
	addr *= 8;

	_lcd_set_cgram_address(addr);
	char i;
	for(i = 0; i < 8; i++) {
		_lcd_write_byte((*(rows+i)) & 0x1F, 1);
	}
	
	lcd_gotoxy(0,0); // reset position so that we are no longer writing to CGRAM but instead 
}

void lcd_init() {
	if(lcd_initialised)
		return;
	lcd_initialised = true;
	LCDDDR = 0xFF;

	_delay_ms(1000);	//Wait for boot
	//Function set
	_lcd_write_nibble(0b0010,0);	// Set 4-bit mode
	_delay_ms(10);
	_lcd_write_byte(0b00101000,0);	// Set 2-line mode and display off
	_delay_us(100);
	_lcd_write_byte(0b00001100,0);	// display on, cursor off, blink off
	_delay_us(100);
	_lcd_write_byte(0b00000001,0);	//clear display
	_delay_ms(20);
	_lcd_write_byte(0b00000110,0);	// set increment mode and entire shifts off
	_delay_ms(20);

	LCDPORT = 1<<BL; // turn on backlight
}

void _lcd_flash() {
	_delay_us(10);
	LCDPORT |=  (1<<EN);
	_delay_us(10);
	LCDPORT &= ~(1<<EN);
	_delay_us(100);
}

void _lcd_check_busy() {
	LCDDDR = 1<<EN | 1<<RW | 1<<RS;
	LCDPORT |=  (1<<RW);
	LCDPORT &= ~(1<<RS);

	while (LCDPIN & (1<<BF)) {
		_lcd_flash();
	}

	LCDDDR = 0xFF;
	LCDPORT &= ~(1<<RW);
}

void _lcd_write_nibble(unsigned char nibble, bool rs) {
	//_lcd_check_busy();
	LCDPORT &= ~(0x0F<<D4);
	LCDPORT &= ~(1<<RW); 		// Write mode
	if(rs)
		LCDPORT |=  (1<<RS); 	// Data mode
	else
		LCDPORT &= ~(1<<RS);	// Command mode
	
	LCDPORT |=  (nibble<<D4);
	_lcd_flash();
}

void _lcd_write_byte(unsigned char byte, unsigned char rs) {
	_lcd_write_nibble((byte >> 4) & 0x0F, rs); 	// Send latter four bits
	_lcd_write_nibble(byte & 0x0F, rs);  		// Send former fout bits
}

void _lcd_set_cgram_address(unsigned char addr) {
	addr %= 0b01000000;
	_lcd_write_byte((0b01000000 | (addr)),0); // Set character address
}

void _lcd_set_ddram_address(unsigned char addr) {
	//addr %= 0b10000000;
	_lcd_write_byte((0b10000000 | addr),0); // Set character address
}

#endif