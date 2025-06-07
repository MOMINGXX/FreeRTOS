#include "Delay.h"

static uint32_t fac_us = 0;         // us延时倍乘数
static uint16_t fac_ms = 0;        // ms延时倍乘数

extern void xPortSysTickHandler(void);
//systick中断服务函数,使用OS时用到
void SysTick_Handler(void)
{	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
    {
        xPortSysTickHandler();	
    }
}

/****
	* @brief	延时 初始化    			  
	* @param   	无   
	* @return   无  	
	* Sample usage:Delay_Init();
    */
void Delay_Init()
{
	uint32_t reload;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//选择内部时钟  HCLK
	fac_us = SystemCoreClock / 1000000;										//不论是否使用OS,fac_us都需要使用
	reload = SystemCoreClock /configTICK_RATE_HZ;       //根据configTICK_RATE_HZ设定溢出时间
														//reload为24位寄存器,最大值:16777216,在72M下,约合0.233s左右    
	fac_ms = 1000/configTICK_RATE_HZ;             	   	//代表OS可以延时的最少单位       
	
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;    	   	//开启SYSTICK中断
	SysTick->LOAD = reload;                       	  	//每1/configTICK_RATE_HZ秒中断一次    
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;     	  	//开启SYSTICK    
}

/****
	* @brief	延时 微秒    			  
	* @param   	time    延时时间
	* @return   无  	
	* Sample usage:Delay_us(100);
    */
void Delay_us(uint32_t time)
{
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks = time * fac_us; 						//需要的节拍数	  		 
	told=SysTick->VAL;        					//刚进入时的计数器值
	while(1)
	{
		tnow = SysTick -> VAL;	
		if(tnow != told)
		{	    
			if(tnow < told)tcnt += told - tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt += reload - tnow + told;	    
			told = tnow;
			if(tcnt >= ticks)
				break;							//时间超过/等于要延迟的时间,则退出.
		}  
	};
}

/****
	* @brief	延时 毫秒  			  
	* @param   	time    延时时间
	* @return   无  	
	* Sample usage:Delay_ms(100);
    */
void Delay_ms(uint32_t time)
{
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行	    
	{		 
		if(time >= fac_ms)						//延时的时间大于OS的最少时间周期 
		{ 
   			vTaskDelay(time / fac_ms);	 		//FreeRTOS延时
		}
		time %= fac_ms;						//OS已经无法提供这么小的延时了,采用普通方式延时    
	}
	
	Delay_us((uint32_t)(time * 1000));				//普通方式延时
}

/****
	* @brief	延时 秒 			  
	* @param   	time    延时时间
	* @return   无  	
	* Sample usage:Delay_s(100);
    */
void Delay_s(uint16_t time)
{
	while(time--)
	{
		Delay_ms(1000);
	}
}
