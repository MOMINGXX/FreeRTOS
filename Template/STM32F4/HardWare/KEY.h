#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f4xx.h"                  // Device header
#include "NRF24L01.h"
#include "LED.h"
#include "TIMERx.h"
#include "USART.h"
//#include "Delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define KEY_USE_TIMER	1				//使用定时器消抖  可直接读取长按状态

extern QueueHandle_t Key_QueueHandle;

// MP3 Play
typedef enum 
{
    REDECE = 1,
    ADD,
    STOP,
    START,
    FORWWARD,
    LEFT,
    RIGHT,
    BACK,
    PLAY,
    SUSPENDED,
    NEXT,
    LAST,
}Key_Typedef;

#define KEYx_CLK                	(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC)
#define KEY_APBx_ClockCMD       	RCC_AHB1PeriphClockCmd

#define KEY1x_GPIO_PORT         	GPIOA    	//  4 15
#define KEY2x_GPIO_PORT         	GPIOB      	//	1 7 8 9
#define KEY1_GPIO_PORT          	GPIOC      	//  0 1 2 13 14 15

#define KEY_FRONTL_GPIO_PIN      	GPIO_Pin_15 	//GPIOA
#define ROUKER_RIGHT_GPIO_PIN    	GPIO_Pin_4 		//GPIOA
#define KEY_ON_GPIO_PIN          	GPIO_Pin_9		//GPIOB
#define KEY_LEFT_GPIO_PIN        	GPIO_Pin_8		//GPIOB
#define KEY_FRONTR_GPIO_PIN      	GPIO_Pin_1		//GPIOB
#define ROUKER_LEFT_GPIO_PIN     	GPIO_Pin_7		//GPIOB
#define KEY_DOWN_GPIO_PIN        	GPIO_Pin_13		//GPIOC
#define KEY_RIGHT_GPIO_PIN       	GPIO_Pin_14		//GPIOC
#define KEY_X_GPIO_PIN           	GPIO_Pin_15		//GPIOC
#define KEY_Y_GPIO_PIN           	GPIO_Pin_1 		//GPIOC
#define KEY_A_GPIO_PIN           	GPIO_Pin_0 		//GPIOC
#define KEY_B_GPIO_PIN           	GPIO_Pin_2 		//GPIOC

void KEY_Init();
void KEY_LOOP();
uint8_t Key_GetNumber();
void Key_Send(uint8_t KetNum);
void vKey_Task(void *pvParameters);

#endif
