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

TaskHandle_t Task1_Handler;
TaskHandle_t Task2_Handler;
TimerHandle_t TIMER1_Handler;
TimerHandle_t TIMER2_Handler;
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

void PWM()
{
	uint8_t  count = 0;
	count ++;
	if(count < 20)
	{
		LED_LEFT(ON);
		LED_MIDDLE(ON);
	}
	else
	{
		LED_LEFT(OFF);
		LED_MIDDLE(OFF);
	}
	if(count > 100)	count = 0;
}

void vTimer1Callback( TimerHandle_t pxTimer )
{
	PWM();
}

void vTimer2Callback( TimerHandle_t pxTimer )
{
	LED_MIDDLE_TOGGLE;
}

void Task1(void *pvParameters)
{	
	uint8_t Key_Number = 0;
    while(1)
    {
		xQueueReceive(Key_QueueHandle,&Key_Number,portMAX_DELAY);
		if(Key_Number == 1)
		{
			xTimerStart(TIMER1_Handler,portMAX_DELAY);	
			//xTimerStart(TIMER2_Handler,portMAX_DELAY);	
		}
		else if(Key_Number == 2)
		{
			xTimerStop(TIMER1_Handler,portMAX_DELAY);
			//xTimerStop(TIMER2_Handler,portMAX_DELAY);
		}
		else if(Key_Number == 3)
		{
		    xTimerChangePeriod(TIMER1_Handler,500,portMAX_DELAY);
		}
	}	
}

//接受键值
void Task2(void *pvParameters)
{ 
    while(1)
    {

	}
}

void SystemCreatTask(void *pvParameters)
{
	taskENTER_CRITICAL();			//进入临界区

	TIMER1_Handler = xTimerCreate("Timer1",100,pdTRUE,(void *)1,vTimer1Callback);
	if(TIMER1_Handler != NULL)
	{
	    dynamic_display_stack_info("创建成功");
	}

	/*TIMER2_Handler = xTimerCreate("Timer2",1000,pdTRUE,(void *)2,vTimer2Callback);
	if(TIMER2_Handler != NULL)
	{
	    dynamic_display_stack_info("创建成功");
	}*/

	xTaskCreate(Task1, "Task1", 130, NULL,2,&Task1_Handler);
	xTaskCreate(vKey_Task, "vKey_Task", 130, NULL,2,NULL);
	xTaskCreate(Task2, "Task2", 130, NULL,2,&Task2_Handler);
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
             