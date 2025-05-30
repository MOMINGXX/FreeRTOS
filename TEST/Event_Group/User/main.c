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
#include "event_groups.h"

TaskHandle_t Task1_Handler;
TaskHandle_t SystemCreatTask_Handler;

EventGroupHandle_t xEventGroup_Handle;

enum 
{
    EVENT_BITS1 = (1 << 0),
    EVENT_BITS2 = (1 << 1),
    EVENT_BITS3 = (1 << 2),
    EVENT_BITS4 = (1 << 3),
}EVENT_BITS;

// 定义全局变量记录当前显示行
static uint16_t current_line = 28;
#define MAX_LINES 10  // 根据你的LCD高度调整
#define LINE_HEIGHT 24 // 每行高度

// 动态显示中文字符函数
void dynamic_display_stack_info(char *text)
{
    // 如果超出屏幕范围，重置行号
    if(current_line >= 280) {
        current_line = 28;
    }
    
    // 显示堆栈信息
    Display_GB2312_String(0, current_line, 24, (uint8_t *)text, BLACK, WHITE);
    
    // 移动到下一行
    current_line += LINE_HEIGHT;
}

void Task1(void *pvParameters)
{	
	uint8_t Key_Number = 0;
    while(1)
    {
		xQueueReceive(Key_QueueHandle, &Key_Number, portMAX_DELAY);
		if(Key_Number == 1)
		{
		    xEventGroupSetBits(xEventGroup_Handle, EVENT_BITS1);
		}
		else if(Key_Number == 2)
		{
		    xEventGroupSetBits(xEventGroup_Handle, EVENT_BITS2);
		}
		else if(Key_Number == 3)
		{
		    xEventGroupSetBits(xEventGroup_Handle, EVENT_BITS3);
		}
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

//接受键值
void Task2(void *pvParameters)
{ 
	EventBits_t xReturnedValue;
    while(1)
    {
		xReturnedValue = xEventGroupWaitBits(xEventGroup_Handle, EVENT_BITS1 | EVENT_BITS2 | EVENT_BITS3, pdTRUE, pdFALSE, portMAX_DELAY);
		if (xReturnedValue & EVENT_BITS1)
		{
			dynamic_display_stack_info("事件一获取成功");
		}
		else if (xReturnedValue & EVENT_BITS2)
		{
			dynamic_display_stack_info("事件二获取成功");
		}
		else if (xReturnedValue & EVENT_BITS3)
		{
			dynamic_display_stack_info("事件三获取成功");
		}
		printf("xReturnedValue = 0x%x\r\n",xReturnedValue);
		vTaskDelay(pdMS_TO_TICKS(100));
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

void SystemCreatTask(void *pvParameters)
{
	taskENTER_CRITICAL();			//进入临界区

	xEventGroup_Handle = xEventGroupCreate();
	if(xEventGroup_Handle != NULL)
	{
		printf("Event Group Created Successfully!\r\n");
		dynamic_display_stack_info("事件组创建成功");
	}
	
	xTaskCreate(Task1, "Task1", 130, NULL,2,&Task1_Handler);
	xTaskCreate(vKey_Task, "vKey_Task", 130, NULL,2,NULL);
	xTaskCreate(Task2, "Task2", 130, NULL,2,NULL);
	xTaskCreate(Task3, "Task3", 130, NULL,2,NULL);
	vTaskDelete(NULL);
	
	taskEXIT_CRITICAL();			//退出临界区
}

int main()
{
	Delay_Init();
	LED_Init();
//	MPU6050_Init();
	Usart_Init();
//	EQUIPMENT_Init();
	W25Q128JV_Init();
//	NRF24L01_Init();
	KEY_Init();
	LCD_Init();
//	TIME3x_Init();
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		//中断优先级组4

	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	
	xTaskCreate(SystemCreatTask,"SystemCreatTask",180,NULL,2,&SystemCreatTask_Handler);		
	vTaskStartScheduler();
	
	while(1)
	{
			
	}
}    
             