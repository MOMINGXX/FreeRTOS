#ifndef _DELAY_H_
#define _DELAY_H_ 			   

#include "stm32f4xx.h"                  // Device header
//#include "misc.h"
#include "FreeRTOS.h"	
#include "task.h"	

void Delay_Init();
void Delay_us(uint32_t time);
void Delay_ms(uint32_t time);
void Delay_s(uint16_t time);

#endif

