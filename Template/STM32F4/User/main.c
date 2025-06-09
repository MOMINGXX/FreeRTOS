#include "stm32f4xx.h"                  // Device header
#include "USART.h"
#include "KEY.h"
#include "Delay.h"
#include "MPU6050.h"
#include "ROUKER.h"
#include "NRF24L01.h"
#include "LCD_Init.h"
#include "LCD_Picture.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "LCD.h"
#include "timers.h"
#include "TIMERx.h"
#include "semphr.h"

TaskHandle_t Task1_Handler;
TaskHandle_t SystemCreatTask_Handler;

QueueHandle_t Key_SemaphoreHandle ;

uint32_t Key_Number = 0;
uint8_t Mode[4];

// 定义全局变量记录当前显示行
static uint16_t current_line = 58;
#define MAX_LINES 8  // 根据你的LCD高度调整
#define LINE_HEIGHT 24 // 每行高度

// 动态显示中文字符函数
void dynamic_display_stack_info(char *text)
{
    // 如果超出屏幕范围，重置行号
    if(current_line >= 280) {
        current_line = 58;
    }
    
    // 显示堆栈信息
    LCD_ShowGB2312_Chinese(0, current_line, 24, (uint8_t *)text, BLACK, WHITE);
    
    // 移动到下一行
    current_line += LINE_HEIGHT;
}

void Task1(void *pvParameters)
{	
	uint16_t Num1 = 0,Num2 = 0;
    while(1)
    {
		if (Key_Number)
		{
			if (Key_Number == 1)
			{
				Mode[0]++;
				Mode[0] %= 6;
				LED_SetMode(0,Mode[0]);
			}
			else if (Key_Number == 2)
			{
				Mode[1]++;
				Mode[1] %= 6;
				LED_SetMode(1,Mode[1]);
			}
			else if (Key_Number == 3)
			{
				Mode[2]++;
				Mode[2] %= 6;
				LED_SetMode(2,Mode[2]);
			}
			else if (Key_Number == 4)
			{
				Mode[3]++;
				Mode[3] %= 6;
				LED_SetMode(3,Mode[3]);
			}
			else if (Key_Number == 5)
			{
				int i;
				i++;
				LCD_ShowIntNum(150,184,i,3,BLACK,WHITE,24);
			}
			LCD_ShowString(20,64,"Key_Number = ",BLACK,WHITE,24,0);
			LCD_ShowIntNum(180,64,Key_Number,2,BLACK,WHITE,24);
			Key_Number = 0;
		}
		LCD_ShowString(20,88,"LED1_Mode = ",BLACK,WHITE,24,0);
		LCD_ShowIntNum(150,88,Mode[0],2,BLACK,WHITE,24);
		LCD_ShowString(20,112,"LED2_Mode = ",BLACK,WHITE,24,0);
		LCD_ShowIntNum(150,112,Mode[1],2,BLACK,WHITE,24);
		LCD_ShowString(20,136,"LED3_Mode = ",BLACK,WHITE,24,0);
		LCD_ShowIntNum(150,136,Mode[2],2,BLACK,WHITE,24);
		LCD_ShowString(20,160,"LED4_Mode = ",BLACK,WHITE,24,0);
		LCD_ShowIntNum(150,160,Mode[3],2,BLACK,WHITE,24);

		if(Key_Check(KEY_FRONTL,KEY_SINGLE) || Key_Check(KEY_ROUKER_RIGHT,KEY_REPEAT))
		{
			Num1++;
		}
		if(Key_Check(KEY_FRONTR,KEY_DOUBLE))
		{
			Num1+=100;
		}
		if(Key_Check(KEY_ROUKER_RIGHT,KEY_LONG))
		{
			Num1 = 0;
		}
		if(Key_Check(KEY_B,KEY_LONG))
		{
			Num2 = 5;
		}
		LCD_ShowString(20,208,"Num1 = ",BLACK,WHITE,24,0);
		LCD_ShowIntNum(150,208,Num1,5,BLACK,WHITE,24);
		LCD_ShowString(20,232,"Num2 = ",BLACK,WHITE,24,0);
		LCD_ShowIntNum(150,232,Num2,5,BLACK,WHITE,24);
		printf("Key_Number = %d\r\n",Key_Number);
	}	
}

void Task2(void *pvParameters)
{	
    while(1)
    {
		//xQueueReceive(Key_QueueHandle,&Key_Number,portMAX_DELAY);
		xTaskNotifyWait(0,0xffffffff,&Key_Number,portMAX_DELAY);
		vTaskDelay(pdMS_TO_TICKS(10));
	}	
}

void SystemCreatTask(void *pvParameters)
{
	taskENTER_CRITICAL();			//进入临界区

	xTaskCreate(Task1, "Task1", 130, NULL,2,&Task1_Handler);
	xTaskCreate(Task2, "Task2", 130, NULL,2,&Key_NotifyHandle);
	xTaskCreate(vKey_ReadTask,"vKey_ReadTask",130,NULL,2,NULL);
	vTaskDelete(NULL);
	
	taskEXIT_CRITICAL();			//退出临界区
}

int main()
{
	Delay_Init();
//	MPU6050_Init();
	Usart_Init();
//	EQUIPMENT_Init();
	W25Q128JV_Init();
//	NRF24L01_Init();
	TIMEx_Init();
	LED_Init();
	KEY_Init();
	LCD_Init();
	TIME3x_Init();
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		//中断优先级组4
	LCD_SetBrightness(5);
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	xTaskCreate(SystemCreatTask,"SystemCreatTask",180,NULL,2,&SystemCreatTask_Handler);		
	
	vTaskStartScheduler();
	while(1)
	{
	}
}    
             