#include "Tetris.h"

//IOC file (APB1/2 prescalers) and SYSCLK main and firmware

// screen is (x,y) = (240, 320) pixels max, and each square is 20 pixels

//////////////////////////////////////////////////
// STATICS
static uint16_t gameState = 0;
static uint16_t fallingBlock = 18;
static bool fallingBlockMatrix[14][17]={0};
static bool currentFallingBlockMatrix[14][17]={0};
static bool futureFallingBlockMatrix[14][17]={0};
static bool restingBlockMatrix[14][17]={0};
static uint16_t fallingBlockX = 0;
static uint16_t fallingBlockY = 0;
static bool isBlockFalling = 0;
static bool incomingBlockRotation = 0;
static uint16_t timerRolloverCount = 0;

void restingBlockMatrixInit(void){
	//put 1s in the sides
	for(int i=0; i<17; i++){
		restingBlockMatrix[0][i]=1;
		restingBlockMatrix[13][i]=1;
	}
	//put 1s in the bottom
	for(int i=0; i<14; i++){
		restingBlockMatrix[i][16]=1;
	}
}




void mainMenu(void){

	HAL_Delay(200);
	LCD_Clear(0, LCD_COLOR_WHITE);

	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_SetFont(&Font16x24);

	LCD_DisplayChar(75,20,'T');
	LCD_DisplayChar(90,20,'E');
	LCD_DisplayChar(105,20,'T');
	LCD_DisplayChar(120,20,'R');
	LCD_DisplayChar(130,20,'I');
	LCD_DisplayChar(140,20,'S');


	drawOrangeRicky0(50, 50);
	drawBlueRicky0(125, 125);
	drawClevelandZ0(135, 75);
	drawRhodeIslandZ0(15, 115);
	drawSmashboy0(60, 150);
	drawHero0(140, 170);
	drawTeewee0(100, 200);

	uint16_t pixel_shift1 = 15;
	uint16_t pixel_shift2 = 17;
	LCD_DisplayChar(30+pixel_shift1, 250, 'P');
	LCD_DisplayChar(45+pixel_shift1, 250, 'r');
	LCD_DisplayChar(55+pixel_shift1, 250, 'e');
	LCD_DisplayChar(68+pixel_shift1, 250, 's');
	LCD_DisplayChar(80+pixel_shift1, 250, 's');

	LCD_DisplayChar(105+pixel_shift1, 250, 'B');
	LCD_DisplayChar(120+pixel_shift1, 250, 'u');
	LCD_DisplayChar(131+pixel_shift1, 250, 't');
	LCD_DisplayChar(140+pixel_shift1, 250, 't');
	LCD_DisplayChar(150+pixel_shift1, 250, 'o');
	LCD_DisplayChar(163+pixel_shift1, 250, 'n');

	LCD_DisplayChar(30+pixel_shift2, 285, 'T');
	LCD_DisplayChar(43+pixel_shift2, 285, 'o');

	LCD_DisplayChar(70+pixel_shift2, 285, 'C');
	LCD_DisplayChar(85+pixel_shift2, 285, 'o');
	LCD_DisplayChar(98+pixel_shift2, 285, 'n');
	LCD_DisplayChar(110+pixel_shift2, 285, 't');
	LCD_DisplayChar(120+pixel_shift2, 285, 'i');
	LCD_DisplayChar(130+pixel_shift2, 285, 'n');
	LCD_DisplayChar(142+pixel_shift2, 285, 'u');
	LCD_DisplayChar(155+pixel_shift2, 285, 'e');

};


void playGame(void){
	LCD_Clear(0, LCD_COLOR_WHITE);
	timerStart(TIM7_0);
	restingBlockMatrixInit();
	while(getGameState() == GAME_STATE_PLAY){
		uint32_t random;
		RNG_HandleTypeDef hrng = {0};
		hrng.Instance = RNG;
		HAL_RNG_Init(&hrng);
		HAL_RNG_GenerateRandomNumber(&hrng, &random);

		uint16_t randomFrom0To18 = random%19;
		setFallingBlock(randomFrom0To18);

		generateFallingBlock(fallingBlock);
		HAL_Delay(1000);
		while(isBlockFalling){
			moveBlockDown(fallingBlock, fallingBlockX, fallingBlockY);
			if(getScheduledEvents() & (1<<4)){
				setGameState(GAME_STATE_END);
				isBlockFalling = false;
			}
			HAL_Delay(1000);
		}

		HAL_Delay(1000);
	}
}

void youLoseIdiot(void){
	LCD_Clear(0, LCD_COLOR_GREY);
	timerStop(TIM7_0);
	printSec();
}

void printSec(){
	uint16_t seconds = ((TIM7_0->CNT) + 0xffff*getTimerRollover())/2200;
	LCD_DisplayChar(34, 150, 'T');
	LCD_DisplayChar(45, 150, 'i');
	LCD_DisplayChar(57, 150, 'm');
	LCD_DisplayChar(71, 150, 'e');
	LCD_DisplayChar(80, 150, ':');

	LCD_DisplayChar(100, 150, '(');
	LCD_DisplayChar(110, 150, 's');
	LCD_DisplayChar(120, 150, 'e');
	LCD_DisplayChar(130, 150, 'c');
	LCD_DisplayChar(140, 150, 'o');
	LCD_DisplayChar(150, 150, 'n');
	LCD_DisplayChar(160, 150, 'd');
	LCD_DisplayChar(170, 150, 's');
	LCD_DisplayChar(180, 150, ')');

	char seconds_singlesPlace = seconds%10 + '0';
	char seconds_tensPlace = (seconds/10)%10 + '0';
	char seconds_hundredsPlace = (seconds/100)%10+'0';

	LCD_DisplayChar(100, 180, seconds_singlesPlace);
	LCD_DisplayChar(85, 180, seconds_tensPlace);
	LCD_DisplayChar(70, 180, seconds_hundredsPlace);
}


void updateTick(){

}


void setGameState(uint16_t newState){
	gameState = newState;
};

uint16_t getGameState(void){
	return gameState;
}


void iWantToRotateBlock(void){
	incomingBlockRotation = 1;
}

void rotateBlock(uint16_t fallingBlock){

	switch(fallingBlock){
		case HERO_0:
			setFallingBlock(HERO_1);
			clearBlock(HERO_0, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(HERO_1, fallingBlockX, fallingBlockY);
			incomingBlockRotation = 0;
			break;
		case HERO_1:
			setFallingBlock(HERO_0);
			clearBlock(HERO_1, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(HERO_0, fallingBlockX, fallingBlockY);
			incomingBlockRotation = 0;
			break;
		case ORANGE_RICKY_0:
			setFallingBlock(ORANGE_RICKY_1);
			clearBlock(ORANGE_RICKY_0, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(ORANGE_RICKY_1, fallingBlockX, fallingBlockY);
			incomingBlockRotation = 0;
			break;
		case ORANGE_RICKY_1:
			setFallingBlock(ORANGE_RICKY_2);
			clearBlock(ORANGE_RICKY_1, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(ORANGE_RICKY_2, fallingBlockX, fallingBlockY);
			incomingBlockRotation = 0;
			break;
		case ORANGE_RICKY_2:
			setFallingBlock(ORANGE_RICKY_3);
			clearBlock(ORANGE_RICKY_2, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(ORANGE_RICKY_3, fallingBlockX, fallingBlockY);
			incomingBlockRotation = 0;
			break;
		case ORANGE_RICKY_3:
			setFallingBlock(ORANGE_RICKY_0);
			clearBlock(ORANGE_RICKY_3, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(ORANGE_RICKY_0, fallingBlockX, fallingBlockY);
			incomingBlockRotation = 0;
			break;
		case BLUE_RICKY_0:
			setFallingBlock(BLUE_RICKY_1);
			clearBlock(BLUE_RICKY_0, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(BLUE_RICKY_1, fallingBlockX, fallingBlockY);
			incomingBlockRotation = 0;
			break;
		case BLUE_RICKY_1:
			setFallingBlock(BLUE_RICKY_2);
			clearBlock(BLUE_RICKY_1, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(BLUE_RICKY_2, fallingBlockX, fallingBlockY);
			incomingBlockRotation = 0;
			break;
		case BLUE_RICKY_2:
			setFallingBlock(BLUE_RICKY_3);
			clearBlock(BLUE_RICKY_2, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(BLUE_RICKY_3, fallingBlockX, fallingBlockY);
			incomingBlockRotation = 0;
			break;
		case BLUE_RICKY_3:
			setFallingBlock(BLUE_RICKY_0);
			clearBlock(BLUE_RICKY_3, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(BLUE_RICKY_0, fallingBlockX, fallingBlockY);
			incomingBlockRotation = 0;
			break;
		case CLEVELAND_Z_0:
			setFallingBlock(CLEVELAND_Z_1);
			clearBlock(CLEVELAND_Z_0, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(CLEVELAND_Z_1, fallingBlockX, fallingBlockY);
			incomingBlockRotation=0;
			break;
		case CLEVELAND_Z_1:
			setFallingBlock(CLEVELAND_Z_0);
			clearBlock(CLEVELAND_Z_1, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(CLEVELAND_Z_0, fallingBlockX, fallingBlockY);
			incomingBlockRotation=0;
			break;
		case RHODE_ISLAND_Z_0:
			setFallingBlock(RHODE_ISLAND_Z_1);
			clearBlock(RHODE_ISLAND_Z_0, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(RHODE_ISLAND_Z_1, fallingBlockX, fallingBlockY);
			incomingBlockRotation=0;
			break;
		case RHODE_ISLAND_Z_1:
			setFallingBlock(RHODE_ISLAND_Z_0);
			clearBlock(RHODE_ISLAND_Z_1, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(RHODE_ISLAND_Z_0, fallingBlockX, fallingBlockY);
			incomingBlockRotation=0;
			break;
		case TEEWEE_0:
			setFallingBlock(TEEWEE_1);
			clearBlock(TEEWEE_0, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(TEEWEE_1, fallingBlockX, fallingBlockY);
			incomingBlockRotation=0;
			break;
		case TEEWEE_1:
			setFallingBlock(TEEWEE_2);
			clearBlock(TEEWEE_1, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(TEEWEE_2, fallingBlockX, fallingBlockY);
			incomingBlockRotation=0;
			break;
		case TEEWEE_2:
			setFallingBlock(TEEWEE_3);
			clearBlock(TEEWEE_2, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(TEEWEE_3, fallingBlockX, fallingBlockY);
			incomingBlockRotation=0;
			break;
		case TEEWEE_3:
			setFallingBlock(TEEWEE_0);
			clearBlock(TEEWEE_3, fallingBlockX, fallingBlockY);
			clearFallingBlockMatrix();
			putBlockIntoFallingMatrix(TEEWEE_0, fallingBlockX, fallingBlockY);
			incomingBlockRotation=0;
			break;
		break;
	}
}


void setFallingBlock(uint16_t updateFallingBlock){
	fallingBlock = updateFallingBlock;
};

uint16_t getFallingBlock(void){
	return fallingBlock;
};

void generateFallingBlock(uint16_t fallingBlock){
	clearFallingBlockMatrix();
	fallingBlockX = 80;
	fallingBlockY = 0;
	drawBlock(fallingBlock, fallingBlockX, fallingBlockY);
	putBlockIntoFallingMatrix(fallingBlock, fallingBlockX, fallingBlockY);
	isBlockFalling = 1;

}

void clearFallingBlockMatrix(void){
	for(int i=0; i<14; i++){
		for(int j=0; j<17; j++){
			fallingBlockMatrix[i][j]=0;
		}
	}
}

void putBlockIntoFallingMatrix(uint16_t randomNumber, uint16_t fallingBlockX, uint16_t fallingBlockY){
	uint16_t matrixX = fallingBlockX/20+1;
	uint16_t matrixY = fallingBlockY/20;
	switch(fallingBlock){
		case HERO_0:
			for(int i=0; i<4; i++){
				fallingBlockMatrix[i+matrixX][matrixY]=1;
			}
			break;
		case HERO_1:
			for(int i=0; i<4; i++){
				fallingBlockMatrix[matrixX][i+matrixY]=1;
			}
			break;
		case ORANGE_RICKY_0:
			fallingBlockMatrix[matrixX][matrixY+1]=1;
			fallingBlockMatrix[matrixX+1][matrixY+1]=1;
			fallingBlockMatrix[matrixX+2][matrixY+1]=1;
			fallingBlockMatrix[matrixX+2][matrixY]=1;
			break;
		case ORANGE_RICKY_1:
			fallingBlockMatrix[matrixX][matrixY]=1;
			fallingBlockMatrix[matrixX][matrixY+1]=1;
			fallingBlockMatrix[matrixX][matrixY+2]=1;
			fallingBlockMatrix[matrixX+1][matrixY+2]=1;
			break;
		case ORANGE_RICKY_2:
			fallingBlockMatrix[matrixX][matrixY]=1;
			fallingBlockMatrix[matrixX+1][matrixY]=1;
			fallingBlockMatrix[matrixX+2][matrixY]=1;
			fallingBlockMatrix[matrixX][matrixY+1]=1;
			break;
		case ORANGE_RICKY_3:
			fallingBlockMatrix[matrixX][matrixY]=1;
			fallingBlockMatrix[matrixX+1][matrixY]=1;
			fallingBlockMatrix[matrixX+1][matrixY+1]=1;
			fallingBlockMatrix[matrixX+1][matrixY+2]=1;
			break;
		case BLUE_RICKY_0:
			fallingBlockMatrix[matrixX][matrixY]=1;
			fallingBlockMatrix[matrixX][matrixY+1]=1;
			fallingBlockMatrix[matrixX+1][matrixY+1]=1;
			fallingBlockMatrix[matrixX+2][matrixY+1]=1;
			break;
		case BLUE_RICKY_1:
			fallingBlockMatrix[matrixX][matrixY]=1;
			fallingBlockMatrix[matrixX+1][matrixY]=1;
			fallingBlockMatrix[matrixX][matrixY+1]=1;
			fallingBlockMatrix[matrixX][matrixY+2]=1;
			break;
		case BLUE_RICKY_2:
			fallingBlockMatrix[matrixX][matrixY]=1;
			fallingBlockMatrix[matrixX+1][matrixY]=1;
			fallingBlockMatrix[matrixX+2][matrixY]=1;
			fallingBlockMatrix[matrixX+2][matrixY+1]=1;
			break;
		case BLUE_RICKY_3:
			fallingBlockMatrix[matrixX+1][matrixY]=1;
			fallingBlockMatrix[matrixX+1][matrixY+1]=1;
			fallingBlockMatrix[matrixX+1][matrixY+2]=1;
			fallingBlockMatrix[matrixX][matrixY+2]=1;
			break;
		case CLEVELAND_Z_0:
			fallingBlockMatrix[matrixX][matrixY]=1;
			fallingBlockMatrix[matrixX+1][matrixY]=1;
			fallingBlockMatrix[matrixX+1][matrixY+1]=1;
			fallingBlockMatrix[matrixX+2][matrixY+1]=1;
			break;
		case CLEVELAND_Z_1:
			fallingBlockMatrix[matrixX+1][matrixY]=1;
			fallingBlockMatrix[matrixX+1][matrixY+1]=1;
			fallingBlockMatrix[matrixX][matrixY+1]=1;
			fallingBlockMatrix[matrixX][matrixY+2]=1;
			break;
		case RHODE_ISLAND_Z_0:
			fallingBlockMatrix[matrixX][matrixY+1]=1;
			fallingBlockMatrix[matrixX+1][matrixY+1]=1;
			fallingBlockMatrix[matrixX+1][matrixY]=1;
			fallingBlockMatrix[matrixX+2][matrixY]=1;
			break;
		case RHODE_ISLAND_Z_1:
			fallingBlockMatrix[matrixX][matrixY]=1;
			fallingBlockMatrix[matrixX][matrixY+1]=1;
			fallingBlockMatrix[matrixX+1][matrixY+1]=1;
			fallingBlockMatrix[matrixX+1][matrixY+2]=1;
			break;
		case TEEWEE_0:
			fallingBlockMatrix[matrixX+1][matrixY]=1;
			fallingBlockMatrix[matrixX][matrixY+1]=1;
			fallingBlockMatrix[matrixX+1][matrixY+1]=1;
			fallingBlockMatrix[matrixX+2][matrixY+1]=1;
			break;
		case TEEWEE_1:
			fallingBlockMatrix[matrixX][matrixY]=1;
			fallingBlockMatrix[matrixX][matrixY+1]=1;
			fallingBlockMatrix[matrixX][matrixY+2]=1;
			fallingBlockMatrix[matrixX+1][matrixY+1]=1;
			break;
		case TEEWEE_2:
			fallingBlockMatrix[matrixX][matrixY]=1;
			fallingBlockMatrix[matrixX+1][matrixY]=1;
			fallingBlockMatrix[matrixX+2][matrixY]=1;
			fallingBlockMatrix[matrixX+1][matrixY+1]=1;
			break;
		case TEEWEE_3:
			fallingBlockMatrix[matrixX][matrixY+1]=1;
			fallingBlockMatrix[matrixX+1][matrixY]=1;
			fallingBlockMatrix[matrixX+1][matrixY+1]=1;
			fallingBlockMatrix[matrixX+1][matrixY+2]=1;
			break;
		case SMASHBOY_0:
			fallingBlockMatrix[matrixX][matrixY]=1;
			fallingBlockMatrix[matrixX+1][matrixY]=1;
			fallingBlockMatrix[matrixX][matrixY+1]=1;
			fallingBlockMatrix[matrixX+1][matrixY+1]=1;
		break;
	}
};


// moves falling block down, if possible. If not, it updates the restingBlockMatrix and returns false.
bool moveBlockDown(uint16_t blockShape, uint16_t x, uint16_t y){

	 // do a rotation first, if one is requested
	if(incomingBlockRotation){
					// write fcts that update the matrices and fallingBlock, then draw the new block
					rotateBlock(fallingBlock);
					//
					// drawBlock(fallingBlock, fallingBlockX, fallingBlockY);
				}

	for(int i=0; i<14; i++){
		for(int j=0; j<17; j++){
			currentFallingBlockMatrix [i][j]= fallingBlockMatrix[i][j];
		}
	}



	//copy row onto one below it, starting at bottom
	for(int i=15; i>-1; i--){
		for(int j=1; j<14; j++){
			if(currentFallingBlockMatrix[j][i] == 1){
				futureFallingBlockMatrix[j][i+1]=1;
			}
			else{
				futureFallingBlockMatrix[j][i+1]=0;
			}
//			if(i==0){
//				printf("Test");
//			}
//			if(i==1){
//				printf("Test");
//			}
//			if(i==2){
//				printf("Test");
//			}
		}
	}
	//fill top row with 0s
	for(int k=1; k<14; k++){
		futureFallingBlockMatrix[k][0]=0;
	}


	//check if we hit anything in the resting block matrix
	bool weHitSomething = false;
	for(int i=1; i<14; i++){
		for(int j=0; j<17; j++){
			if( (restingBlockMatrix[i][j]==1) && (futureFallingBlockMatrix[i][j]==1) ){
				weHitSomething = true;
			}
		}
	}
//	for(int i=0; i<14; i++){
//		printf("%d", restingBlockMatrix[i][16]);
//		printf("\n");
//		printf("%d", currentFallingBlockMatrix[i][16]);
//	}

	if(weHitSomething){
		// move current position of falling block into restingBlockMatrix
		for(int i=0; i<14; i++){
			for(int j=0; j<17; j++){
				if(currentFallingBlockMatrix[i][j]==1){
					restingBlockMatrix[i][j]=1;
				}
			}
		}
		isBlockFalling = 0;
		bool youLose = false;
		for(int i=1; i<13; i++){
			if(restingBlockMatrix[i][0]==1){
				youLose=true;
			}
		}
		if(youLose){
			addSchedulerEvent(END_THE_GAME);

		}
		return false;
	}
	else{
		// move block down one
		for(int i=0; i<14; i++){
				for(int j=0; j<17; j++){
					currentFallingBlockMatrix[i][j]= futureFallingBlockMatrix[i][j];
					fallingBlockMatrix[i][j]= futureFallingBlockMatrix[i][j];
					if(i>5 && i<10){
						printf("Hello World!");
					}
				}
		}
		// clear old block and print new one
		clearBlock(fallingBlock, fallingBlockX, fallingBlockY);
		drawBlock(fallingBlock, fallingBlockX, fallingBlockY + 20);
		fallingBlockY = fallingBlockY + 20;
		return true;
	}
};

//bool canMoveBlockSideways(void){
//	bool currentFallingBlockMatrix[14][17]=fallingBlockMatrix;
//	bool futureFallingBlockMatrix[14][17]={0};
//	for(int i=15;i>=0; i++){
//		for(int j=1; j<14; j++){
//			if(currentFallingBlockMatrix[j][i]=fdsafdsa){
//
//			}
//		}
//	}
//};








/////////////////////////////////////////////////////////////////////////////
// DRAW/CLEAR BLOCKS

void drawBlock(uint16_t block, uint16_t x, uint16_t y){
	switch(block){
			case HERO_0:
				drawHero0(x,y);
				break;
			case HERO_1:
				drawHero1(x,y);
				break;
			case ORANGE_RICKY_0:
				drawOrangeRicky0(x,y);
				break;
			case ORANGE_RICKY_1:
				drawOrangeRicky1(x,y);
				break;
			case ORANGE_RICKY_2:
				drawOrangeRicky2(x,y);
				break;
			case ORANGE_RICKY_3:
				drawOrangeRicky3(x,y);
				break;
			case BLUE_RICKY_0:
				drawBlueRicky0(x,y);
				break;
			case BLUE_RICKY_1:
				drawBlueRicky1(x,y);
				break;
			case BLUE_RICKY_2:
				drawBlueRicky2(x,y);
				break;
			case BLUE_RICKY_3:
				drawBlueRicky3(x,y);
				break;
			case CLEVELAND_Z_0:
				drawClevelandZ0(x,y);
				break;
			case CLEVELAND_Z_1:
				drawClevelandZ1(x,y);
				break;
			case RHODE_ISLAND_Z_0:
				drawRhodeIslandZ0(x,y);
				break;
			case RHODE_ISLAND_Z_1:
				drawRhodeIslandZ1(x,y);
				break;
			case TEEWEE_0:
				drawTeewee0(x,y);
				break;
			case TEEWEE_1:
				drawTeewee1(x,y);
				break;
			case TEEWEE_2:
				drawTeewee2(x,y);
				break;
			case TEEWEE_3:
				drawTeewee3(x,y);
				break;
			case SMASHBOY_0:
				drawSmashboy0(x,y);
			break;
		}
};

void clearBlock(uint16_t block, uint16_t x, uint16_t y){
	switch(block){
			case HERO_0:
				clearHero0(x,y);
				break;
			case HERO_1:
				clearHero1(x,y);
				break;
			case ORANGE_RICKY_0:
				clearOrangeRicky0(x,y);
				break;
			case ORANGE_RICKY_1:
				clearOrangeRicky1(x,y);
				break;
			case ORANGE_RICKY_2:
				clearOrangeRicky2(x,y);
				break;
			case ORANGE_RICKY_3:
				clearOrangeRicky3(x,y);
				break;
			case BLUE_RICKY_0:
				clearBlueRicky0(x,y);
				break;
			case BLUE_RICKY_1:
				clearBlueRicky1(x,y);
				break;
			case BLUE_RICKY_2:
				clearBlueRicky2(x,y);
				break;
			case BLUE_RICKY_3:
				clearBlueRicky3(x,y);
				break;
			case CLEVELAND_Z_0:
				clearClevelandZ0(x,y);
				break;
			case CLEVELAND_Z_1:
				clearClevelandZ1(x,y);
				break;
			case RHODE_ISLAND_Z_0:
				clearRhodeIslandZ0(x,y);
				break;
			case RHODE_ISLAND_Z_1:
				clearRhodeIslandZ1(x,y);
				break;
			case TEEWEE_0:
				clearTeewee0(x,y);
				break;
			case TEEWEE_1:
				clearTeewee1(x,y);
				break;
			case TEEWEE_2:
				clearTeewee2(x,y);
				break;
			case TEEWEE_3:
				clearTeewee3(x,y);
				break;
			case SMASHBOY_0:
				clearSmashboy0(x,y);
				break;
		}
};

void drawHero0(uint16_t x, uint16_t y){
	for(int i=0; i<80; i++){
		LCD_Draw_Vertical_Line(x+i,y,20,LCD_COLOR_MAGENTA);
	}
};

void drawHero1(uint16_t x, uint16_t y){

	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i,y,20,LCD_COLOR_MAGENTA);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i,y+20,20,LCD_COLOR_MAGENTA);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i,y+40,20,LCD_COLOR_MAGENTA);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i,y+60,20,LCD_COLOR_MAGENTA);
	}
};

void drawOrangeRicky0(uint16_t x, uint16_t y){
	for(int i=0; i<60; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_ORANGE);
	}
	for(int i=40; i<60; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_ORANGE);
	}
}

void drawOrangeRicky1(uint16_t x, uint16_t y){
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_ORANGE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_ORANGE);
	}
	for(int i=0; i<40; i++){
		LCD_Draw_Vertical_Line(x+i, y+40, 20, LCD_COLOR_ORANGE);
	}
}

void drawOrangeRicky2(uint16_t x, uint16_t y){
	for(int i=0; i<60; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_ORANGE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_ORANGE);
	}
}

void drawOrangeRicky3(uint16_t x, uint16_t y){
	for(int i=0; i<40; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_ORANGE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i+20, y+20, 40, LCD_COLOR_ORANGE);
	}
}



void drawBlueRicky0(uint16_t x, uint16_t y){
	for(int i=0; i<60; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_BLUE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_BLUE);
	}
}

void drawBlueRicky1(uint16_t x, uint16_t y){
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y, 60, LCD_COLOR_BLUE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i+20, y, 20, LCD_COLOR_BLUE);
	}
}

void drawBlueRicky2(uint16_t x, uint16_t y){
	for(int i=0; i<60; i++){
			LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_BLUE);
		}
	for(int i=0; i<20; i++){
			LCD_Draw_Vertical_Line(x+i+40, y+20, 20, LCD_COLOR_BLUE);
		}
}

void drawBlueRicky3(uint16_t x, uint16_t y){
	for(int i=0; i<20; i++){
			LCD_Draw_Vertical_Line(x+i+20, y, 60, LCD_COLOR_BLUE);
		}
	for(int i=0; i<20; i++){
			LCD_Draw_Vertical_Line(x+i, y+40, 20, LCD_COLOR_BLUE);
		}
}


void drawClevelandZ0(uint16_t x, uint16_t y){
	for(int i=0; i<40; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_GREEN);
	}
	for(int i=20; i<60; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_GREEN);
	}
}

void drawClevelandZ1(uint16_t x, uint16_t y){
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i+20, y, 40, LCD_COLOR_GREEN);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 40, LCD_COLOR_GREEN);
	}
}

void drawRhodeIslandZ0(uint16_t x, uint16_t y){
	for(int i=0; i<40; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_RED);
	}
	for(int i=20; i<60; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_RED);
	}
}

void drawRhodeIslandZ1(uint16_t x, uint16_t y){
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y, 40, LCD_COLOR_RED);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i+20, y+20, 40, LCD_COLOR_RED);
	}
}

void drawTeewee0(uint16_t x, uint16_t y){
	for(int i=0; i<60; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_YELLOW);
	}
	for(int i=20; i<40; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_YELLOW);
	}
}

void drawTeewee1(uint16_t x, uint16_t y){
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y, 60, LCD_COLOR_YELLOW);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i+20, y+20, 20, LCD_COLOR_YELLOW);
	}
}

void drawTeewee2(uint16_t x, uint16_t y){
	for(int i=0; i<60; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_YELLOW);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i+20, y+20, 20, LCD_COLOR_YELLOW);
	}
}

void drawTeewee3(uint16_t x, uint16_t y){
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_YELLOW);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i+20, y, 60, LCD_COLOR_YELLOW);
	}
}

void drawSmashboy0(uint16_t x, uint16_t y){
	for(int i=0; i<40; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_GREY);
	}
	for(int i=0; i<40; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_GREY);
	}
}
















void clearHero0(uint16_t x, uint16_t y){
	for(int i=0; i<80; i++){
		LCD_Draw_Vertical_Line(x+i,y,20,LCD_COLOR_WHITE);
	}
};

void clearHero1(uint16_t x, uint16_t y){

	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i,y,20,LCD_COLOR_WHITE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i,y+20,20,LCD_COLOR_WHITE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i,y+40,20,LCD_COLOR_WHITE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i,y+60,20,LCD_COLOR_WHITE);
	}
};

void clearOrangeRicky0(uint16_t x, uint16_t y){
	for(int i=0; i<60; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_WHITE);
	}
	for(int i=40; i<60; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_WHITE);
	}
}

void clearOrangeRicky1(uint16_t x, uint16_t y){
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_WHITE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_WHITE);
	}
	for(int i=0; i<40; i++){
		LCD_Draw_Vertical_Line(x+i, y+40, 20, LCD_COLOR_WHITE);
	}
}

void clearOrangeRicky2(uint16_t x, uint16_t y){
	for(int i=0; i<60; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_WHITE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_WHITE);
	}
}

void clearOrangeRicky3(uint16_t x, uint16_t y){
	for(int i=0; i<40; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_WHITE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i+20, y+20, 40, LCD_COLOR_WHITE);
	}
}



void clearBlueRicky0(uint16_t x, uint16_t y){
	for(int i=0; i<60; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_WHITE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_WHITE);
	}
}

void clearBlueRicky1(uint16_t x, uint16_t y){
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y, 60, LCD_COLOR_WHITE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i+20, y, 20, LCD_COLOR_WHITE);
	}
}

void clearBlueRicky2(uint16_t x, uint16_t y){
	for(int i=0; i<60; i++){
			LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_WHITE);
		}
	for(int i=0; i<20; i++){
			LCD_Draw_Vertical_Line(x+i+40, y+20, 20, LCD_COLOR_WHITE);
		}
}

void clearBlueRicky3(uint16_t x, uint16_t y){
	for(int i=0; i<20; i++){
			LCD_Draw_Vertical_Line(x+i+20, y, 60, LCD_COLOR_WHITE);
		}
	for(int i=0; i<20; i++){
			LCD_Draw_Vertical_Line(x+i, y+40, 20, LCD_COLOR_WHITE);
		}
}


void clearClevelandZ0(uint16_t x, uint16_t y){
	for(int i=0; i<40; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_WHITE);
	}
	for(int i=20; i<60; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_WHITE);
	}
}

void clearClevelandZ1(uint16_t x, uint16_t y){
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i+20, y, 40, LCD_COLOR_WHITE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 40, LCD_COLOR_WHITE);
	}
}

void clearRhodeIslandZ0(uint16_t x, uint16_t y){
	for(int i=0; i<40; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_WHITE);
	}
	for(int i=20; i<60; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_WHITE);
	}
}

void clearRhodeIslandZ1(uint16_t x, uint16_t y){
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y, 40, LCD_COLOR_WHITE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i+20, y+20, 40, LCD_COLOR_WHITE);
	}
}

void clearTeewee0(uint16_t x, uint16_t y){
	for(int i=0; i<60; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_WHITE);
	}
	for(int i=20; i<40; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_WHITE);
	}
}

void clearTeewee1(uint16_t x, uint16_t y){
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y, 60, LCD_COLOR_WHITE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i+20, y+20, 20, LCD_COLOR_WHITE);
	}
}

void clearTeewee2(uint16_t x, uint16_t y){
	for(int i=0; i<60; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_WHITE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i+20, y+20, 20, LCD_COLOR_WHITE);
	}
}

void clearTeewee3(uint16_t x, uint16_t y){
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_WHITE);
	}
	for(int i=0; i<20; i++){
		LCD_Draw_Vertical_Line(x+i+20, y, 60, LCD_COLOR_WHITE);
	}
}

void clearSmashboy0(uint16_t x, uint16_t y){
	for(int i=0; i<40; i++){
		LCD_Draw_Vertical_Line(x+i, y+20, 20, LCD_COLOR_WHITE);
	}
	for(int i=0; i<40; i++){
		LCD_Draw_Vertical_Line(x+i, y, 20, LCD_COLOR_WHITE);
	}
}


uint16_t getTimerRollover(){
	return timerRolloverCount;
}

void addToTimerRollover(){
	timerRolloverCount++;
}


