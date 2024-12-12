/*
* InterruptControl.c
*
*  Created on: Oct 3, 2024
*      Author: derek
*/
#include <InterruptControl.h>

void interruptEnable(uint8_t IRQ_num){
	if(IRQ_num < 32){
			*NVIC_ISER0 |= 1<<IRQ_num;
		}
		else{
			int modIRQ = IRQ_num%32;
			*NVIC_ISER1 |= 1<<(modIRQ);
		}
};

void interruptDisable(uint8_t IRQ_num){
	if(IRQ_num < 32){
			*NVIC_ICER0 |= 1<<IRQ_num;
		}
		else{
			uint32_t modIRQ = IRQ_num%32;
			*NVIC_ICER1 |= 1<<(modIRQ);
		}
};

void interruptClearPending(uint8_t IRQ_num){
	if(IRQ_num < 32){
			*NVIC_ICPR0 |= 1<<IRQ_num;
		}
		else{
			uint32_t modIRQ = IRQ_num%32;
			*NVIC_ICPR1 |= 1<<(modIRQ);
		}
};

void interruptSetPending(uint8_t IRQ_num){
	if(IRQ_num < 32){
			*NVIC_ISPR0 |= 1<<IRQ_num;
		}
		else{
			uint32_t modIRQ = IRQ_num%32;
			*NVIC_ISPR1 |= 1<<(modIRQ);
		}
};

void EXTIClear(uint8_t pin_num){
	EXTI->PR |= (1<<pin_num);
};
