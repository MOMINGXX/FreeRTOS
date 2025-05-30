#ifndef _TIMERx_H_
#define _TIMERx_H_ 			   

#include "stm32f4xx.h"                  // Device header
#include "LED.h"

#define TIME_x                      TIM2
#define TIMEx_CLK                   RCC_APB1Periph_TIM2
#define TIMEx_APBxClockCmd          RCC_APB1PeriphClockCmd
#define TIMEx_IRQChannel            TIM2_IRQn
#define TIMEx_IRQHandler            TIM2_IRQHandler
#define TIME_x_PreemptionPriority   5
#define TIME_x_SubPriority          0

#define TIME3_x                     TIM3
#define TIME3x_CLK                  RCC_APB1Periph_TIM3
#define TIME3x_APBxClockCmd         RCC_APB1PeriphClockCmd
#define TIME3x_IRQChannel           TIM3_IRQn
#define TIME3x_IRQHandler           TIM3_IRQHandler
#define TIME3_x_PreemptionPriority  5
#define TIME3_x_SubPriority         0

void TIMEx_Init();
void TIME3x_Init();
#endif

