#include "main.h"
#include "stm32f3xx_hal.h"

#include "gameBase.h"
#include "LiquidCrystal.h"

#include "ui.h"

struct Point {
	uint8_t posx;
	uint8_t posy;
};

struct zombie {
	struct Point place;
	uint8_t health;
	uint8_t power;
	int TimeCounter;
	enum ZombiesType type;
};

//Variables
int cursorX = 0;
int cursorY = 0;
extern struct zombie * zombieGame;

int shouldZombieMove(struct zombie z) {
	if(z.TimeCounter <= 0)
		return 1;
	else
		return 0;
}

struct zombie initZombie(struct zombie z, enum ZombiesType type){
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
	z.place.posx = 0;
	z.place.posy = 0;

	return z;
}

void mainGame () {
    zombieGame = malloc(sizeof(struct zombie));

	for(int i = 0; i < 5; i++) {
		initZombie(zombieGame[i], ADELAPT);
	}
}
