/*
* Scheduler.h
*
*  Created on: Sep 10, 2024
*      Author: derek
*/
#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>


#define LED_TOGGLE_EVENT (1 << 0)
#define LED_DELAY_EVENT (1 << 1)
#define BUTTON_POLL_EVENT (1 << 2)
#define UPDATE_TICK_EVENT (1<<3)
#define END_THE_GAME (1<<4)

uint32_t getScheduledEvents();
void addSchedulerEvent(uint32_t event);
void removeSchedulerEvent(uint32_t event);

#endif /* SCHEDULER_H_ */
