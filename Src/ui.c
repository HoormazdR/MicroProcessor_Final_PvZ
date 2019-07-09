#include "main.h"
#include "stm32f3xx_hal.h"
#include <string.h>

#include "ui.h"
#include "LiquidCrystal.h"

#include "gameBase.h"

// current lcd frame
uint8_t lcd[4][20];

// last lcd frame
uint8_t lcd_last[4][20];

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
	showZombies();
}

void showZombies() {
	for(int i = 0; i < 1; i++) {
		putch(zombieGame[i].place.posx, zombieGame[i].place.posy, 'Z');
	}
}
