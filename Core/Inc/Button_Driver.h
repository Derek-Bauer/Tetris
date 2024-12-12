/*
* Button_Driver.h
*
*  Created on: Sep 24, 2024
*      Author: derek
*/
#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_


#include "stm32f4xx_hal.h"
#include <InterruptControl.h>

#define BUTTON_PORT 0
#define BUTTON_PIN_NUM 0
#define BUTTON_ON 1
#define BUTTON_OFF 0

void BUTTON_Init();
void BUTTON_Clk_Enable();
bool BUTTON_Pressed();
void buttonInterruptInit();

#endif /* BUTTON_DRIVER_H_ */
