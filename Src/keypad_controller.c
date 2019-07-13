#include "main.h"
#include "stm32f3xx_hal.h"
#include "ui.h"
#include "gameBase.h"

char selectedChar = 'a';
uint8_t clickTimes = 0;
uint8_t pre_row = 0;
uint8_t pre_col = 0;
uint8_t col_num = 1;
uint8_t debunc_counter = 0;
uint8_t pos = 0;
uint32_t keypad_lastClick_tick  = 1;
uint16_t pre_GPIO_PIN = 0;
uint16_t pre_potan = 0;
extern uint16_t potanLightRand[3];



void keypad_clicked(uint8_t row ,uint8_t col){
	char a[30];
	sprintf(&a, "Keypad Clicked Row: %d, Col: %d \n", row, col);
	log(a);
	keypadController(row, col);
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

void gameKeypad(uint8_t row, uint8_t col){
	if (row == 1 && col == 2) {
		ui_move_cursor_up_down(0);
	} else if (row == 2 && col == 2) {
		ui_move_cursor_up_down(1);
	}
	else if (row == 1 && col == 1 && plant_mode1_timer == 0) {
		actorOfTheGame.PvZPlants[exist_plant] = initPlant(actorOfTheGame.PvZPlants[exist_plant], cursorPos[0], cursorPos[1], Potato);
		exist_plant++;
		plant_mode1_timer = CON_PLANT_POTATO_RESPAWN_TIME;
	}
	else if (row == 2 && col == 1 && plant_mode2_timer == 0) {
		actorOfTheGame.PvZPlants[exist_plant] = initPlant(actorOfTheGame.PvZPlants[exist_plant], cursorPos[0], cursorPos[1], Rose);
		exist_plant++;
		plant_mode2_timer = CON_PLANT_ROZ_RESPAWN_TIME;
	}
	else if (row == 3 && col == 1 && plant_mode3_timer == 0) {
		actorOfTheGame.PvZPlants[exist_plant] = initPlant(actorOfTheGame.PvZPlants[exist_plant], cursorPos[0], cursorPos[1], Venus);
		exist_plant++;
		plant_mode3_timer = CON_PLANT_VENUS_RESPAWN_TIME;
	}
}

void keypadController(uint8_t row, uint8_t col){
	if(GameState == STE_ENTER_NAME){
		if(row > 1)
			mobileKeypad(col - 1, row - 2);
	}
	else if(GameState == STE_NORMAL_GAME){
		gameKeypad(row, col);
	}
}

int HW_VOLOME_MIN = 2;
int HW_VOLOME_MAX = 63;
void potan_controller(){
	uint16_t potan = potanLightRand[0];

	if (potan < HW_VOLOME_MIN)
		HW_VOLOME_MIN = potan;
	if (potan > HW_VOLOME_MAX)
		HW_VOLOME_MAX = potan;

	if (CHECK_STATE(GameState, STE_TYPE_GAME)) {
		potan = (potan - HW_VOLOME_MIN) * (CON_LCD_W_CHANGE - 1) / (HW_VOLOME_MAX - HW_VOLOME_MIN);

		if (cursorPos[0] > potan) {
			log("\nleft\n");
			ui_move_cursor_left_right(0);
		} else if (cursorPos[0] < potan) {
			log("\nright\n");
			ui_move_cursor_left_right(1);
		}
	}

}
