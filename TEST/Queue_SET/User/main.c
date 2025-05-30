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
	uint8_t Key_Value = 0;
    while(1)
    {
		
		xQueueReceive(Key_QueueHandle,&Key_Value,portMAX_DELAY);
		if(Key_Value == 1)
		{
			xSemaphoreGive(Semaphore_Handler);
			dynamic_display_stack_info("�ͷ��ź���");
		}
		else if(Key_Value ==2)
		{
			xQueueSend(QueueText_Handle,&Key_Value,0);
			dynamic_display_stack_info("�ͷŶ���");
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

//���ܼ�ֵ
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
			printf("����״̬: ʣ��=%ld, �ȴ�����=%ld\n", 
			uxQueueSpacesAvailable(QueueText_Handle),
			uxQueueMessagesWaiting(QueueText_Handle));
		}
		else if(xActivatedMember == Semaphore_Handler)
		{
			xSemaphoreTake(Semaphore_Handler,portMAX_DELAY);
			printf("�ź�����ȡ\r\n");
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
	taskENTER_CRITICAL();			//�����ٽ���

	QueueSet_Handler = xQueueCreateSet(2);						//�������м�	�¼��� 2
	if(NULL != QueueSet_Handler)
	{
		printf("���м������ɹ�\r\n");
		Display_GB2312_String(0, current_line * 1, 24, (uint8_t*)"���м������ɹ�", BLACK, WHITE);
	}
	QueueText_Handle = xQueueCreate(5,sizeof(uint8_t));
	if(NULL != QueueText_Handle)
	{
		printf("���д����ɹ�\r\n");
		Display_GB2312_String(0, current_line * 2, 24, (uint8_t*)"���д����ɹ�", BLACK, WHITE);
	}
	Semaphore_Handler = xSemaphoreCreateBinary();				//������ֵ�ź���
	if(NULL != Semaphore_Handler)
	{
		printf("�ź��������ɹ�\r\n");
		Display_GB2312_String(0, current_line * 3, 24, (uint8_t*)"�����ɹ�", BLACK, WHITE);
	}
	if(xQueueAddToSet(Semaphore_Handler,QueueSet_Handler))		//����ֵ�ź�����ӵ����м�
	{
		printf("�ź�����ӳɹ�\r\n");
		Display_GB2312_String(0, current_line*4, 24, (uint8_t*)"�ź�����ӳɹ�", BLACK, WHITE);
	}
	if(xQueueAddToSet(QueueText_Handle,QueueSet_Handler))		//����ֵ�ź�����ӵ����м�
	{
		printf("������ӳɹ�\r\n");
		Display_GB2312_String(0, current_line*5, 24, (uint8_t*)"������ӳɹ�", BLACK, WHITE);
	}
	xTaskCreate(Task1, "Task1", 130, NULL,2,&Task1_Handler);
	xTaskCreate(vKey_Task, "vKey_Task", 130, NULL,3,NULL);
	xTaskCreate(Task2, "Task2", 130, NULL,2,NULL);
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
             