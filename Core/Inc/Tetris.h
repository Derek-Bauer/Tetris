#include "stm32f4xx_hal.h"
#include "LCD_Driver.h"
#include "STM32F429i.h"
#include "Timer_Driver.h"
#include "Scheduler.h"



//////////////////////////////////////////////////////
// MACROS

#define HERO_0 0
#define HERO_1 1
#define ORANGE_RICKY_0 2
#define ORANGE_RICKY_1 3
#define ORANGE_RICKY_2 4
#define ORANGE_RICKY_3 5
#define BLUE_RICKY_0 6
#define BLUE_RICKY_1 7
#define BLUE_RICKY_2 8
#define BLUE_RICKY_3 9
#define CLEVELAND_Z_0 10
#define CLEVELAND_Z_1 11
#define RHODE_ISLAND_Z_0 12
#define RHODE_ISLAND_Z_1 13
#define TEEWEE_0 14
#define TEEWEE_1 15
#define TEEWEE_2 16
#define TEEWEE_3 17
#define SMASHBOY_0 18

#define GAME_STATE_RESET 0
#define GAME_STATE_MENU 1
#define GAME_STATE_PLAY 2
#define GAME_STATE_END 3


//////////////////////////////////////////////////////////////
// GAME STATE
void setGameState(uint16_t newState);
uint16_t getGameState(void);
void playGame(void);
void mainMenu(void);
void youLoseIdiot(void);


////////////////////////////////////////////////////////////////////
// GAME LOGIC
void updateTick(void);
bool moveBlockDown(uint16_t blockShape, uint16_t x, uint16_t y);
bool canMoveBlockSideways(void);
void restingBlockMatrixInit(void);
void generateFallingBlock(uint16_t randomNumber);
void drawBlock(uint16_t block, uint16_t x, uint16_t y);
void clearBlock(uint16_t block, uint16_t x, uint16_t y);
void putBlockIntoFallingMatrix(uint16_t randomNumber, uint16_t fallingBlockX, uint16_t fallingBlockY);
void clearFallingBlockMatrix(void);
void printSec(void);
uint16_t getTimerRollover(void);
void addToTimerRollover(void);

////////////////////////////////////////////////////////////////////////////
// DRAW BLOCKS
void drawHero0(uint16_t x, uint16_t y);
void drawHero1(uint16_t x, uint16_t y);

void drawOrangeRicky0(uint16_t x, uint16_t y);
void drawOrangeRicky1(uint16_t x, uint16_t y);
void drawOrangeRicky2(uint16_t x, uint16_t y);
void drawOrangeRicky3(uint16_t x, uint16_t y);

void drawBlueRicky0(uint16_t x, uint16_t y);
void drawBlueRicky1(uint16_t x, uint16_t y);
void drawBlueRicky2(uint16_t x, uint16_t y);
void drawBlueRicky3(uint16_t x, uint16_t y);

void drawClevelandZ0(uint16_t x, uint16_t y);
void drawClevelandZ1(uint16_t x, uint16_t y);

void drawRhodeIslandZ0(uint16_t x, uint16_t y);
void drawRhodeIslandZ1(uint16_t x, uint16_t y);

void drawTeewee0(uint16_t x, uint16_t y);
void drawTeewee1(uint16_t x, uint16_t y);
void drawTeewee2(uint16_t x, uint16_t y);
void drawTeewee3(uint16_t x, uint16_t y);

void drawSmashboy0(uint16_t x, uint16_t y);






void clearHero0(uint16_t x, uint16_t y);
void clearHero1(uint16_t x, uint16_t y);

void clearOrangeRicky0(uint16_t x, uint16_t y);
void clearOrangeRicky1(uint16_t x, uint16_t y);
void clearOrangeRicky2(uint16_t x, uint16_t y);
void clearOrangeRicky3(uint16_t x, uint16_t y);

void clearBlueRicky0(uint16_t x, uint16_t y);
void clearBlueRicky1(uint16_t x, uint16_t y);
void clearBlueRicky2(uint16_t x, uint16_t y);
void clearBlueRicky3(uint16_t x, uint16_t y);

void clearClevelandZ0(uint16_t x, uint16_t y);
void clearClevelandZ1(uint16_t x, uint16_t y);

void clearRhodeIslandZ0(uint16_t x, uint16_t y);
void clearRhodeIslandZ1(uint16_t x, uint16_t y);

void clearTeewee0(uint16_t x, uint16_t y);
void clearTeewee1(uint16_t x, uint16_t y);
void clearTeewee2(uint16_t x, uint16_t y);
void clearTeewee3(uint16_t x, uint16_t y);

void clearSmashboy0(uint16_t x, uint16_t y);

///////////////////////////////////////////////////////////////////////
// ROTATION
void rotateBlock(uint16_t fallingBlock);
void setFallingBlock(uint16_t updateFallingBlock);
void iWantToRotateBlock(void);
uint16_t getFallingBlock(void);
