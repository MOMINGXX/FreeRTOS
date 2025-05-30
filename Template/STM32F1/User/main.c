#include "stm32f10x.h"                  // Device header
#include "USART.h"
#include "FreeRTOS.h"
#include "task.h"

void Delay(uint16_t t)
{
	while(t--);
	return;
}

void Delay_us(uint16_t t) 
{  
   while(t--) 
	{
      Delay(6);    
   }	
}

void Delay_ms(uint16_t t) 
{ 
   while(t--) 
		{
      Delay_us(1000);    
   }
}

void Task1(void *pvParameters);
void Task2(void *pvParameters);

void Schedule_Task(void *pvParameters)
{
	xTaskCreate(Task1,"Task1",128,NULL,2,NULL);
	xTaskCreate(Task2,"Task2",128,NULL,2,NULL);
	vTaskDelete(NULL);
}
 
void Task1(void *pvParameters)
{
	uint8_t Task1Flag = 0;
	while(1)
	{
		printf("Task1 Run Frequency = %d!!!\t\t\r\n",++Task1Flag);
		Delay_ms(10);
//		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

void Task2(void *pvParameters)
{
	uint8_t Task2Flag = 0;
	while(1)
	{
		printf("Task2 Run Frequency = %d!!!\t\t\r\n",++Task2Flag);
		Delay_ms(10);
//		vTaskDelay(pdMS_TO_TICKS(200));
	}
}

int main()
{
	Usart_Init();
	xTaskCreate(Schedule_Task,"Schedule_Task",128,NULL,1,NULL);
	vTaskStartScheduler();
	while(1)
	{
		
	}
}
