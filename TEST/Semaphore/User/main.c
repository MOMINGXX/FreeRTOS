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

// ����ȫ�ֱ�����¼��ǰ��ʾ��
static uint16_t current_line = 28;
#define MAX_LINES 10  // �������LCD�߶ȵ���
#define LINE_HEIGHT 24 // ÿ�и߶�

// ��̬��ʾ�����ַ�����
void dynamic_display_stack_info(char *text)
{
    // ���������Ļ��Χ�������к�
    if(current_line >= 280) {
        current_line = 28;
    }
    
    // ��ʾ��ջ��Ϣ
    Display_GB2312_String(0, current_line, 24, (uint8_t *)text, BLACK, WHITE);
    
    // �ƶ�����һ��
    current_line += LINE_HEIGHT;
}

void Task1(void *pvParameters)
{	
	uint8_t Key_Number = 0;
    while(1)
    {
//		xQueueReceive(Key_QueueHandle,&Key_Number,portMAX_DELAY);
		Key_Number = Key_GetNumber();
//		xSemaphoreTake(Semaphore_Handler,portMAX_DELAY);
		
		if(Key_Number == 1)
		{
			dynamic_display_stack_info("һ���°���");
			printf("Key_Number: %d����\r\n",Key_Number);
		}
		xSemaphoreGive(Semaphore_Handler);
		vTaskDelay(pdMS_TO_TICKS(100)); 
	}
}

//���ܼ�ֵ
void Task2(void *pvParameters)
{ 
	uint8_t Key_Number = 0;
    while(1)
    {
		xQueueReceive(Key_QueueHandle,&Key_Number,portMAX_DELAY);
		xSemaphoreTake(Semaphore_Handler,portMAX_DELAY);
		
		if(Key_Number == 2)
		{
			dynamic_display_stack_info("�����°���");
			printf("Key_Number: %d����\r\n",Key_Number);
		}
		xSemaphoreGive(Semaphore_Handler);
		vTaskDelay(pdMS_TO_TICKS(10)); 
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
	taskENTER_CRITICAL();			//�����ٽ���

	Semaphore_Handler = xSemaphoreCreateBinary();
	if(NULL != Semaphore_Handler)
	{
		printf("�����ɹ�\r\n");
		Display_GB2312_String(0, current_line, 24, (uint8_t*)"�����ɹ�", BLACK, WHITE);
	}
	xSemaphoreGive(Semaphore_Handler);
	xTaskCreate(Task1, "Task1", 130, NULL,2,&Task1_Handler);
//	xTaskCreate(vKey_Task, "vKey_Task", 130, NULL,2,NULL);
//	xTaskCreate(Task2, "Task2", 130, NULL,2,NULL);
	xTaskCreate(Task3, "Task3", 130, NULL,2,NULL);
	vTaskDelete(NULL);
	
	taskEXIT_CRITICAL();			//�˳��ٽ���
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
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		//�ж����ȼ���4

	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	
	xTaskCreate(SystemCreatTask,"SystemCreatTask",180,NULL,2,&SystemCreatTask_Handler);		
	vTaskStartScheduler();
	
	while(1)
	{
			
	}
}    
             