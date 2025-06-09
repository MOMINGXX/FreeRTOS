#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f4xx.h"                  // Device header
#include "USART.h"
#include "TIMERx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define KEY_USE_TIMER				//ʹ�ö�ʱ������  ��ֱ�Ӷ�ȡ����״̬    ������ʽ
//#define KEY_USE_QUEUE				//ʹ�ö���
#define KEY_USE_NOTIFY			//ʹ��֪ͨ 

extern QueueHandle_t Key_QueueHandle;
extern TaskHandle_t Key_NotifyHandle;

#define KEY_PRESSED		            1       //��������
#define KEY_UNPRESSED			    0       //����δ����

#define KEY_COUNT					12      //��������

#define KEY_LONG_TIME				1000    //��������ʱ�� 1s      
#define KEY_DOUBLE_TIME				300     //����˫��ʱ��  300ms
#define KEY_REPEAT_TIME			    50      //�����ظ�ʱ��  50ms

typedef enum
{
    KEY_HOLD   = 0x01,          //������ס����
    KEY_PRESS  = 0x02,          //��������
    KEY_FREED  = 0x04,          //�����ͷ�
    KEY_SINGLE = 0x08,          //��������
    KEY_DOUBLE = 0x10,          //����˫��
    KEY_LONG   = 0x20,          //��������
    KEY_REPEAT = 0x40,          //�����ظ�
}KEY_STATEFLAG;                 //����״̬��־

typedef enum
{
    KEY_FRONTL  = 0,
    KEY_FRONTR,
    KEY_ROUKER_RIGHT,
    KEY_ROUKER_LEFT,
    KEY_UP,
    KEY_LEFT,
    KEY_DOWN,
    KEY_RIGHT,
    KEY_Y,
    KEY_X,
    KEY_A,
    KEY_B,
}KEY_INDEX;                    //��������

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
#define KEY_UP_GPIO_PIN          	GPIO_Pin_9		//GPIOB
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
void KEY_Tick();
void Key_StateTick();
uint8_t Key_GetNumber();
uint8_t Key_Check(uint8_t KEY_ID,uint8_t flag);
void vKey_ReadTask(void *pvParameters);

#endif
