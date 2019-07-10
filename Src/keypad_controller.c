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
uint8_t col_num = 1;
uint8_t debunc_counter = 0;

uint8_t pos = 0;
uint32_t keypad_lastClick_tick  = 1;

uint16_t pre_GPIO_PIN = 0;
void keypad_clicked(uint8_t row ,uint8_t col){
	char a[30];
	sprintf(&a, "Keypad Clicked Row: %d, Col: %d \n", row, col);
	log(a);
//	keypadController(row, col);
}

void keypad_handler()
{

  col_num++;
  if(col_num == 5)
  		  col_num = 0;

  HAL_GPIO_WritePin(KEYPAD_COL1_PORT, KEYPAD_COL1_PIN, col_num == 1);
  HAL_GPIO_WritePin(KEYPAD_COL2_PORT, KEYPAD_COL2_PIN, col_num == 2);
  HAL_GPIO_WritePin(KEYPAD_COL3_PORT, KEYPAD_COL3_PIN, col_num == 3);
  HAL_GPIO_WritePin(KEYPAD_COL4_PORT, KEYPAD_COL4_PIN, col_num == 4);

  uint32_t diff = HAL_GetTick() - keypad_lastClick_tick;
  printf("%ld \n", diff);
  if( diff > 30){
	  pre_GPIO_PIN = 0;
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN_NUMBER){
	keypad_lastClick_tick = HAL_GetTick();

	debunc_counter++;
	if(debunc_counter < 5){
		return;
	}

	debunc_counter = 0;
	if(GPIO_PIN_NUMBER == pre_GPIO_PIN){
		  debunc_counter++;
		  return;
	  }
	  pre_GPIO_PIN = GPIO_PIN_NUMBER;
	  debunc_counter = 0;
	if(GPIO_PIN_NUMBER == 1)
		keypad_clicked(1, col_num);
	else if(GPIO_PIN_NUMBER == 2)
		keypad_clicked(2, col_num);
	else if(GPIO_PIN_NUMBER == 4)
		keypad_clicked(3, col_num);
	else if(GPIO_PIN_NUMBER == 128)
		keypad_clicked(4, col_num);
}


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
