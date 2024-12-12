/*
* InterruptControl.h
*
*  Created on: Oct 3, 2024
*      Author: derek
*/
#ifndef INTERRUPTCONTROL_H_
#define INTERRUPTCONTROL_H_


#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <STM32F429i.h>

#define EXTI0_IRQ_NUMBER 6

void interruptEnable(uint8_t IRQ_num);
void interruptDisable(uint8_t IRQ_num);
void interruptClearPending(uint8_t IRQ_num);
void interruptSetPending(uint8_t IRQ_num);
void EXTIClear(uint8_t IRQ_num);

#define NVIC_ISER0 ((volatile uint32_t*) 0xE000E100)
#define NVIC_ICER0 ((volatile uint32_t*) 0xE000E180)
#define NVIC_ISPR0 ((volatile uint32_t*) 0xE000E200)
#define NVIC_ICPR0 ((volatile uint32_t*) 0xE000E280)
#define NVIC_IABR0 ((volatile uint32_t*) 0xE000E300)
#define NVIC_IPR0 ((volatile uint32_t*) 0xE000E400)
#define NVIC_ISER1 ((volatile uint32_t*) 0xE000E104)
#define NVIC_ICER1 ((volatile uint32_t*) 0xE000E184)
#define NVIC_ISPR1 ((volatile uint32_t*) 0xE000E204)
#define NVIC_ICPR1 ((volatile uint32_t*) 0xE000E284)
#define NVIC_IABR1 ((volatile uint32_t*) 0xE000E304)
#define NVIC_IPR1 ((volatile uint32_t*) 0xE000E404)

#endif /* INTERRUPTCONTROL_H_ */
