#include "main.h"
#include "stm32f3xx_hal.h"

#include "gameBase.h"
#include "LiquidCrystal.h"

#include "ui.h"

struct zombies {
	uint8_t health;
	uint8_t power;
	int TimeCounter;
	enum ZombiesType type;
};

//Variables
int cursorX = 0;
int cursorY = 0;

struct Point {
	uint8_t posx;
	uint8_t posy;
};

int shouldZombieMove(struct zombies z) {
	if(z.TimeCounter <= 0)
		return 1;
	else
		return 0;
}



struct zombies initZombie(struct zombies z, enum ZombiesType type){
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

	return z;
}
