#include "main.h"
#include "stm32f3xx_hal.h"
#include <string.h>

#include "ui.h"
#include "LiquidCrystal.h"

#include "gameBase.h"
//the main state of whole game


extern enum State{
	GAME,
	MENU,
	ENTER_NAME
};

extern enum state;
int GameState = STE_NORMAL_GAME;

unsigned char enemyType1[] = {
		0x00, 0x0E, 0x0E, 0x1F, 0x04, 0x04, 0x0E, 0x0A
};

unsigned char enemyType1_frame2[] = {
		0x00, 0x0E, 0x0E, 0x04, 0x1F, 0x04, 0x0E, 0x0A
};

unsigned char plantType2[] = {
		0x1C, 0x1E, 0x10, 0x15, 0x1F, 0x10, 0x10, 0x1C
};

unsigned char plantType1[] = {
		 0x15, 0x15, 0x0E, 0x04, 0x15, 0x1F, 0x0E, 0x04
};

// current lcd frame
uint8_t lcd[4][20];

// last lcd frame
uint8_t lcd_last[4][20];
uint8_t cursorPos[2] = {0};
uint8_t cursor_blink_flg = 0;
uint8_t cursor_changed = 0;
uint8_t preCursorPos[2] = {0};

//uart log

extern uint16_t potanLightRand[3];
extern UART_HandleTypeDef huart3;

//UART

#ifdef __GNUC__
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 1000); // change &uart1 accordingly
	return ch;
}


void log(char str[]){
	int size = strlen(str);
	HAL_UART_Transmit(&huart3, str, size, 1000);
}
void log_adc(){
	  char whatToTransfare[40];
	  sprintf(&whatToTransfare, "Potan: %4d, Light: %4d, Rand: %4d \n"
			  ,potanLightRand[0], potanLightRand[1], potanLightRand[2]);
	  log(whatToTransfare);
}


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


	//Show cursor-----------------

	if(cursor_changed){
		cursor_changed = 0;
		setCursor(preCursorPos[0], preCursorPos[1]);
		write(' ');
	}
	preCursorPos[0] = cursorPos[0];
	preCursorPos[1] = cursorPos[1];

	setCursor(cursorPos[0], cursorPos[1]);
	uint8_t interval = 3;
	if(cursor_blink_flg < interval){
		write(lcd[cursorPos[0]][cursorPos[1]]);
	}
	else{
		write('_');
	}
	cursor_blink_flg++;
	if(cursor_blink_flg > 2 * interval)
		cursor_blink_flg = 0;

//	cursor_blink_flg = !cursor_blink_flg;

	//-----------------------------
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

int digit_7seg=0;
void refresh_7seg() {

	// game time to current ragham
	int t = time_game;
	int ragham;
	if(digit_7seg==0)
		ragham=health;
	if(digit_7seg==1)
		ragham=t/100%10;
	if(digit_7seg==2)
		ragham=t/10%10;
	if(digit_7seg==3)
		ragham=t%10;

	HAL_GPIO_WritePin(SSEG_dot_GPIO_Port, SSEG_dot_Pin, 1);
	if(digit_7seg==0)
		HAL_GPIO_WritePin(SSEG_dot_GPIO_Port, SSEG_dot_Pin, 0);

	// normal show 7seg
	show_7seg_oni(digit_7seg, ragham);

	digit_7seg++;
	digit_7seg=digit_7seg%4;

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

//GameCodes
/******************************************************************/

int frame = 0;
int startFrame = 0;
void screen_normal_game() {
	if(frame==0) {
		 createChar(0, enemyType1);
		 createChar(1, enemyType1_frame2);
		 createChar(2, plantType1);
	}
	char enemy = 0;
	char enemy_f2 = 1;
	char plant_Type1 = 2;

	clearLCD();
	for(int i = 0; i < exist_enemy; i++) {
		if(frame%2 == 0)
			putch(actorOfTheGame.PvZzombies[i].place.posx, actorOfTheGame.PvZzombies[i].place.posy, enemy);
		else if (frame%2 == 1)
			putch(actorOfTheGame.PvZzombies[i].place.posx, actorOfTheGame.PvZzombies[i].place.posy, enemy_f2);
	}

	for(int i = 0; i < exist_plant; i++) {
		if(frame%2==0)
			putch(actorOfTheGame.PvZPlants[i].place.posx, actorOfTheGame.PvZPlants[i].place.posy,  plant_Type1);
		else if(frame%2==1)
			putch(actorOfTheGame.PvZPlants[i].place.posx, actorOfTheGame.PvZPlants[i].place.posy,  plant_Type1);

	}

	if(frame > 100)
		frame = 1;
}

void refresh_ui(void) {
	int ok;
	// normal game
	if (GameState == STE_NORMAL_GAME) {
		screen_normal_game();
		ok=1;
	}

	frame++;
}

void showZombies() {
	clearLCD();
	for(int i = 0; i < 10; i++) {
		putch(actorOfTheGame.PvZzombies[i].place.posx, actorOfTheGame.PvZzombies[i].place.posy, 'Z');
	}
}

void moveCursor(uint8_t x, uint8_t y){
	if(x<0||y<0||x>=20||y>=4)
			return;
	cursor_changed =  1;
	cursorPos[1] = y;
	cursorPos[0] = x;
}
void ui_move_cursor_up_down(uint8_t upOrDown){
	if(upOrDown){
		moveCursor(cursorPos[0], cursorPos[1]+1);
	}
	else{
		moveCursor(cursorPos[0], cursorPos[1]-1);
	}
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
