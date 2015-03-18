 /** 
  * File:    buttons.h 
  * 
  * Author1:  Anton Christensen (anton.christensen9700@gmail.com)
  * Date:     Winter 2014 
  * 
  * Description: 
  * 	Functionality for using the button board 
  * 	connected to an AVR
  *
  */

#ifndef __buttons_h__
#define __buttons_h__

#include <avr/io.h>

#define BUTTONPIN PIND

char is_pressed(int pin) {
	return ~BUTTONPIN & 1<<pin;
}

#endif
