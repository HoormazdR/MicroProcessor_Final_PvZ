#include "main.h"
#include "stm32f3xx_hal.h"

#include "gameBase.h"
#include "LiquidCrystal.h"
#include <stdlib.h>

#include "ui.h"

//Variables
int health = 5;
int exist_plant = 0;
int exist_enemy = 0;
int time_sys = 0;
int time_game = 0;
int cursorX = 0;
int cursorY = 0;
struct actor actorOfTheGame;

int looseHealth (uint8_t position) {
	if(position == 4)
		return 1;
	return 0;
}

void deletePlant (struct plant array[], int position, int size) {
	for (int c = position; c < size - 1; c++)
	         array[c] = array[c+1];
}

int checkZombieEat(int i) {
	for(int j = 0; j < 10; j++) {
		if(actorOfTheGame.PvZPlants[j].place.posy == actorOfTheGame.PvZzombies[i].place.posy + 1) {
			actorOfTheGame.PvZPlants[j].power -= actorOfTheGame.PvZzombies[i].power;
			if(actorOfTheGame.PvZPlants[j].power <= 0)
				deletePlant(actorOfTheGame.PvZPlants, j, exist_plant);
			//TODO: Add zombie deletetion
			return 1;
		}
		return 0;
	}
}

//TODO: Need To Change
void updateZomiesMove() {
	for(int i = 0; i < 10; i++) {
		if(!checkZombieEat(i)) {
			actorOfTheGame.PvZzombies[i].TimeCounter = actorOfTheGame.PvZzombies[i].TimeCounter - 1000;
			if(actorOfTheGame.PvZzombies[i].TimeCounter <= 0)
			{
				actorOfTheGame.PvZzombies[i].place.posy++;
				if(looseHealth(actorOfTheGame.PvZzombies[i].place.posy))
					health--;
				if(actorOfTheGame.PvZzombies[i].place.posy >= 4)
					actorOfTheGame.PvZzombies[i].place.posy = 0;
				actorOfTheGame.PvZzombies[i].TimeCounter = CON_ENEMY_STEP_INTIAL;
			}
		}

	}
	refresh_ui();
}

void timer_addation () {
	time_sys += 1;
	time_game += 1;
}

void update_time() {
	timer_addation();
	updateZomiesMove();
}

int shouldZombieMove(struct zombie z) {
	if(z.TimeCounter <= 0)
		return 1;
	else
		return 0;
}

struct zombie initZombie(struct zombie z,uint8_t i, uint8_t j, enum ZombiesType type){
	if(type == MOZTAFA)
		z.power = CON_ZOMBIE_MOZTAFA_POWER;
	else if (type == JAVADI)
		z.power = CON_ZOMBIE_JAVADI_POWER;
	else if (type == MAMAD)
		z.power = CON_ZOMBIE_MAMAD_POWER;
	else
		z.power = CON_ZOMBIE_ADELAPT_POWER;

	z.health = CON_ZOMBIE_BASE_HEALTH;
	z.TimeCounter = CON_ENEMY_STEP_INTIAL;
	z.type = type;
	z.place.posx = i;
	z.place.posy = j;

	return z;
}

struct plant initPLant(struct plant p, int i, int j, enum PlantsType type) {
	if(type == Potato)
		p.power = CON_PLANT_POTATO_EAT_POINT;
	else if (type == Roze)
		p.power = CON_PLANT_ROZ_EAT_POINT;
	else if (type == Venus)
		p.power = CON_PLANT_VENUS_EAT_POINT;

	p.type = type;
	p.place.posx = i;
	p.place.posy = j;

	return p;
}

void mainGame () {

	exist_plant = 1;
	actorOfTheGame.PvZPlants[0] = initPLant(actorOfTheGame.PvZPlants[0], 3, 2, Venus);
	exist_enemy = 5;
	for(int i = 0; i < exist_enemy; i++) {
		actorOfTheGame.PvZzombies[i] = initZombie(actorOfTheGame.PvZzombies[i], i, 0, ADELAPT);
	}
}
