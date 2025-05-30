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
#include "semphr.h"
#include "TIMERx.h"

TaskHandle_t Task1_Handler;
TaskHandle_t Task2_Handler;
TaskHandle_t Task3_Handler;
TaskHandle_t Task4_Handler;
TaskHandle_t SystemCreatTask_Handler;

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
	uint8_t Key_Number = 0;
    while(1)
    {
		Key_Number = Key_GetNumber();
		if(Key_Number != 0)				//信号量											
		{
		    if(Key_Number == 1)
			{
				printf("任务二通知发送！！！\r\n");
				dynamic_display_stack_info("发送任务通知!");
				xTaskNotifyGive(Task2_Handler);
			}
			else if(Key_Number == 2)		//事件标志组
			{
				xTaskNotify(Task3_Handler,0x01,eSetBits);
			}
			else if(Key_Number == 3)		//事件标志组
			{
				xTaskNotify(Task3_Handler,0x02,eSetBits);
			}
			else xTaskNotify(Task4_Handler,Key_Number,eSetValueWithOverwrite);			//队列
		}
		vTaskDelay(pdMS_TO_TICKS(100));
	}	
}

//接受键值
void Task2(void *pvParameters)
{ 
	uint32_t xReturn;
    while(1)
    {
		xReturn = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);		//pdTRUE为清除通知值	pdFALSE 通知值减一
		if(xReturn)
		{
			printf("接收任务通知成功!!!\r\n");
			dynamic_display_stack_info("接收任务通知成功!");
		}
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

void Task3(void *pvParameters)
{
	uint32_t xReturn;
	while(1)
	{
		LED_LEFT_TOGGLE;
		vTaskDelay(pdMS_TO_TICKS(1000));
		xTaskNotifyWait(0,0,&xReturn, portMAX_DELAY);
		if(xReturn & 0x01)
		{
		    LED_MIDDLE_TOGGLE;
		}
		if(xReturn & 0x02)
		{
		    LED_RIGHT_TOGGLE;
		}
	}
}

void Task4(void *pvParameters)
{
	uint32_t xReturn;
	while(1)
	{
		xTaskNotifyWait(0,0,&xReturn, portMAX_DELAY);
		LCD_ShowString(20,24,"Key_Number = ",BLACK,WHITE,24,0);
		LCD_ShowIntNum(180,24,xReturn,2,BLACK,WHITE,24);
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void SystemCreatTask(void *pvParameters)
{
	taskENTER_CRITICAL();			//进入临界区

	xTaskCreate(Task1, "Task1", 130, NULL,2,&Task1_Handler);
	xTaskCreate(vKey_Task, "vKey_Task", 130, NULL,2,NULL);
	xTaskCreate(Task2, "Task2", 130, NULL,2,&Task2_Handler);
	xTaskCreate(Task3, "Task3", 130, NULL,2,&Task3_Handler);
	xTaskCreate(Task4, "Task4", 130, NULL,2,&Task4_Handler);
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
             