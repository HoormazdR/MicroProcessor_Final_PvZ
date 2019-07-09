#include "main.h"
#include "stm32f3xx_hal.h"
#include "ui.h"

extern enum State{
	GAME,
	MENU,
	ENTER_NAME
};

extern enum State state;

void keypadController(uint8_t row, uint8_t col){
	if(state == ENTER_NAME){
		if(row > 1)
			mobileKeypad(col - 1, row - 2);
	}
}

char selectedChar = 'a';
uint8_t clickTimes = 0;
uint8_t pre_row = 0;
uint8_t pre_col = 0;
void mobileKeypad(uint8_t row, uint8_t col){
	if(pre_row != row || pre_col != col)
		selectedChar = 'a';
	selectedChar += row*12 + col*3;
	ui_enterName_putchar(selectedChar);
	selectedChar++;
	clickTimes++;
	if(clickTimes > 2){
		clickTimes = 0;
		selectedChar -= 3;
	}
}
