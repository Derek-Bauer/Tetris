/*
* Button_Driver.c
*
*  Created on: Sep 24, 2024
*      Author: derek
*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"


#define ACTIVE 1

void BUTTON_Clk_Enable(){
	__HAL_RCC_GPIOA_CLK_ENABLE();
};

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
HAL_StatusTypeDef HAL_GPIO_LockPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

bool BUTTON_Pressed(){
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == ACTIVE){
		return 1;
	}
	else{
		return 0;
	}
};



void BUTTON_Init(){
	GPIO_InitTypeDef pin_config = {0};
				pin_config.Pin = GPIO_PIN_0;
				pin_config.Mode = GPIO_MODE_INPUT;
				pin_config.Speed = GPIO_SPEED_FREQ_HIGH;
				pin_config.Pull = GPIO_NOPULL;
			BUTTON_Clk_Enable();
			HAL_GPIO_Init(GPIOA, &pin_config);
};

void buttonInterruptInit(){
	GPIO_InitTypeDef pin_config = {0};
					pin_config.Pin = GPIO_PIN_0;
					pin_config.Mode = GPIO_MODE_IT_RISING;
					pin_config.Speed = GPIO_SPEED_FREQ_HIGH;
					pin_config.Pull = GPIO_NOPULL;
	BUTTON_Clk_Enable();
	HAL_GPIO_Init(GPIOA, &pin_config);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

};
