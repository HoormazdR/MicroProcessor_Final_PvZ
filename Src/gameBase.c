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
int score = 0;
int plant_mode1_timer = CON_PLANT_POTATO_RESPAWN_TIME;
int plant_mode2_timer = CON_PLANT_ROZ_RESPAWN_TIME;
int plant_mode3_timer = CON_PLANT_VENUS_RESPAWN_TIME;
int GameState = 0;



void deletePlant (struct plant array[], int position, int size) {
	for (int c = position; c < size - 1; c++)
	         array[c] = array[c+1];
	exist_plant--;
}

void deleteZombie (struct zombie array[], int position, int size) {
	array[position].isDead = 1;
}

void looseHealth (struct zombie z[], int i) {
	if(z[i].place.posy >= 4) {
		deleteZombie(z, i, 22);
		health--;
		last_chance_time = 0;
		if(health <= 0)
			GameState = STE_LOOSE;
	}
}

int checkZombieEat(int i) {
	for(int j = 0; j < exist_plant; j++) {
		struct plant p = actorOfTheGame.PvZPlants[j];
		struct zombie z = actorOfTheGame.PvZzombies[i];
		if(p.place.posy == z.place.posy + 1 && p.place.posx == z.place.posx) {
			p.health = p.health - z.power;
			z.health -= p.power;
			if(z.health <= 0)
				deleteZombie(actorOfTheGame.PvZzombies, i, 22);
			if(p.health <= 0)
				deletePlant(actorOfTheGame.PvZPlants, j, 50);
			return 1;
		}
	}
	return 0;
}

//makes new zombie
struct zombie initZombie(struct zombie z,uint8_t i, enum ZombiesType type){
	if(type == MOZTAFA)
		z.power = CON_ZOMBIE_MOZTAFA_POWER;
	else if (type == JAVADI)
		z.power = CON_ZOMBIE_JAVADI_POWER;
	else if (type == MAMAD)
		z.power = CON_ZOMBIE_MAMAD_POWER;
	else
		z.power = CON_ZOMBIE_ADELAPT_POWER;

	z.health = CON_ZOMBIE_BASE_HEALTH;
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
			actorOfTheGame.PvZzombies[i] = initZombie(actorOfTheGame.PvZzombies[i], rand() % 20, MOZTAFA);
			return;
		}
	}
}

void updateZomiesMove() {
	int chance = rand() % 10 + 1;

	if(zombie_alive < 5 && chance > 6) {
		addZombie();
	}

	for(int i = 0; i < CON_ZOMBIE_COUNT_INITIAL + (CON_ZOMBIE_COUNT_INCREASE_PER_LAP * (level - 1)); i++) {
		if(!checkZombieEat(i) && actorOfTheGame.PvZzombies[i].isDead == 0 && actorOfTheGame.PvZzombies[i].isInitial == 1) {
			actorOfTheGame.PvZzombies[i].TimeCounter = actorOfTheGame.PvZzombies[i].TimeCounter - 1000;
			if(actorOfTheGame.PvZzombies[i].TimeCounter <= 0)
			{
				actorOfTheGame.PvZzombies[i].place.posy++;
				actorOfTheGame.PvZzombies[i].TimeCounter = CON_ZOMBIE_STEP_INTIAL;
				looseHealth(actorOfTheGame.PvZzombies, i);
			}
		}
	}

	refresh_ui();
}

void timer_addation () {
	time_sys += 1;
	if(CHECK_STATE(GameState, STE_TYPE_GAME)) {
		time_game++;
		last_chance_time++;
	}
}

void updatePlantCoolDown() {
	if(plant_mode1_timer > 0)
		plant_mode1_timer--;
	if(plant_mode2_timer > 0)
		plant_mode2_timer--;
	if(plant_mode3_timer > 0)
		plant_mode3_timer--;
}

void update_time() {
	timer_addation();

	if(GameState == STE_NORMAL_GAME) {
		updateZomiesMove();
		updatePlantCoolDown();
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
	else if (type == Roze)
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
	GameState = STE_NORMAL_GAME;
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

	actorOfTheGame.PvZPlants[0] = initPlant(actorOfTheGame.PvZPlants[0], 17, 2, Potato);
	exist_plant++;

}

void calculateScore () {
	score = level * (time_game + last_chance_time);
}
