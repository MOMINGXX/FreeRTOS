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
TaskHandle_t SystemCreatTask_Handler;

QueueHandle_t Semaphore_Handler;
QueueSetHandle_t QueueText_Handle;
QueueSetHandle_t QueueSet_Handler;

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
	uint8_t Key_Value = 0;
    while(1)
    {
		
		xQueueReceive(Key_QueueHandle,&Key_Value,portMAX_DELAY);
		if(Key_Value == 1)
		{
			xSemaphoreGive(Semaphore_Handler);
			dynamic_display_stack_info("释放信号量");
		}
		else if(Key_Value ==2)
		{
			xQueueSend(QueueText_Handle,&Key_Value,0);
			dynamic_display_stack_info("释放队列");
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

//接受键值
void Task2(void *pvParameters)
{ 
	QueueSetMemberHandle_t xActivatedMember;
	uint8_t Key_Value;
    while(1)
    {
		xActivatedMember = xQueueSelectFromSet(QueueSet_Handler,portMAX_DELAY);
		if(xActivatedMember == QueueText_Handle)
		{
			xQueueReceive(QueueText_Handle,&Key_Value,portMAX_DELAY);
			printf("队列状态: 剩余=%ld, 等待任务=%ld\n", 
			uxQueueSpacesAvailable(QueueText_Handle),
			uxQueueMessagesWaiting(QueueText_Handle));
		}
		else if(xActivatedMember == Semaphore_Handler)
		{
			xSemaphoreTake(Semaphore_Handler,portMAX_DELAY);
			printf("信号量获取\r\n");
			LED_RIGHT_TOGGLE;
		}
	}
}

void Task3(void *pvParameters)
{
	while(1)
	{
		LED_LEFT_TOGGLE;
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void SystemCreatTask(void *pvParameters)
{
	taskENTER_CRITICAL();			//进入临界区

	QueueSet_Handler = xQueueCreateSet(2);						//创建队列集	事件数 2
	if(NULL != QueueSet_Handler)
	{
		printf("队列集创建成功\r\n");
		Display_GB2312_String(0, current_line * 1, 24, (uint8_t*)"队列集创建成功", BLACK, WHITE);
	}
	QueueText_Handle = xQueueCreate(5,sizeof(uint8_t));
	if(NULL != QueueText_Handle)
	{
		printf("队列创建成功\r\n");
		Display_GB2312_String(0, current_line * 2, 24, (uint8_t*)"队列创建成功", BLACK, WHITE);
	}
	Semaphore_Handler = xSemaphoreCreateBinary();				//创建二值信号量
	if(NULL != Semaphore_Handler)
	{
		printf("信号量创建成功\r\n");
		Display_GB2312_String(0, current_line * 3, 24, (uint8_t*)"创建成功", BLACK, WHITE);
	}
	if(xQueueAddToSet(Semaphore_Handler,QueueSet_Handler))		//将二值信号量添加到队列集
	{
		printf("信号量添加成功\r\n");
		Display_GB2312_String(0, current_line*4, 24, (uint8_t*)"信号量添加成功", BLACK, WHITE);
	}
	if(xQueueAddToSet(QueueText_Handle,QueueSet_Handler))		//将二值信号量添加到队列集
	{
		printf("队列添加成功\r\n");
		Display_GB2312_String(0, current_line*5, 24, (uint8_t*)"队列添加成功", BLACK, WHITE);
	}
	xTaskCreate(Task1, "Task1", 130, NULL,2,&Task1_Handler);
	xTaskCreate(vKey_Task, "vKey_Task", 130, NULL,3,NULL);
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
             