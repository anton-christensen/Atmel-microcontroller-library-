  /** 
  * File:    serial.h 
  * 
  * Author:  Anton Christensen (anton.christensen9700@gmail.com)
  * Date:    January 2014 
  * 
  * Description: 
  * 	Serial communications functionality
  * 	directs STDIN and STDOUT to the USART to be send via a serial interface
  *
  * Usage: 
  * 	Run 'usart_init();' to setup and initialize the usart, 
  * 	then just use ordinary stdio functions to send and recieve data
  *
  * Credits:
  * 	Based on - https://github.com/tuupola/avr_demo/tree/master/blog/simple_usart
  */

#ifndef __AATG_SERIAL__
#define __AATG_SERIAL__

#define BAUD 9600

#include <avr/io.h>
#include <util/setbaud.h>
#include <stdio.h>
 
#define MCP2200_UBBR F_CPU/16/BAUD-1

void usart_init();
char _usart_getchar(FILE* stream); 
void _usart_putchar(char data, FILE* stream);

FILE usart_output = FDEV_SETUP_STREAM(_usart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE usart_input  = FDEV_SETUP_STREAM(NULL, _usart_getchar, _FDEV_SETUP_READ);

void usart_init() {
  // Set baud rate
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
  // Enable receiver and transmitter
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  // Set frame format: 8data, 1stop bit, parity bit disables
  UCSR0C = (1<<UCSZ00)|(1<<UCSZ01);

  // bind stdin and stdout to usart
  stdout = &usart_output;
  stdin = &usart_input;
}
void _usart_putchar(char data, FILE* stream) {
  while ( !(UCSR0A & (1<<UDRE0)) );	// Wait for empty transmit buffer
  UDR0 = data; 						// Start transmission
}
char _usart_getchar(FILE* stream) {
  while ( !(UCSR0A & (1<<RXC0)) );	// Wait for incoming data
  return UDR0;							// Return the data
}

#endif