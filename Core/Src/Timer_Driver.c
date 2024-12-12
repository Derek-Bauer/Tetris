/*
 * Timer_Driver.c
 *
 *  Created on: Oct 10, 2024
 *      Author: derek
 */

#include <Timer_Driver.h>

void timerInit(GPTIMR_RegDef_t* gptimr, GPTimer_Config_t timr_config){
	gptimr->CR1 &= ~(0x3 << CLK_DIV_SEL_OFFSET);
	gptimr->CR1 |= (timr_config.ClkDivSel << CLK_DIV_SEL_OFFSET);
	gptimr->CR1 &= ~(0x1 << AUTO_RELOAD_BUFF_OFFSET);
	gptimr->CR1 |= (timr_config.AutoReloadBuffEn << AUTO_RELOAD_BUFF_OFFSET);
	gptimr->CR1 &= ~(0x3 << CENTER_ALIGN_MODE_SEL_OFFSET);
	gptimr->CR1 |= (timr_config.CenterAlignModeSel << CENTER_ALIGN_MODE_SEL_OFFSET);
	gptimr->CR1 &= ~(0x1 << COUNT_DOWN_EN_OFFSET);
	gptimr->CR1 |= (timr_config.CountDownEn << COUNT_DOWN_EN_OFFSET);
	gptimr->CR1 &= ~(0x1 << ONE_PULSE_MODE_OFFSET);
	gptimr->CR1 |= (timr_config.OnePulseMode << ONE_PULSE_MODE_OFFSET);
	gptimr->CR1 &= ~(0x1 << DISABLE_UPDATE_EVENT_OFFSET);
	gptimr->CR1 |=(timr_config.DisableUpdateEvent << DISABLE_UPDATE_EVENT_OFFSET);

	//gptimr->CR2 &= ~(0x7 << MASTER_MODE_SEL_OFFSET);
	//gptimr->CR2 |= (timr_config.MasterModeSel << MASTER_MODE_SEL_OFFSET);

	gptimr->DIER &= ~(0x1 << UPDATE_INTERRUPT_EN_OFFSET);
	gptimr->DIER |= (timr_config.InterruptUpdateEn << UPDATE_INTERRUPT_EN_OFFSET);

	gptimr->PSC = (timr_config.PrescalerVal);

	gptimr->ARR = (timr_config.AutoReloadVal);

	// if timer's interrupt update is enabled in config, enable the interrupt
	if((gptimr->DIER) & (0x1 << UPDATE_INTERRUPT_EN_OFFSET)){
		timerInterruptEnableDisable(gptimr, ENABLE);
	}
};

void LEDTimer7Init(){
	GPTimer_Config_t timr7_config = {0};
	timr7_config.AutoReloadVal = 0xFFFF;
	timr7_config.ClkDivSel = CLK_DIV_RAT_4;
	timr7_config.PrescalerVal = 0xFFFF;
	timr7_config.MasterModeSel = MASTER_MODE_RESET;
	timr7_config.CenterAlignModeSel = EDGE_ALIGNED_MODE;
	timr7_config.AutoReloadBuffEn = NON_ACTIVE;
	timr7_config.CountDownEn = NON_ACTIVE;
	timr7_config.InterruptUpdateEn = ACTIVE;
	timr7_config.DisableUpdateEvent = NON_ACTIVE;
	timr7_config.OnePulseMode = NON_ACTIVE;

	TIM_CLK_ENABLE(TIM7_RCC_CLK_OFFSET);
	timerInit(TIM7_0, timr7_config);
};

void timerClk(GPTIMR_RegDef_t* gptimr, uint8_t on_off){
	if(gptimr == TIM5_0){
		TIM_CLK_ENABLE(TIM5_RCC_CLK_OFFSET);
	}

	if(gptimr == TIM2_0){
		TIM_CLK_ENABLE(TIM2_RCC_CLK_OFFSET);
	}
	if(gptimr == TIM7_0){
		TIM_CLK_ENABLE(TIM7_RCC_CLK_OFFSET);
	}
};


void timerStart(GPTIMR_RegDef_t* gptimr){
	gptimr->CR1 |= (0x1<< COUNTER_ENABLE_OFFSET);
};


void timerStop(GPTIMR_RegDef_t* gptimr){
	gptimr->CR1 &= ~(0x1<< COUNTER_ENABLE_OFFSET);
};


void timerReset(GPTIMR_RegDef_t* gptimr){
	gptimr->CNT = 0;
};


uint32_t timerCountValue(GPTIMR_RegDef_t* gptimr){
	return gptimr->CNT;
};


void timerInterruptEnableDisable(GPTIMR_RegDef_t* gptimr, uint8_t enable_disable){
	if(enable_disable == ENABLE){
		if(gptimr == TIM5_0){
			interruptEnable(TIM5_IRQ);
		}
		if(gptimr == TIM2_0){
			interruptEnable(TIM2_IRQ);
		}
		if(gptimr == TIM7_0){
			interruptEnable(TIM7_IRQ);
		}
	}

	if(enable_disable == DISABLE){
		if(gptimr == TIM5_0){
			interruptDisable(TIM5_IRQ);
		}
		if(gptimr == TIM2_0){
			interruptDisable(TIM2_IRQ);
		}
		if(gptimr == TIM7_0){
			interruptDisable(TIM7_IRQ);
		}
	}
};


uint32_t timerReloadValue(GPTIMR_RegDef_t* gptimr){
	return gptimr->ARR;
};
