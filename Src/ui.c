#include "main.h"
#include "stm32f3xx_hal.h"
#include <string.h>

#include "ui.h"
#include "LiquidCrystal.h"

#include "gameBase.h"

//the main state of whole game
unsigned char enemyType1[] = {
		0x00, 0x0E, 0x0E, 0x1F, 0x04, 0x04, 0x0E, 0x0A
};

unsigned char enemyType1_frame2[] = {
		0x00, 0x0E, 0x0E, 0x04, 0x1F, 0x04, 0x0E, 0x0A
};

unsigned char plantType1[] = {
		 0x15, 0x15, 0x0E, 0x04, 0x15, 0x1F, 0x0E, 0x04
};

unsigned char plantType2[] = {
		 0x00, 0x0A, 0x11, 0x15, 0x0E, 0x0E, 0x0E, 0x0E
};

unsigned char plantType3[] = {
		0x00, 0x0E, 0x0E, 0x1F, 0x1F, 0x1F, 0x1F, 0x0E
};

// current lcd frame
uint8_t lcd[4][20];

// last lcd frame
uint8_t lcd_last[4][20];
uint8_t cursor_blink_flg = 0;
uint8_t cursor_changed = 0;
uint8_t preCursorPos[2] = {0};

//uart log

extern uint16_t potanLightRand[3];
extern UART_HandleTypeDef huart3;




void log(char str[]){
	int size = strlen(str);
	HAL_UART_Transmit(&huart3, str, size, 1000);
}
void log_adc(){
	  char whatToTransfare[40];
	  sprintf(&whatToTransfare, "Rand: %4d \n"
			  , potanLightRand[2]);
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

void cursorUpdate() {
	putch(cursorPos[0], cursorPos[1], '_');
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

int digit_7seg=0;
void refresh_7seg() {

	// game time to current ragham
	int t = time_game;
	int ragham;
	if(digit_7seg==3)
		ragham=health;
	if(digit_7seg==2)
		ragham=t/100%10;
	if(digit_7seg==1)
		ragham=t/10%10;
	if(digit_7seg==0)
		ragham=t%10;

	HAL_GPIO_WritePin(SSEG_dot_GPIO_Port, SSEG_dot_Pin, 1);
	if(digit_7seg==3)
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

long frame = 0;
long frame_starttime=0;

void changeState(int toState){


	// change ui
	GameState=toState;

	// change ui things
	frame=0;
	frame_starttime=time_sys;
}

void screen_normal_game() {
	if(frame==0) {
		 createChar(0, enemyType1);
		 createChar(1, enemyType1_frame2);
		 createChar(2, plantType1);
		 createChar(3, plantType2);
		 createChar(4, plantType3);
	}
	char enemy = 0;
	char enemy_f2 = 1;
	char plant_Type1 = 2;
	char plant_Type2 = 3;
	char plant_Type3 = 4;

	clearLCD();
	for(int i = 0; i < CON_ZOMBIE_SIZE; i++) {
		if(frame%20 >= 0 && frame%20 <= 10 && actorOfTheGame.PvZzombies[i].isDead==0 && actorOfTheGame.PvZzombies[i].isInitial==1)
			putch(actorOfTheGame.PvZzombies[i].place.posx, actorOfTheGame.PvZzombies[i].place.posy, enemy);
		else if(frame%20 >= 0 && frame%20 <= 10 && actorOfTheGame.PvZzombies[i].isDead==1 && actorOfTheGame.PvZzombies[i].isInitial==1)
			putch(actorOfTheGame.PvZzombies[i].place.posx, actorOfTheGame.PvZzombies[i].place.posy, ' ');
		if (frame%20 >= 11 && frame%20 < 20  && actorOfTheGame.PvZzombies[i].isDead==0 && actorOfTheGame.PvZzombies[i].isInitial==1)
			putch(actorOfTheGame.PvZzombies[i].place.posx, actorOfTheGame.PvZzombies[i].place.posy, enemy_f2);
		else if(frame%20 >= 11 && frame%20 < 20 && actorOfTheGame.PvZzombies[i].isDead==1 && actorOfTheGame.PvZzombies[i].isInitial==1)
					putch(actorOfTheGame.PvZzombies[i].place.posx, actorOfTheGame.PvZzombies[i].place.posy, ' ');
	}

	for(int i = 0; i < exist_plant; i++) {
		if(actorOfTheGame.PvZPlants[i].type == Rose)
			putch(actorOfTheGame.PvZPlants[i].place.posx, actorOfTheGame.PvZPlants[i].place.posy,  plant_Type1);
		else if (actorOfTheGame.PvZPlants[i].type == Venus)
			putch(actorOfTheGame.PvZPlants[i].place.posx, actorOfTheGame.PvZPlants[i].place.posy,  plant_Type2);
		else if (actorOfTheGame.PvZPlants[i].type == Potato)
			putch(actorOfTheGame.PvZPlants[i].place.posx, actorOfTheGame.PvZPlants[i].place.posy,  plant_Type3);

	}

	if(frame%20 >= 0 && frame%20 <= 10 )
		cursorUpdate();
}


/*
 * @story: this function
 */

void moveCursor(uint8_t x, uint8_t y){
	if(x<0||y<1||x>=20||y>=4)
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

void ui_move_cursor_left_right(uint8_t leftOrRight){

	if(leftOrRight){
		moveCursor(cursorPos[0]+1, cursorPos[1]);
	}
	else{
		moveCursor(cursorPos[0]-1, cursorPos[1]);
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

void ui_loose_screen() {
	clearLCD();
	char a[10];
	sprintf(a, "You Score : %d", score);
	putstr(5, 1, "Game Over !");
	putstr(3, 2, a);
}

void ui_win_screen() {
	char a[10];
	sprintf(a, "Up %d", level);
	clearLCD();
	if(frame%2==0)
		putstr(6,1, "Level");
	else if(frame%2==1)
		putstr(12,1, a);

	if(frame > 25)
		changeState(STE_NORMAL_GAME);
}



void refresh_ui(void) {
	int ok;
	// normal game
	if (GameState == STE_NORMAL_GAME) {
		screen_normal_game();
		ok=1;
	}

	if (GameState == STE_ENTER_NAME) {
		ui_enterNameInit();
		ok=1;

	}

	if(GameState == STE_LOOSE) {
		ui_loose_screen();
		ok=1;
	}

	if(GameState == STE_WIN) {
		ui_win_screen();
		ok=1;
	}

	frame++;
}
