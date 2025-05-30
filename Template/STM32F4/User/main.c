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
#include "TIMERx.h"

TaskHandle_t Task1_Handler;
TaskHandle_t SystemCreatTask_Handler;

QueueHandle_t KeyNumber_QueueHandle;
QueueHandle_t Message_QueueHandle;

char arr[20] = {"Hello World!"};

// 定义全局变量记录当前显示行
static uint16_t current_line = 28;
#define MAX_LINES 10  // 根据你的LCD高度调整
#define LINE_HEIGHT 24 // 每行高度

// 动态显示函数
void dynamic_display_stack_info(uint32_t uxHighWaterMark)
{
    // 如果超出屏幕范围，重置行号
    if(current_line >= 280) {
        current_line = 28;
    }
    
    // 显示堆栈信息
    Display_GB2312_String(0, current_line, 24, (uint8_t *)"正常", BLACK, WHITE);
    LCD_ShowChar(144, current_line, ':', BLACK, WHITE, 24, 0);
    LCD_ShowIntNum(168, current_line, uxHighWaterMark, 4, BLACK, WHITE, 24);
    
    // 移动到下一行
    current_line += LINE_HEIGHT;
}

void Task1(void *pvParameters)
{
	uint8_t KeyNumber;		
    while(1)
    {
		KeyNumber = Key_GetNumber();
		if(KeyNumber != 0)
		{
			printf("KeyNumber = %d\r\n",KeyNumber);
		}
		vTaskDelay(10);
	}
}

//接受键值
void Task2(void *pvParameters)
{ 
	uint8_t KeyNum;
	BaseType_t xReturn;
    while(1)
    {
		xReturn = xQueueReceive(Key_QueueHandle,&KeyNum,portMAX_DELAY);
		if(xReturn)
		{
			LCD_ShowString(0,120,"KeyNumber =",BLACK,WHITE,24,0);
			LCD_ShowIntNum(144,120,KeyNum,2,BLACK,WHITE,24);
			printf("KeyNumber = %d\r\n",KeyNum);
		} 
//		else
//		{
//			printf("KeyNumber Receive ERROR\r\n");
//		}
	}
}

void Task3(void *pvParameters)
{
	while(1)
	{
		LED_LEFT_TOGGLE;
		vTaskDelay(1000);
	}
}

void Task4(void *pvParameters)
{
	while(1)
	{
		LED_RIGHT_TOGGLE;
		vTaskDelay(2000);
	}
}

void SystemCreatTask(void *pvParameters)
{
	taskENTER_CRITICAL();			//进入临界区
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	KeyNumber_QueueHandle = xQueueCreate(2,sizeof(uint8_t));			
	if(KeyNumber_QueueHandle != NULL)
	{
		LCD_ShowString(0,24,"KeyNumber Successful!",BLACK,WHITE,24,0);
		printf("KeyNumber_QueueHandle Successful !\r\n");
	}
	Message_QueueHandle = xQueueCreate(1,sizeof(arr) / sizeof(arr[0]));			
	if(Message_QueueHandle != NULL)
	{
		LCD_ShowString(0,48,"Message Successful!",BLACK,WHITE,24,0);
		printf("Message_QueueHandle Successful !\r\n");
	}
	else 
	{
		LCD_ShowString(0,48,"Message ERROR!",BLACK,WHITE,24,0);
		printf("Message_QueueHandle ERROR !\r\n");
	}
	

	xTaskCreate(Task1, "Task1", 130, NULL,2,&Task1_Handler);
	xTaskCreate(vKey_Task, "vKey_Task", 130, NULL,2,NULL);
	xTaskCreate(Task2, "Task2", 130, NULL,2,NULL);
	xTaskCreate(Task3, "Task3", 130, NULL,2,NULL);
	xTaskCreate(Task4, "Task4", 130, NULL,2,NULL);
	vTaskDelete(NULL);
	taskEXIT_CRITICAL();			//退出临界区
}

int main()
{
	LED_Init();
//	MPU6050_Init();
	Usart_Init();
//	EQUIPMENT_Init();
	W25Q128JV_Init();
//	NRF24L01_Init();
	KEY_Init();
	Delay_Init();
	LCD_Init();
	TIMEx_Init();
//	TIME3x_Init();
	
   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	
	xTaskCreate(SystemCreatTask,"SystemCreatTask",180,NULL,2,&SystemCreatTask_Handler);		
	vTaskStartScheduler();
	
	while(1)
	{
			
	}
}    
             