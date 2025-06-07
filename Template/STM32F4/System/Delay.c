#include "Delay.h"

static uint32_t fac_us = 0;         // us��ʱ������
static uint16_t fac_ms = 0;        // ms��ʱ������

extern void xPortSysTickHandler(void);
//systick�жϷ�����,ʹ��OSʱ�õ�
void SysTick_Handler(void)
{	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
    {
        xPortSysTickHandler();	
    }
}

/****
	* @brief	��ʱ ��ʼ��    			  
	* @param   	��   
	* @return   ��  	
	* Sample usage:Delay_Init();
    */
void Delay_Init()
{
	uint32_t reload;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//ѡ���ڲ�ʱ��  HCLK
	fac_us = SystemCoreClock / 1000000;										//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
	reload = SystemCoreClock /configTICK_RATE_HZ;       //����configTICK_RATE_HZ�趨���ʱ��
														//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��0.233s����    
	fac_ms = 1000/configTICK_RATE_HZ;             	   	//����OS������ʱ�����ٵ�λ       
	
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;    	   	//����SYSTICK�ж�
	SysTick->LOAD = reload;                       	  	//ÿ1/configTICK_RATE_HZ���ж�һ��    
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;     	  	//����SYSTICK    
}

/****
	* @brief	��ʱ ΢��    			  
	* @param   	time    ��ʱʱ��
	* @return   ��  	
	* Sample usage:Delay_us(100);
    */
void Delay_us(uint32_t time)
{
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks = time * fac_us; 						//��Ҫ�Ľ�����	  		 
	told=SysTick->VAL;        					//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow = SysTick -> VAL;	
		if(tnow != told)
		{	    
			if(tnow < told)tcnt += told - tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt += reload - tnow + told;	    
			told = tnow;
			if(tcnt >= ticks)
				break;							//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
}

/****
	* @brief	��ʱ ����  			  
	* @param   	time    ��ʱʱ��
	* @return   ��  	
	* Sample usage:Delay_ms(100);
    */
void Delay_ms(uint32_t time)
{
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����	    
	{		 
		if(time >= fac_ms)						//��ʱ��ʱ�����OS������ʱ������ 
		{ 
   			vTaskDelay(time / fac_ms);	 		//FreeRTOS��ʱ
		}
		time %= fac_ms;						//OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	
	Delay_us((uint32_t)(time * 1000));				//��ͨ��ʽ��ʱ
}

/****
	* @brief	��ʱ �� 			  
	* @param   	time    ��ʱʱ��
	* @return   ��  	
	* Sample usage:Delay_s(100);
    */
void Delay_s(uint16_t time)
{
	while(time--)
	{
		Delay_ms(1000);
	}
}
