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

#define LED_LEFT(x)         if(x) \
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
#define LED_CONTROL(port,pin)   if(x) \
									GPIO_ResetBits(port,pin); \
								else \
									GPIO_SetBits(port,pin);

#define LED_LEFT_TOGGLE			(LEDx_GPIO_PORT->ODR ^= LED_LEFT_GPIO_PIN)		
#define LED_RIGHT_TOGGLE		(LED1_GPIO_PORT->ODR ^= LED_RIGHT_GPIO_PIN)		
#define LED_MIDDLE_TOGGLE		(LEDx_GPIO_PORT->ODR ^= LED_MIDDLE_GPIO_PIN)		
#define LED_DOWN_TOGGLE			(LEDx_GPIO_PORT->ODR ^= LED_DOWN_GPIO_PIN)	

#define LED_NUM					4	

//LEDÁÁÃðÄ£Ê½
typedef enum
{
	LED_OFF = 0,
	LED_ON,
	LED_SLOW_BLINK,
	LED_FAST_BLINK,
	LED_BIT_BLINK,
	LED_BREATHE
}LED_LIGHT_MODE;

typedef enum
{
	LED_LEFT = 0,
	LED_RIGHT,
	LED_MIDDLE,
	LED_DOWN
}LED_NAME;
						
void LED_Init();
void LED_Tick();
void LED_SetMode(uint8_t Num,LED_LIGHT_MODE mode);

#endif
