#include "TIMERx.h"

/****
	* @brief    ��ʱ��2�ж� ����			  
	* @param   	��
	* @return   ��    	
	* Sample usage:TIMEx_NVIC_Config(); 
    */
static void TIMEx_NVIC_Config()
{
    NVIC_InitTypeDef NVIC_InitStruct;
    //��������
    NVIC_InitStruct.NVIC_IRQChannel = TIMEx_IRQChannel;      //�ж�ͨ��
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             //�ж�ͨ��ʹ��
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = TIME_x_PreemptionPriority;   //��ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = TIME_x_SubPriority;          //�����ȼ�
    NVIC_Init(&NVIC_InitStruct);
}

/****
	* @brief    ��ʱ��2�ж� ����			  
	* @param   	��
	* @return   ��    	
	* Sample usage:TIMEx_NVIC_Config(); 
    */
static void TIME3x_NVIC_Config()
{
    NVIC_InitTypeDef NVIC_InitStruct;
    //��������
    NVIC_InitStruct.NVIC_IRQChannel = TIME3x_IRQChannel;      //�ж�ͨ��
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             //�ж�ͨ��ʹ��
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = TIME3_x_PreemptionPriority;   //��ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = TIME3_x_SubPriority;          		//�����ȼ�
    NVIC_Init(&NVIC_InitStruct);
}

/****
	* @brief    ��ʱ��2 ����		1ms	  
	* @param   	��
	* @return   ��    	
	* Sample usage:TIMEx_Config(); 
    */
static void TIMEx_Config()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    //������ʱ��ʱ��
    TIMEx_APBxClockCmd(TIMEx_CLK,ENABLE);
    //ѡ��Ϊ�ڲ�ʱ�ӣ�Ĭ���ڲ�ʱ�ӣ�
    TIM_InternalClockConfig(TIME_x);
    //��������
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        //ʱ�ӷ�Ƶ 1��Ƶ
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    //����ģʽ     ���ϼ���
    TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;                   //ARR   �Զ���װ
    TIM_TimeBaseInitStruct.TIM_Prescaler = 90 - 1;                  //PSC   Ԥ��Ƶ
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;               //�ظ�������
    TIM_TimeBaseInit(TIME_x,&TIM_TimeBaseInitStruct);

    TIM_ClearFlag(TIME_x,TIM_IT_Update);

    TIM_ITConfig(TIME_x,TIM_IT_Update,ENABLE);      //��ʱ��ʹ�ܸ����ж�

    //ʹ�ܶ�ʱ��
    TIM_Cmd(TIME_x,ENABLE); 
}

/****
	* @brief    ��ʱ��3 ����			  
	* @param   	��
	* @return   ��    	
	* Sample usage:TIMEx_Config(); 
    */
static void TIME3x_Config()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    //������ʱ��ʱ��
    TIMEx_APBxClockCmd(TIME3x_CLK,ENABLE);
    //ѡ��Ϊ�ڲ�ʱ�ӣ�Ĭ���ڲ�ʱ�ӣ�
    TIM_InternalClockConfig(TIME3_x);
    //��������
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        //ʱ�ӷ�Ƶ 1��Ƶ
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    //����ģʽ     ���ϼ���
    TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;                   //ARR   �Զ���װ
    TIM_TimeBaseInitStruct.TIM_Prescaler = 90 - 1;                 //PSC   Ԥ��Ƶ
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;               //�ظ�������
    TIM_TimeBaseInit(TIME3_x,&TIM_TimeBaseInitStruct);

    TIM_ClearFlag(TIME3_x,TIM_IT_Update);

    TIM_ITConfig(TIME3_x,TIM_IT_Update,ENABLE);      //��ʱ��ʹ�ܸ����ж�

    //ʹ�ܶ�ʱ��
    TIM_Cmd(TIME3_x,ENABLE); 
}


/****
	* @brief    ��ʱ��2 ��ʼ��		  
	* @param   	��
	* @return   ��    	
	* Sample usage:TIMEx_Init(); 
    */
void TIMEx_Init()
{
    TIMEx_Config();
    TIMEx_NVIC_Config();
}

/****
	* @brief    ��ʱ��3 ��ʼ��		  
	* @param   	��
	* @return   ��    	
	* Sample usage:TIMEx_Init(); 
    */
void TIME3x_Init()
{
    TIME3x_Config();
    TIME3x_NVIC_Config();
}

	