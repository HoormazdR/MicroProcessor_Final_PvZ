/*
 * gameBase.h
 *
 *  Created on: Jul 8, 2019
 *      Author: MSI
 */

#ifndef GAMEBASE_H_
#define GAMEBASE_H_

// Constants
#define CON_TOTAL_LEVEL 8
#define CON_LAP_TIME 20000 // ms
#define CON_LCD_W 20
#define CON_LCD_H 4
#define CON_HEALTH 5
#define CON_ENEMY_COUNT_INITIAL 8
#define CON_ENEMY_COUNT_INCREASE_PER_LAP 1
#define CON_ENEMY_MAX_SCREEN_SIZE 5
#define CON_ENEMY_MAX (CON_ENEMY_COUNT_INITIAL + CON_ENEMY_COUNT_INCREASE_PER_LAP*CON_TOTAL_LEVEL)
#define CON_ENEMY_STEP_INTIAL 2000 // ms
#define CON_ENEMY_STEP_DECREASE_PER_LAP 1000/CON_TOTAL_LEVEL // ms (start from 1.5s -> 0.5s)
#define CON_PLANT_POTATO_RESPAWN_TIME 4
#define CON_PLANT_ROZ_RESPAWN_TIME 8
#define CON_PLANT_VENUS_RESPAWN_TIME 10
#define CON_PLANT_POTATO_EAT_POINT 1
#define CON_PLANT_ROZ_EAT_POINT 2
#define CON_PLANT_VENUS_EAT_POINT 4
#define CON_ZOMBIE_MOZTAFA_POWER 1
#define CON_ZOMBIE_JAVADI_POWER 2
#define CON_ZOMBIE_MAMAD_POWER 3
#define CON_ZOMBIE_ADELAPT_POWER 4
#define CON_ZOMBIE_BASE_HEALTH 5

#define STE_TYPE_POPUP          0x01
#define STE_TYPE_GAME           0x02
#define STE_TYPE_CONFRIM_POPUP  0x03

#define STE_READY_TO_PLAY  0x0A00 + STE_TYPE_CONFRIM_POPUP
#define STE_START          0x0100 + STE_TYPE_CONFRIM_POPUP
#define STE_LOOSE          0x0200 + STE_TYPE_CONFRIM_POPUP
#define STE_WIN            0x0300 + STE_TYPE_CONFRIM_POPUP
#define STE_CONTINUE       0x0400 + STE_TYPE_CONFRIM_POPUP
#define STE_END            0x0500 + STE_TYPE_POPUP
#define STE_LEVEL_UP       0x0600 + STE_TYPE_POPUP
#define STE_NORMAL_GAME    0x0700 + STE_TYPE_GAME

enum ZombiesType {MOZTAFA, JAVADI, MAMAD, ADELAPT};

// Variables
extern int cursorX;
extern int cursorY;

#endif /* GAMEBASE_H_ */
