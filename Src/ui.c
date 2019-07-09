#include "main.h"
#include "stm32f3xx_hal.h"
#include <string.h>

#include "ui.h"
#include "LiquidCrystal.h"

//the main state of whole game
extern enum State{
	GAME,
	MENU,
	ENTER_NAME
};

extern enum state;

// current lcd frame
uint8_t lcd[4][20];

// last lcd frame
uint8_t lcd_last[4][20];

uint8_t cursorPos[2] = {0};

//Based Function for lcd and stuff
// lcd low level functions
void putch(int x, int y, char t) {
	if(x<0||y<0||x>=20||y>=4)
		return;
	lcd[y][x] = t;
}
void putchp(int x, int y, char* t) {
	putch(x,y,t[0]);
}
void putstr(int x, int y, const char str[]) {
  if (str == NULL) return;

  const uint8_t *buffer = (const uint8_t *)str;
  size_t size = strlen(str);

  while (size--) {
		lcd[y][x] = *buffer++;
		x++;
  }
}
void clearLCD_Rect(int sx, int sy, int w, int h) {
  for (int y=sy;y<h;y++)
    for (int x=sx;x<w;x++)
			lcd[y][x]=' ';
}
void clearLCD() {
	clearLCD_Rect(0,0,20,4);
}

// this function refresh just changed parts of lcd
uint8_t cursor_interval = 0;
void refresh_lcd() {

	int cux=0;
	for (int y=0;y<4;y++) {
		cux=-1;
		for (int x=0;x<20;x++) {
				if(lcd[y][x]!=lcd_last[y][x]) {
					if(cux != x) {
						cux=x;
						setCursor(cux,y);
					}
					write(lcd[y][x]);
					cux++;
				}
		}

	}
    //cursor controller
    if(cursorPos[0] > -1 && cursorPos[1] > -1){
    	if(!cursor_interval){
    		setCursor(cursorPos[1], cursorPos[0]);
			write('_');

    	}
    	else{
    		setCursor(cursorPos[1], cursorPos[0]);
    		write(lcd[cursorPos[0]][cursorPos[1]]);

    	}
    	cursor_interval = !cursor_interval;
    }



	// save last frame after update
	memcpy(lcd_last,lcd,80);
}


void show_7seg(int a) {
	HAL_GPIO_WritePin(SevSeg_a0_GPIO_Port, SevSeg_a0_Pin, a & 1);
	HAL_GPIO_WritePin(SevSeg_a1_GPIO_Port, SevSeg_a1_Pin, a & 2);
	HAL_GPIO_WritePin(SevSeg_a2_GPIO_Port, SevSeg_a2_Pin, a & 4);
	HAL_GPIO_WritePin(SevSeg_a3_GPIO_Port, SevSeg_a3_Pin, a & 8);
}

void show_7seg_oni(int i, int a) {
	HAL_GPIO_WritePin(SSEG_i0_GPIO_Port, SSEG_i0_Pin, 0);
	HAL_GPIO_WritePin(SSEG_i1_GPIO_Port, SSEG_i1_Pin, 0);
	HAL_GPIO_WritePin(SSEG_i2_GPIO_Port, SSEG_i2_Pin, 0);
	HAL_GPIO_WritePin(SSEG_i3_GPIO_Port, SSEG_i3_Pin, 0);

	show_7seg(a);

	HAL_GPIO_WritePin(SSEG_i0_GPIO_Port, SSEG_i0_Pin, i == 0);
	HAL_GPIO_WritePin(SSEG_i1_GPIO_Port, SSEG_i1_Pin, i == 1);
	HAL_GPIO_WritePin(SSEG_i2_GPIO_Port, SSEG_i2_Pin, i == 2);
	HAL_GPIO_WritePin(SSEG_i3_GPIO_Port, SSEG_i3_Pin, i == 3);
}

void lcd_inital() {
	 for (int y=0;y<4;y++) {
	      setCursor(0,y);
	      for (int x=0;x<20;x++) {
			lcd[y][x]=' ';
	        write(lcd[y][x]);
	      }
	    }

	 memcpy(lcd_last,lcd,80);
}

void test_ui() {

//	HAL_Delay(5000);
	putstr(0, 0, "Hoormazd");
}

void moveCursor(uint8_t x, uint8_t y){
	if(x<0||y<0||x>=20||y>=4)
			return;
	cursorPos[0] = y;
	cursorPos[1] = x;
}

void ui_enterNameInit(){
	clearLCD();
	putstr(0,0,"Enter your name");
	putstr(7,2,"****");
	moveCursor(7, 2);
}
void ui_enterName_putchar(char c){
	putch(cursorPos[1],cursorPos[0], c);
}
