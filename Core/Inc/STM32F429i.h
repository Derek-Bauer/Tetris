/*
* STM32F429i.h
*
*  Created on: Sep 10, 2024
*      Author: derek
*/
#ifndef STM32F429I_H_
#define STM32F429I_H_

#include <stdint.h>
#include <stdbool.h>


typedef struct {
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFR[2];
}GPIO_RegDef_t;


typedef struct {
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t AHB3RSTR;
	volatile uint32_t Res1;
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t Res2[2];
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t AHB3ENR;
	volatile uint32_t Res3;
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t Res4[2];
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	volatile uint32_t AHB3LPENR;
	volatile uint32_t Res5;
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	volatile uint32_t Res6[2];
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t Res7[2];
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
	volatile uint32_t PLLSAICFGR;
	volatile uint32_t DCKCFGR;
}RCC_RegDef_t;


typedef struct {
	volatile uint32_t MEMRMP;
	volatile uint32_t PMC;
	volatile uint32_t EXTICR[4];
	volatile uint32_t Res8[2];
	volatile uint32_t CMPCR;
}SYSCFG_RegDef_t;


typedef struct {
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
}EXTI_RegDef_t;

typedef struct{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMCR;
	volatile uint32_t DIER;
	volatile uint32_t SR;
	volatile uint32_t EGR;
	volatile uint32_t CCMR1;
	volatile uint32_t CCMR2;
	volatile uint32_t CCER;
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;
	volatile uint32_t Res1;
	volatile uint32_t CCR1;
	volatile uint32_t CCR2;
	volatile uint32_t CCR3;
	volatile uint32_t CCR4;
	volatile uint32_t Res2;
	volatile uint32_t DCR;
	volatile uint32_t DMAR;
	volatile uint32_t OR;
}GPTIMR_RegDef_t;


typedef struct{
	volatile uint32_t CR;
	volatile uint32_t SR;
	volatile uint32_t DR;
}RNG_RegDef_t;

#define AHB1_BASE_ADDR 0x40020000
#define AHB2_BASE_ADDR 0x50000000
#define APB1_BASE_ADDR 0x40000000
#define APB2_BASE_ADDR 0x40010000

#define RNG_BASE_ADDR (AHB2_BASE_ADDR+0x60800)
#define RCC_BASE_ADDR (AHB1_BASE_ADDR + 0x3800)
#define GPIOG_BASE_ADDR (AHB1_BASE_ADDR + 0x1800)
#define GPIOA_BASE_ADDR (AHB1_BASE_ADDR + 0x0)
#define SYSCFG_BASE_ADDR (APB2_BASE_ADDR + 0x3800)
#define EXTI_BASE_ADDR (APB2_BASE_ADDR + 0x3C00)
#define TIM2_BASE_ADDR (APB1_BASE_ADDR + 0)
#define TIM5_BASE_ADDR (APB1_BASE_ADDR + 0x0C00)
#define TIM7_BASE_ADDR (APB1_BASE_ADDR + 0x1400)

#define GPIOG_0 ((GPIO_RegDef_t*)GPIOG_BASE_ADDR)
#define GPIOA_0 ((GPIO_RegDef_t*)GPIOA_BASE_ADDR)
#define SYSCFG_0 ((SYSCFG_RegDef_t*)SYSCFG_BASE_ADDR)
#define EXTI_0 ((EXTI_RegDef_t*)EXTI_BASE_ADDR)
#define TIM2_0 ((GPTIMR_RegDef_t*) TIM2_BASE_ADDR)
#define TIM5_0 ((GPTIMR_RegDef_t*) TIM5_BASE_ADDR)
#define TIM7_0 ((GPTIMR_RegDef_t*) TIM7_BASE_ADDR)
#define RCC_0 ((RCC_RegDef_t*) RCC_BASE_ADDR)


#define AHB1_CLK_ENABLE(offset) (RCC->AHB1ENR |=(1<<offset))
#define AHB1_CLK_DISABLE(offset) (RCC->AHB1ENR &= ~(1<<offset))

#define TIM_CLK_ENABLE(offset) (RCC->APB1ENR |=(1<<offset))
#define TIM_CLK_DISABLE(offset) (RCC->APB1ENR &= ~(1<<offset))

#define TIM2_RCC_CLK_OFFSET 0
#define TIM5_RCC_CLK_OFFSET 3
#define TIM7_RCC_CLK_OFFSET 5
#define GPIOG_RCC_BIT 6
#define GPIOA_RCC_BIT 0

#define ACTIVE 1
#define NON_ACTIVE 0
#define SET 1
#define RESET 0
#define ENABLE SET
#define DISABLE RESET
#define BUTTON_PIN_NUM 0
#define SET_ENABLE NVIC_ISER0
#define CLEAR_ENABLE NVIC_ICER0
#define SET_PENDING NVIC_ISPR0
#define CLEAR_PENDING NVIC_ICPR0
#define ACTIVE_BIT NVIC_IABR0
#define PRIORITY NVIC_IPR0
#define TRIGGER_INTERRUPT STIR
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
#define STIR ((volatile uint32_t*) 0xE000EF00x)
#define EXTI_LINE0_IRQ 6
#define SYSCFG_CLK_ENABLE() (RCC->APB2ENR |=(1<<14))
#define SYSCFG_CLK_DISABLE() (RCC->APB2ENR ~=(1<<14))
#define GPIOG_RCC_ENABLE_ADDRESS 0x40023830
#define GPIOG_AHB1ENR_BIT_OFFSET 6
#define GPIOG_MODER 0x40021800
#define GPIOG_ODR 0x40021814
#define GPIOG_MODER_BITS 0
#define RED_LED_ODR_BIT 14
#define RED_LED_MODER_BITS 28
#define GREEN_LED_ODR_BIT 13
#define GREEN_LED_MODER_BITS 26
#define MY_NAME_LENGTH 5



#define TIM2_IRQ 28
#define TIM5_IRQ 50
#define TIM7_IRQ 55



#endif /* STM32F429I_H_ */
