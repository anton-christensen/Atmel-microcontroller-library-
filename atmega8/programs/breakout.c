 /** 
  * File:    breakout.c 
  * 
  * Author1:  Anton Christensen (anton.christensen9700@gmail.com)
  * Date:     Winter 2014 
  * 
  * Description: 
  * 	Breakout game, built for AT-Mega16 
  *		with 20x4 character display.
  *  	requires the aatg library
  */

// 8x5  dots/char
// 4x20 chars/screen
// 32x100 resolution
#include <util/delay.h>
#include <stdlib.h>

#include "../aatg/buttons.h"
#include "../aatg/lcd.h"

#define SCREEN_WIDTH  32
#define SCREEN_HEIGHT 100
#define CHARH 5
#define CHARW 8
#define YCHARS 20
#define XCHARS 4 

#define BRICK 0xFF
#define TRANSPARENT 0xFE

void Breakout(); // Run this function to start the program

typedef struct object {
	int x, y, w, h;
	bool visible;
} object;

int Draw(object* obj, int start) {
	// make empty array of tiles
	unsigned char screen[20][4][8];
	int a,b,c;
	for(a = 0; a < 20; a++)
		for(b = 0; b < 4; b++)
			for(c = 0; c < 8; c++)
				screen[a][b][c] = 0;

	//calculate pixels to be drawn
	int y,x;
	for(y = 0; y < obj->h; y++) {
		for(x = 0; x < obj->w; x++) {
			screen[(obj->y + y)/CHARH][(obj->x + x)/CHARW][(obj->x + x)%CHARW] |= (1<<((SCREEN_HEIGHT-1-(obj->y + y))%CHARH));
		}
	}

	// generate sprites and draw
	int spriteNum = start;
	for(y = obj->y-(obj->y%CHARH); y < obj->y+obj->h; y+= CHARH) {
		for(x = obj->x-(obj->x%CHARW); x < obj->x+obj->w; x+= CHARW) {
			if(spriteNum < 7) {
				lcd_set_char(spriteNum,(unsigned char*)screen[y/CHARH][x/CHARW]);
				lcd_gotoxy(y/CHARH, x/CHARW);
				_lcd_write_byte(spriteNum++,1);
			}
		}
	}
	return spriteNum;
		
}

void Breakout() {

	int i,j;
	int points = 0;
	DDRB = 0xFF;
	PORTB = points;

	object bricks[4][3];
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 3; j++) {
			bricks[i][j].x = i;
			bricks[i][j].y = j+YCHARS-3;
			bricks[i][j].w = CHARW;
			bricks[i][j].h = CHARH;
			bricks[i][j].visible = true;
		}
	}
	// Draw bricks
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 3; j++) {
			lcd_gotoxy(bricks[i][j].y,bricks[i][j].x);
			_lcd_write_byte(BRICK,1);
		}
	}
	
	object player = {12,9,7,1,1};
	object ball   = {15,10,1,1,1};
	int ballDX, ballDY;
	ballDY = 1;
	ballDX = 1;
	bool hit = true;
	bool changed = true;
	int oldXSquare = ball.x/CHARW, oldYSquare = ball.y/CHARH;
	while(1) {
		// if left pressed
		if(is_pressed(3) && player.x > 0) {
			// if player pad leaving a screen character
			if((player.x+player.w)%CHARW == 1) {
				//overwrite with blank
				lcd_gotoxy((player.y+player.h-1)/CHARH, (player.x+player.w)/CHARW);
				_lcd_write_byte(TRANSPARENT,1);
			}
			changed = true;
			player.x--;
		}
		//if right pressed
		if(is_pressed(6) && player.x + player.w < SCREEN_WIDTH) {
			// if player pad leaving a screen character
			if(player.x%CHARW == CHARW-1) {
				//overwrite with blank
				lcd_gotoxy(player.y/CHARH, player.x/CHARW);
				_lcd_write_byte(TRANSPARENT,1);
			}
			changed = true;
			player.x++;
		}

		
		ball.y += ballDY;
		if(ball.y == player.y) {
			ball.x += ballDX;
			if(ball.x >= player.x && ball.x <= player.x+player.w) {
				ballDY *= -1;
				ball.y += ballDY*2;
			}
		}
		else {
			if(ball.y/CHARH > YCHARS-4  ) {
				if(bricks[ball.x/CHARW][YCHARS-(ball.y/CHARH)-1].visible == true) {
					bricks[ball.x/CHARW][YCHARS-(ball.y/CHARH)-1].visible = false;
					lcd_gotoxy(ball.y/CHARH, ball.x/CHARW);
					_lcd_write_byte(TRANSPARENT,1);
					ballDY *= -1;
					ball.y += ballDY;
					points++;
					PORTB = points;
				}
			}

			ball.x += ballDX;
			if(ball.y/CHARH > YCHARS-4  ) {
				if(bricks[ball.x/CHARW][YCHARS-(ball.y/CHARH)-1].visible == true) {
					bricks[ball.x/CHARW][YCHARS-(ball.y/CHARH)-1].visible = false;
					lcd_gotoxy(ball.y/CHARH, ball.x/CHARW);
					_lcd_write_byte(TRANSPARENT,1);
					ballDX *= -1;
					ball.x += ballDX;
					points++;
					PORTB = points;
				}	
			}
		}

		if(ball.y == SCREEN_HEIGHT-1)
			ballDY *= -1;
		if(ball.x == 0 || ball.x == SCREEN_WIDTH-1)
			ballDX *= -1;

		if(changed) {
			Draw(&player, 1);
		}
		

		//DRAW BALL
		if(ball.x/CHARW != oldXSquare || ball.y/CHARH != oldYSquare) {
			//Clear ball position //TODO: if leaving CG square
			lcd_gotoxy(oldYSquare,oldXSquare);
			_lcd_write_byte(TRANSPARENT,1);
			oldXSquare = ball.x/CHARW;
			oldYSquare = ball.y/CHARH;
		}
		
		if(player.y/CHARH == oldYSquare && player.x/CHARW == oldXSquare)
			;//Draw(&ball, 1);
		else if(player.y/CHARH == oldYSquare && (player.x+player.w)/CHARW == oldXSquare)
			;//Draw(&ball, 2);
		else
			Draw(&ball, 0);

		if(ball.y <= 0) {
			_delay_ms(1000);
			lcd_clear();
			lcd_gotoxy(6,2);
			lcd_puts("You Lost");
			_delay_ms(3000);
			lcd_clear();
			break;
		}
		else if(points == 12) {
			_delay_ms(1000);
			lcd_clear();
			lcd_gotoxy(6,1);
			lcd_puts("You Won!");
			_delay_ms(3000);
			lcd_clear();
			break;
		}

		//do timeing
		_delay_ms(100);
		changed = hit = false;
	}

}
