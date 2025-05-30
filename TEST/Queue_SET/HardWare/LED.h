#ifndef _LED_H_
#define _LED_H_

#include "stm32f4xx.h"                  // Device header

#define LEDx_CLK				(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC)
#define LED_APBxClockCMD		RCC_AHB1PeriphClockCmd

#define LED1_GPIO_PORT          GPIOB
#define LEDx_GPIO_PORT          GPIOC
#define LED_RIGHT_GPIO_PIN    	GPIO_Pin_2		//GPIOB
#define LED_MIDDLE_GPIO_PIN     GPIO_Pin_4
#define LED_LEFT_GPIO_PIN     	GPIO_Pin_5
#define LED_DOWN_GPIO_PIN       GPIO_Pin_3

#define ON                      1
#define OFF                     0

#define LED_LEFT(x)             if(x) \
									GPIO_ResetBits(LEDx_GPIO_PORT,LED_LEFT_GPIO_PIN); \
								else \
									GPIO_SetBits(LEDx_GPIO_PORT,LED_LEFT_GPIO_PIN);
#define LED_RIGHT(x)            if(x) \
									GPIO_ResetBits(LED1_GPIO_PORT,LED_RIGHT_GPIO_PIN); \
								else \
									GPIO_SetBits(LED1_GPIO_PORT,LED_RIGHT_GPIO_PIN);								
#define LED_MIDDLE(x)           if(x) \
									GPIO_ResetBits(LEDx_GPIO_PORT,LED_MIDDLE_GPIO_PIN); \
								else \
									GPIO_SetBits(LEDx_GPIO_PORT,LED_MIDDLE_GPIO_PIN);
#define LED_DOWN(x)             if(x) \
									GPIO_ResetBits(LEDx_GPIO_PORT,LED_DOWN_GPIO_PIN); \
								else \
									GPIO_SetBits(LEDx_GPIO_PORT,LED_DOWN_GPIO_PIN);

#define LED_LEFT_TOGGLE			(LEDx_GPIO_PORT->ODR ^= LED_LEFT_GPIO_PIN)		
#define LED_RIGHT_TOGGLE		(LED1_GPIO_PORT->ODR ^= LED_RIGHT_GPIO_PIN)		
#define LED_MIDDLE_TOGGLE		(LEDx_GPIO_PORT->ODR ^= LED_MIDDLE_GPIO_PIN)		
#define LED_DOWN_TOGGLE			(LEDx_GPIO_PORT->ODR ^= LED_DOWN_GPIO_PIN)					
								
void LED_Init();

#endif
