/*
 * Timer_Driver.h
 *
 *  Created on: Oct 10, 2024
 *      Author: derek
 */


#ifndef TIMER_DRIVER_H_
#define TIMER_DRIVER_H_

#include <InterruptControl.h>
#include <stdbool.h>

// CONFIG STRUCT
typedef struct {
	uint32_t AutoReloadVal;
	uint32_t MasterModeSel;
	uint32_t ClkDivSel;
	uint32_t PrescalerVal;
	uint32_t CenterAlignModeSel;
	bool AutoReloadBuffEn;
	bool CountDownEn;
	bool InterruptUpdateEn;
	bool DisableUpdateEvent;
	bool OnePulseMode;
}GPTimer_Config_t;


// CONFIGURATION MACROS
#define MASTER_MODE_RESET 0
#define MASTER_MODE_EN 1
#define MASTER_MODE_UPDATE 2
#define MASTER_MODE_COMP 3
#define MASTER_MODE_COMP_OC1 4
#define MASTER_MODE_COMP_OC2 5
#define MASTER_MODE_COMP_OC3 6
#define MASTER_MODE_COMP_OC4 7
#define CLK_DIV_RAT_1 0
#define CLK_DIV_RAT_2 1
#define CLK_DIV_RAT_4 2
#define EDGE_ALIGNED_MODE 0
#define CENTER_ALIGNED_MODE_1 1
#define CENTER_ALIGNED_MODE_2 2
#define CENTER_ALIGNED_MODE_3 3

// OFFSET MACROS
#define CLK_DIV_SEL_OFFSET 8
#define AUTO_RELOAD_BUFF_OFFSET 7
#define CENTER_ALIGN_MODE_SEL_OFFSET 5
#define COUNT_DOWN_EN_OFFSET 4
#define ONE_PULSE_MODE_OFFSET 3
#define DISABLE_UPDATE_EVENT_OFFSET 1
#define MASTER_MODE_SEL_OFFSET 4
#define UPDATE_INTERRUPT_EN_OFFSET 0
#define COUNTER_ENABLE_OFFSET 0
#define UPDATE_INTERRUPT_FLAG_OFFSET 0

// FUNCTIONS
void LEDTimer7Init();
void timerInit(GPTIMR_RegDef_t* gptimr, GPTimer_Config_t timr_config);
void timerClk(GPTIMR_RegDef_t* gptimr, uint8_t on_off);
void timerStart(GPTIMR_RegDef_t* gptimr);
void timerStop(GPTIMR_RegDef_t* gptimr);
void timerReset(GPTIMR_RegDef_t* gptimr);
uint32_t timerCountValue(GPTIMR_RegDef_t* gptimr);
void timerInterruptEnableDisable(GPTIMR_RegDef_t* gptimr, uint8_t enable_disable);
uint32_t timerReloadValue(GPTIMR_RegDef_t* gptimr);





#endif /* TIMER_DRIVER_H_ */
