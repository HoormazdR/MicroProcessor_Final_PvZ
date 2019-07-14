#include "main.h"
#include "stm32f3xx_hal.h"

#include "gameBase.h"
#include "LiquidCrystal.h"
#include <stdlib.h>
#include <time.h>

#include "ui.h"

//Variables
int health = 5;
int exist_plant = 0;
int zombie_alive = 0;
int time_sys = 0;
int time_game = 0;
uint8_t cursorPos[2] = {0,3};
struct actor actorOfTheGame;
int level = 1;
int last_chance_time = 0;
int zombie_enter = 0;
int saveData_index = 0;
int load_index =0;
int score = 0;
int plant_mode1_timer = CON_PLANT_POTATO_RESPAWN_TIME;
int plant_mode2_timer = CON_PLANT_ROZ_RESPAWN_TIME;
int plant_mode3_timer = CON_PLANT_VENUS_RESPAWN_TIME;
int GameState = STE_MENU;
int GameState_next = 0;
extern uint16_t potanLightRand[3];
int getReadyPlant = 0;
struct bounes gameBounes[4];
extern UART_HandleTypeDef huart3;

int getRand() {
	return potanLightRand[2]%100;
}

void deletePlant (struct plant array[], int position, int size) {
	for (int c = position; c < size - 1; c++)
	         array[c] = array[c+1];
	exist_plant--;
}

void deleteZombie (struct zombie array[], int position, int size) {
//	for (int c = position; c < size - 1; c++)
//		         array[c] = array[c+1];
	array[position].isDead = 1;
	zombie_alive--;
}

void looseHealth (struct zombie z[], int i) {
	if(z[i].place.posy >= 4) {
		deleteZombie(z, i, 22);
		health--;
		last_chance_time = 0;
//		if(health <= 0)
//			changeState(STE_LOOSE, STE_ENTER_NAME);
	}
}

int checkZombieEat(int i) {
	for(int j = 0; j < exist_plant; j++) {
		struct plant p = actorOfTheGame.PvZPlants[j];
		struct zombie z = actorOfTheGame.PvZzombies[i];
		if(p.place.posy == z.place.posy + 1 && p.place.posx == z.place.posx && z.isDead == 0) {
			actorOfTheGame.PvZPlants[j].health -= actorOfTheGame.PvZzombies[i].power;
			actorOfTheGame.PvZzombies[i].health -= actorOfTheGame.PvZPlants[j].power;
			if(actorOfTheGame.PvZzombies[i].health <= 0)
				deleteZombie(actorOfTheGame.PvZzombies, i, 22);
			if(actorOfTheGame.PvZPlants[j].health <= 0)
				deletePlant(actorOfTheGame.PvZPlants, j, 50);
			return 1;
		}
	}
	return 0;
}

//makes new zombie
struct zombie initZombie(struct zombie z,uint8_t i, int type){
	if(type == 0)
		z.power = CON_ZOMBIE_MOZTAFA_POWER;
	else if (type == 1)
		z.power = CON_ZOMBIE_JAVADI_POWER;
	else if (type == 2)
		z.power = CON_ZOMBIE_MAMAD_POWER;
	else
		z.power = CON_ZOMBIE_ADELAPT_POWER;

	z.health = 1;
	z.TimeCounter = CON_ZOMBIE_STEP_INTIAL - ((level - 1) * CON_ZOMBIE_STEP_DECREASE_PER_LAP);
	z.type = type;
	z.place.posx = i;
	z.place.posy = 0;
	z.isDead = 0;
	z.isInitial = 1;

	return z;
}

void addZombie() {
	for(int i = 0; i < CON_ZOMBIE_COUNT_INITIAL + (CON_ZOMBIE_COUNT_INCREASE_PER_LAP * (level - 1)); i++) {
		if(actorOfTheGame.PvZzombies[i].isInitial == 0)
		{
			actorOfTheGame.PvZzombies[i] = initZombie(actorOfTheGame.PvZzombies[i], getRand() % 24, getRand() % 4);
			zombie_alive++;
			return;
		}
	}
}

void makeNewZombie() {
	for (int i = 0; i < 22; i++) {
		actorOfTheGame.PvZzombies[i].TimeCounter = CON_ZOMBIE_STEP_INTIAL;
		actorOfTheGame.PvZzombies[i].health = CON_ZOMBIE_BASE_HEALTH;
		actorOfTheGame.PvZzombies[i].isDead = 0;
		actorOfTheGame.PvZzombies[i].isInitial = 0;
		actorOfTheGame.PvZzombies[i].place.posx = 0;
		actorOfTheGame.PvZzombies[i].place.posy = 0;
		actorOfTheGame.PvZzombies[i].power = 0;
	}
}

void addBounes() {
	int type = rand() % 3;
	int x,y;

	do {
		x = rand() % 23 + 1;
		y = rand() % 4 + 1;
	} while (checkLCD(x, y) != ' ');

	for (int i = 0; i < 4; i++) {
		if (gameBounes[i].isActive == 0) {
			gameBounes[i].type = type;
			gameBounes[i].p.posx = x;
			gameBounes[i].p.posy = y;
			gameBounes[i].isActive = 1;
			gameBounes[i].timerCounter = 4000;
		}
	}

}

void updateZomiesMove() {
	int chance = getRand() % 10 + 1;

	if(exist_plant > 6)
		getReadyPlant = 0;

	if(zombie_alive < 5 && chance > 3 && !getReadyPlant) {
		addZombie();
	}

	for(int i = 0; i < CON_ZOMBIE_COUNT_INITIAL + (CON_ZOMBIE_COUNT_INCREASE_PER_LAP * (level - 1)); i++) {
		if(actorOfTheGame.PvZzombies[i].isDead == 0 && !checkZombieEat(i) && actorOfTheGame.PvZzombies[i].isInitial == 1) {
			actorOfTheGame.PvZzombies[i].TimeCounter = actorOfTheGame.PvZzombies[i].TimeCounter - 1000;
			if(actorOfTheGame.PvZzombies[i].TimeCounter <= 0)
			{
				actorOfTheGame.PvZzombies[i].place.posy++;
				actorOfTheGame.PvZzombies[i].TimeCounter = CON_ZOMBIE_STEP_INTIAL;
				looseHealth(actorOfTheGame.PvZzombies, i);
			}
		}
	}

	if(time_game%20 == 0 && time_game > 0) {
		level++;
		makeNewZombie();
		changeState(STE_WIN, STE_ENTER_NAME);
	}

}

void calculateScore () {
	score = level * (time_game + last_chance_time);
}

void timer_addation () {
	time_sys += 1;
	if(CHECK_STATE(GameState, STE_TYPE_GAME) && !getReadyPlant) {
		time_game++;
		last_chance_time++;
	}
}

void updatePlantCoolDown() {
	if(plant_mode1_timer > 0)
	{
		plant_mode1_timer--;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
	}
	else if(plant_mode1_timer <= 0)
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
	if(plant_mode2_timer > 0)
	{
		plant_mode2_timer--;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
	}
	else if(plant_mode2_timer <= 0)
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 1);
	if(plant_mode3_timer > 0)
	{
		plant_mode3_timer--;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
	}
	else if(plant_mode3_timer <= 0)
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);
}

void bounesCounter() {
	for(int i=0; i < 4; i++) {
		if(gameBounes[i].isActive == 1) {
			gameBounes[i].timerCounter -= 1000;
			if(gameBounes[i].timerCounter <= 0) {
				gameBounes[i].isActive = 0;
				gameBounes[i].p.posx = 0;
				gameBounes[i].p.posy = 0;
			}
		}
	}
}

void update_time() {
	timer_addation();

	if(GameState == STE_NORMAL_GAME) {
		updateZomiesMove();
		updatePlantCoolDown();
		calculateScore();
		bounesCounter();
		if(rand() % 10 > 6)
			addBounes();
	}

}

int shouldZombieMove(struct zombie z) {
	if(z.TimeCounter <= 0)
		return 1;
	else
		return 0;
}

//makes new plant
struct plant initPlant(struct plant p, int i, int j, enum PlantsType type) {
	if(type == Potato)
		p.power = CON_PLANT_POTATO_EAT_POINT;
	else if (type == Rose)
		p.power = CON_PLANT_ROZ_EAT_POINT;
	else if (type == Venus)
		p.power = CON_PLANT_VENUS_EAT_POINT;

	p.health = p.power;
	p.type = type;
	p.place.posx = i;
	p.place.posy = j;

	return p;
}

//New Game
void initLogic() {
	srand(time(NULL));   // Initialization, should only be called once.

	//State Game
	level = 1;
	score = 0;
	health = CON_HEALTH;
	exist_plant = 0;
	zombie_alive = 0;
	time_sys = 0;
	time_game = 0;
	cursorPos[1] = 3; //from 0 to 3
	cursorPos[0] = 0; //from 0 to 19
	last_chance_time = 0;
	zombie_enter = 0;
	getReadyPlant = 1;
//	for(int i = 0;i< 23;i++)
//	{
//		actorOfTheGame.PvZPlants[i] = initPlant(actorOfTheGame.PvZPlants[i], i, 3, Venus);
//		exist_plant++;
//	}

}

void saveAddData(char saveData[], char c){
	saveData[saveData_index] = c;
	saveData_index++;
}
void saveTheGame(){

	char saveData[1000];
	saveData_index = 0;

	saveAddData(saveData, time_game);
	saveAddData(saveData, health);
	saveAddData(saveData, level);
	saveAddData(saveData, score);
	saveAddData(saveData, plant_mode1_timer);
	saveAddData(saveData, plant_mode2_timer);
	saveAddData(saveData, plant_mode3_timer);
	saveAddData(saveData, zombie_alive);

	for(int i =0 ; i < 22;i++){
		saveAddData(saveData, actorOfTheGame.PvZzombies[i].health);
		saveAddData(saveData, actorOfTheGame.PvZzombies[i].type);
		saveAddData(saveData, actorOfTheGame.PvZzombies[i].place.posx);
		saveAddData(saveData, actorOfTheGame.PvZzombies[i].place.posy);
		saveAddData(saveData, actorOfTheGame.PvZzombies[i].isInitial);
		saveAddData(saveData, actorOfTheGame.PvZzombies[i].isDead);
		saveAddData(saveData, actorOfTheGame.PvZzombies[i].power);
		saveAddData(saveData, actorOfTheGame.PvZzombies[i].TimeCounter);
	}
	saveAddData(saveData, exist_plant);
	for(int i = 0; i < exist_plant; i++){
		saveAddData(saveData, actorOfTheGame.PvZPlants[i].health);
		saveAddData(saveData, actorOfTheGame.PvZPlants[i].type);
		saveAddData(saveData, actorOfTheGame.PvZPlants[i].place.posx);
		saveAddData(saveData, actorOfTheGame.PvZPlants[i].place.posy);
		saveAddData(saveData, actorOfTheGame.PvZPlants[i].power);
	}

	saveAddData(saveData, 0xFF);

	HAL_UART_Transmit(&huart3,saveData, saveData_index, 1000);

	changeState(STE_NORMAL_GAME, STE_END);


}

char loadGetData(char loadData[]){
	char retval = loadData[load_index];
	load_index++;
	return retval;
}
void loadGame(char saveData[]){

	load_index = 0;


	time_game = loadGetData(saveData);
	health = loadGetData(saveData);
	level = loadGetData(saveData);
	score = loadGetData(saveData);
	plant_mode1_timer = loadGetData(saveData);
	plant_mode2_timer = loadGetData(saveData);
	plant_mode3_timer = loadGetData(saveData);
	zombie_alive  = loadGetData(saveData);

	for(int i =0 ; i < 22;i++){
		actorOfTheGame.PvZzombies[i].health = loadGetData(saveData);
		 actorOfTheGame.PvZzombies[i].type = loadGetData(saveData);
		 actorOfTheGame.PvZzombies[i].place.posx = loadGetData(saveData);
		 actorOfTheGame.PvZzombies[i].place.posy = loadGetData(saveData);
		 actorOfTheGame.PvZzombies[i].isInitial = loadGetData(saveData);
		 actorOfTheGame.PvZzombies[i].isDead = loadGetData(saveData);
		 actorOfTheGame.PvZzombies[i].power = loadGetData(saveData);
		 actorOfTheGame.PvZzombies[i].TimeCounter = loadGetData(saveData);
	}

	exist_plant = loadGetData(saveData);
	for(int i = 0; i < exist_plant; i++){
		actorOfTheGame.PvZPlants[i].health = loadGetData(saveData);
		actorOfTheGame.PvZPlants[i].type = loadGetData(saveData);
		actorOfTheGame.PvZPlants[i].place.posx = loadGetData(saveData);
		actorOfTheGame.PvZPlants[i].place.posy = loadGetData(saveData);
		actorOfTheGame.PvZPlants[i].power = loadGetData(saveData);
	}

	changeState(STE_NORMAL_GAME, STE_END);
}
//void loadData_handler(){
//	if(GameState == STE_LOAD){
//		char saveData[100];
//		HAL_UART_Receive(&huart3, saveData, 100, 1000);
//		loadGame(saveData);
//
//	}
//}
