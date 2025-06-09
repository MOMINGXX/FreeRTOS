#include "TIMERx.h"

/****
	* @brief    定时器2中断 配置			  
	* @param   	无
	* @return   无    	
	* Sample usage:TIMEx_NVIC_Config(); 
    */
static void TIMEx_NVIC_Config()
{
    NVIC_InitTypeDef NVIC_InitStruct;
    //参数配置
    NVIC_InitStruct.NVIC_IRQChannel = TIMEx_IRQChannel;      //中断通道
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             //中断通道使能
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = TIME_x_PreemptionPriority;   //抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = TIME_x_SubPriority;          //子优先级
    NVIC_Init(&NVIC_InitStruct);
}

/****
	* @brief    定时器2中断 配置			  
	* @param   	无
	* @return   无    	
	* Sample usage:TIMEx_NVIC_Config(); 
    */
static void TIME3x_NVIC_Config()
{
    NVIC_InitTypeDef NVIC_InitStruct;
    //参数配置
    NVIC_InitStruct.NVIC_IRQChannel = TIME3x_IRQChannel;      //中断通道
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             //中断通道使能
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = TIME3_x_PreemptionPriority;   //抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = TIME3_x_SubPriority;          		//子优先级
    NVIC_Init(&NVIC_InitStruct);
}

/****
	* @brief    定时器2 配置		1ms	  
	* @param   	无
	* @return   无    	
	* Sample usage:TIMEx_Config(); 
    */
static void TIMEx_Config()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    //开启定时器时钟
    TIMEx_APBxClockCmd(TIMEx_CLK,ENABLE);
    //选择为内部时钟（默认内部时钟）
    TIM_InternalClockConfig(TIME_x);
    //参数配置
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        //时钟分频 1分频
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    //计数模式     向上计数
    TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;                   //ARR   自动重装
    TIM_TimeBaseInitStruct.TIM_Prescaler = 90 - 1;                  //PSC   预分频
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;               //重复计数器
    TIM_TimeBaseInit(TIME_x,&TIM_TimeBaseInitStruct);

    TIM_ClearFlag(TIME_x,TIM_IT_Update);

    TIM_ITConfig(TIME_x,TIM_IT_Update,ENABLE);      //定时器使能更新中断

    //使能定时器
    TIM_Cmd(TIME_x,ENABLE); 
}

/****
	* @brief    定时器3 配置			  
	* @param   	无
	* @return   无    	
	* Sample usage:TIMEx_Config(); 
    */
static void TIME3x_Config()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    //开启定时器时钟
    TIMEx_APBxClockCmd(TIME3x_CLK,ENABLE);
    //选择为内部时钟（默认内部时钟）
    TIM_InternalClockConfig(TIME3_x);
    //参数配置
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        //时钟分频 1分频
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    //计数模式     向上计数
    TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;                   //ARR   自动重装
    TIM_TimeBaseInitStruct.TIM_Prescaler = 90 - 1;                 //PSC   预分频
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;               //重复计数器
    TIM_TimeBaseInit(TIME3_x,&TIM_TimeBaseInitStruct);

    TIM_ClearFlag(TIME3_x,TIM_IT_Update);

    TIM_ITConfig(TIME3_x,TIM_IT_Update,ENABLE);      //定时器使能更新中断

    //使能定时器
    TIM_Cmd(TIME3_x,ENABLE); 
}


/****
	* @brief    定时器2 初始化		  
	* @param   	无
	* @return   无    	
	* Sample usage:TIMEx_Init(); 
    */
void TIMEx_Init()
{
    TIMEx_Config();
    TIMEx_NVIC_Config();
}

/****
	* @brief    定时器3 初始化		  
	* @param   	无
	* @return   无    	
	* Sample usage:TIMEx_Init(); 
    */
void TIME3x_Init()
{
    TIME3x_Config();
    TIME3x_NVIC_Config();
}

	