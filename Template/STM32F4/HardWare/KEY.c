#include "KEY.h"

uint8_t KeyNumber;
KEY_STATEFLAG Key_Flag[KEY_COUNT];
QueueHandle_t Key_QueueHandle;    // 按键队列句柄 
TaskHandle_t Key_NotifyHandle;   // 按键通知任务句柄 

/****
	* @brief    KEY 引脚 配置	
	* @param   	无
	* @return   无    	
	* Sample usage:KEY_Config(); 
    */
static void KEY_Config()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    //开启GPIO时钟
	KEY_APBx_ClockCMD(KEYx_CLK,ENABLE);
    //参数配置                                                                                   
    GPIO_InitStruct.GPIO_Pin =  ROUKER_RIGHT_GPIO_PIN | KEY_X_GPIO_PIN;                    
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;                                                 
    GPIO_Init(KEY1x_GPIO_PORT,&GPIO_InitStruct);                                                   
																							
    GPIO_InitStruct.GPIO_Pin = KEY_UP_GPIO_PIN | KEY_LEFT_GPIO_PIN | 
							   KEY_FRONTR_GPIO_PIN | ROUKER_LEFT_GPIO_PIN;        
    GPIO_Init(KEY2x_GPIO_PORT,&GPIO_InitStruct);                                                   
																									
    GPIO_InitStruct.GPIO_Pin = KEY_DOWN_GPIO_PIN | KEY_RIGHT_GPIO_PIN |KEY_X_GPIO_PIN |
							   KEY_Y_GPIO_PIN | KEY_A_GPIO_PIN | KEY_B_GPIO_PIN;                                                     
    GPIO_Init(KEY1_GPIO_PORT,&GPIO_InitStruct);                                                    
}         

/****
	* @brief    KEY 初始化	
	* @param   	无
	* @return   无    	
	* Sample usage:KEY_Init(); 
    */
void KEY_Init()
{
	//TIMEx_Init();
    KEY_Config();
#ifdef KEY_USE_QUEUE
	Key_QueueHandle = xQueueCreate(1,sizeof(uint8_t));		//创建按键队列
	if(Key_QueueHandle == NULL)							    //创建失败
	{
		printf("Key_QueueHandle Create Fail!\r\n");          
	}
#endif
}                                                                                         

#ifdef KEY_USE_TIMER
/****
	* @brief    返回按键号			  
	* @param   	无
	* @return   Temp   按键号 	
	* Sample usage:Key_GetNumber(); 
    */
uint8_t Key_GetNumber()
{
    uint8_t Temp = 0;
    if(KeyNumber)
    {
        Temp = KeyNumber;
        KeyNumber = 0;
        return Temp;
    }
    return 0;
}

/****
	* @brief    读取按键状态			    
	* @param   	无
	* @return   Number  返回按键状态    	  	
	* Sample usage:KEY_ReadNumber(); 
    */
static uint8_t KEY_ReadNumber()
{
    if(GPIO_ReadInputDataBit(KEY1x_GPIO_PORT,KEY_FRONTL_GPIO_PIN) == RESET)    {return 1;}
    if(GPIO_ReadInputDataBit(KEY2x_GPIO_PORT,KEY_FRONTR_GPIO_PIN) == RESET)    {return 2;}
    if(GPIO_ReadInputDataBit(KEY1x_GPIO_PORT,ROUKER_RIGHT_GPIO_PIN) == RESET)  {return 3;}
    if(GPIO_ReadInputDataBit(KEY2x_GPIO_PORT,ROUKER_LEFT_GPIO_PIN) == RESET)   {return 4;}
    if(GPIO_ReadInputDataBit(KEY2x_GPIO_PORT,KEY_UP_GPIO_PIN) == RESET)        {return 5;}
    if(GPIO_ReadInputDataBit(KEY2x_GPIO_PORT,KEY_LEFT_GPIO_PIN) == RESET)      {return 6;}
    if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_DOWN_GPIO_PIN) == RESET)       {return 7;}
    if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_RIGHT_GPIO_PIN) == RESET)      {return 8;}
    if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_Y_GPIO_PIN) == RESET)          {return 9;}
    if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_X_GPIO_PIN) == RESET)          {return 10;}
    if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_A_GPIO_PIN) == RESET)          {return 11;}
    if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_B_GPIO_PIN) == RESET)          {return 12;}
	return 0;                                
}

/****
	* @brief    定时器扫描	非阻塞式	可读取全部按键	    
	* @param   	无
	* @return   无   	
	* Sample usage:KEY_Tick(); 
    */
void KEY_Tick()
{
    static uint8_t Count;                                      //定义计数变量
    static uint8_t CurrentState,PrevState;                         //定义当前状态和上一个状态变量
    Count ++;                                               //计数变量自加
    if(Count >= 20)                                         //延时20ms
    {
        Count = 0;                                          //计数变量清零       
        PrevState = CurrentState;                           //将当前状态赋值给上一个状态变量
        CurrentState = KEY_ReadNumber();                    //读取按键状态
        if(CurrentState == 0  && PrevState != 0)		    //如果当前状态为0 ,上一个状态不为0，说明按键按下
        {
            KeyNumber = PrevState;                       	//将按键号赋值给全局变量
        }
    }
}

/****
	* @brief    定时器扫描	非阻塞式		  可连续读取按键
	* @param   	无
	* @return   无   	
	* Sample usage:KEY_Tick(); 
    */
/*void KEY_Tick()             
{
    static uint8_t NowState,LastState;
    static uint8_t Count;                                    //定义计数变量
    Count ++;                                               //计数变量自加
    if(Count >= 20)                                         //延时20ms
    { 
        LastState = NowState;
        NowState = KEY_ReadNumber();
        if(LastState == 0  && NowState == 1)		{KeyNumber = 1;}
        else if(LastState == 0 && NowState == 2 )	{KeyNumber = 2;}
        else if(LastState == 0 && NowState == 3 )	{KeyNumber = 3;}
        else if(LastState == 0 && NowState == 4 )	{KeyNumber = 4;}
        else if(LastState == 0 && NowState == 5 )	{KeyNumber = 5;}
        else if(LastState == 0 && NowState == 6 )	{KeyNumber = 6;}
        else if(LastState == 0 && NowState == 7 )	{KeyNumber = 7;}
        else if(LastState == 0 && NowState == 8 )	{KeyNumber = 8;}
        else if(LastState == 0 && NowState == 9 )	{KeyNumber = 9;}
        else if(LastState == 0 && NowState == 10)	{KeyNumber = 10;}
        else if(LastState == 0 && NowState == 11)	{KeyNumber = 11;}
        else if(LastState == 0 && NowState == 12)	{KeyNumber = 12;}
    }
}*/

#else 
/****
	* @brief    读取按键状态	延时函数消抖	阻塞式
	* @param   	无
	* @return   Number  返回按键状态    	
	* Sample usage:KEY_ReadNumber(); 
    */
static uint8_t Key_ReadNumber(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
    if(RESET == GPIO_ReadInputDataBit(GPIOx,GPIO_Pin))
    {
        vTaskDelay(10);
        if(RESET == GPIO_ReadInputDataBit(GPIOx,GPIO_Pin))
        {
			vTaskDelay(10);
            return 1;
        }while(!GPIO_ReadInputDataBit(GPIOx,GPIO_Pin));
    }
    return 0;
}

/****
	* @brief    读取按键			 	  阻塞式
	* @param   	无
	* @return   Number  返回按键    	
	* Sample usage:Key_GetNumber(); 
    */
uint8_t Key_GetNumber()
{
    uint8_t Number = 0;
    if(Key_ReadNumber(KEY1x_GPIO_PORT,KEY_FRONTL_GPIO_PIN)) 		{Number = 1;}  
    else if(Key_ReadNumber(KEY2x_GPIO_PORT,KEY_FRONTR_GPIO_PIN))  	{Number = 2;}
    else if(Key_ReadNumber(KEY2x_GPIO_PORT,ROUKER_LEFT_GPIO_PIN))  	{Number = 3;}
    else if(Key_ReadNumber(KEY1x_GPIO_PORT,ROUKER_RIGHT_GPIO_PIN)) 	{Number = 4;}
    else if(Key_ReadNumber(KEY2x_GPIO_PORT,KEY_UP_GPIO_PIN))	    {Number = 5;}
    else if(Key_ReadNumber(KEY2x_GPIO_PORT,KEY_LEFT_GPIO_PIN))  	{Number = 6;}
    else if(Key_ReadNumber(KEY1_GPIO_PORT,KEY_DOWN_GPIO_PIN)) 		{Number = 7;}
    else if(Key_ReadNumber(KEY1_GPIO_PORT,KEY_RIGHT_GPIO_PIN)) 		{Number = 8;}
    else if(Key_ReadNumber(KEY1_GPIO_PORT,KEY_Y_GPIO_PIN)) 			{Number = 9;}
    else if(Key_ReadNumber(KEY1_GPIO_PORT,KEY_X_GPIO_PIN)) 			{Number = 10;}
    else if(Key_ReadNumber(KEY1_GPIO_PORT,KEY_A_GPIO_PIN)) 			{Number = 11;}
    else if(Key_ReadNumber(KEY1_GPIO_PORT,KEY_B_GPIO_PIN)) 			{Number = 12;}
	return Number; 
}
#endif

#ifdef  KEY_USE_QUEUE
/****
	* @brief   FreeRTOS读取按键	入队任务		
	* @param   无
	* @return  无 
    */
void vKey_ReadTask(void *pvParameters)
{
	uint8_t KEY_NUMBER = 0;
    while(1)
    {
		KEY_NUMBER = Key_GetNumber();		//获取按键
		if(KEY_NUMBER != 0)
		{
			xQueueSend(Key_QueueHandle,&KEY_NUMBER,portMAX_DELAY);		//获取键值存入队列
		}
		vTaskDelay(pdMS_TO_TICKS(10)); 
	}
}

#elif defined KEY_USE_NOTIFY
/****
	* @brief   FreeRTOS读取按键	任务通知方式    队列		
	* @param   无
	* @return  无  
    */
void vKey_ReadTask(void *pvParameters)
{
	uint8_t KEY_NUMBER = 0;
    while(1)
    {
		KEY_NUMBER = Key_GetNumber();		//获取键值
		if(KEY_NUMBER != 0)
		{
			xTaskNotify(Key_NotifyHandle,KEY_NUMBER,eSetValueWithOverwrite);		//获取键值存入
		}
		vTaskDelay(pdMS_TO_TICKS(10));
    }
}
#endif

/****
	* @brief   读取按键	引脚电平状态  判断是否按下 		
	* @param   Key_Num  按键号
	* @return  KEY_PRESSED  按键按下  
               KEY_UNPRESSED  按键未按下
    * Sample usage:Key_GetState(1);            
    */
static uint8_t Key_GetState(uint8_t Key_Num)
{
    if(KEY_FRONTL == Key_Num)
    {
        if(GPIO_ReadInputDataBit(KEY1x_GPIO_PORT,KEY_FRONTL_GPIO_PIN) == RESET) return KEY_PRESSED;
    }
    else if(KEY_FRONTR == Key_Num)
    {
        if(GPIO_ReadInputDataBit(KEY2x_GPIO_PORT,KEY_FRONTR_GPIO_PIN) == RESET) return KEY_PRESSED;
    }
    else if(KEY_ROUKER_RIGHT == Key_Num)
    {
        if(GPIO_ReadInputDataBit(KEY1x_GPIO_PORT,ROUKER_RIGHT_GPIO_PIN) == RESET) return KEY_PRESSED;
    }
    else if(KEY_ROUKER_LEFT == Key_Num)
    {
        if(GPIO_ReadInputDataBit(KEY2x_GPIO_PORT,ROUKER_LEFT_GPIO_PIN) == RESET) return KEY_PRESSED;
    }
    else if(KEY_UP == Key_Num)
    {
        if(GPIO_ReadInputDataBit(KEY2x_GPIO_PORT,KEY_UP_GPIO_PIN) == RESET) return KEY_PRESSED;
    }
    else if(KEY_LEFT == Key_Num)
    {
        if(GPIO_ReadInputDataBit(KEY2x_GPIO_PORT,KEY_LEFT_GPIO_PIN) == RESET) return KEY_PRESSED;
    }
    else if(KEY_DOWN == Key_Num)
    {
        if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_DOWN_GPIO_PIN) == RESET) return KEY_PRESSED;
    }
    else if(KEY_RIGHT == Key_Num)
    {
        if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_RIGHT_GPIO_PIN) == RESET) return KEY_PRESSED;
    }
    else if(KEY_Y == Key_Num)
    {
        if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_Y_GPIO_PIN) == RESET) return KEY_PRESSED;
    }
    else if(KEY_X == Key_Num)
    {
        if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_X_GPIO_PIN) == RESET) return KEY_PRESSED;
    }
    else if(KEY_A == Key_Num)
    {
        if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_A_GPIO_PIN) == RESET) return KEY_PRESSED;
    }
    else if(KEY_B == Key_Num)
    {
        if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_B_GPIO_PIN) == RESET) return KEY_PRESSED;
    }
    return KEY_UNPRESSED;
}

/****
	* @brief   检测按键按下状态 或检测按键是否按下
	* @param   KEY_ID  按键号
	* @param   flag  按下状态   单击 双击  长按 释放
	* @return   1  按键按下  0 按键未按下
    * Sample usage:Key_Check(KEY_HOLD);            
    */
uint8_t Key_Check(uint8_t KEY_ID,uint8_t flag)
{   
    if(Key_Flag[KEY_ID] & flag)             //判断按键状态
    {
        if(flag != KEY_HOLD)       
            Key_Flag[KEY_ID] &= ~flag;      //清除标志位
        return 1;
    }
    return 0;
}

/****
	* @brief    定时器扫描	非阻塞式	读取指定按键状态	  
	* @param   	无
	* @return   无   	
	* Sample usage:KEY_Tick(); 
    */
void Key_StateTick()
{
    static uint8_t KeyState[KEY_COUNT],Count,i;
    static uint16_t Time[KEY_COUNT];
    static uint8_t CurrentState[KEY_COUNT],PrevState[KEY_COUNT];  
    
    for(i = 0;i < KEY_COUNT;i++)
    {
        if(Time[i] > 0)   {Time[i]--;}
    }

    Count ++;                                           
    if(Count >= 20)
    {
        for(i = 0;i < KEY_COUNT;i++)
        {
            PrevState[i] = CurrentState[i];
            CurrentState[i] = Key_GetState(i);
            if(CurrentState[i] == KEY_PRESSED) Key_Flag[i] |= KEY_HOLD;
            else Key_Flag[i] &= ~KEY_HOLD;
            if(CurrentState[i] == KEY_PRESSED && PrevState[i] == KEY_UNPRESSED) Key_Flag[i] |= KEY_PRESS;
            if(CurrentState[i] == KEY_UNPRESSED && PrevState[i] == KEY_PRESSED) Key_Flag[i] |= KEY_FREED;

            if(KeyState[i] == 0)
            {
                if(CurrentState[i] == KEY_PRESSED) 
                {
                    Time[i] = KEY_LONG_TIME;
                    KeyState[i] = 1;
                }
            }
            else if(KeyState[i] == 1)
            {
                if(CurrentState[i] == KEY_UNPRESSED) 
                {
                    Time[i] = KEY_DOUBLE_TIME;
                    KeyState[i] = 2;
                }
                else if(Time[i] == 0) 
                {
                    Time[i] = KEY_REPEAT_TIME;
                    Key_Flag[i] |= KEY_LONG;
                    KeyState[i] = 4;
                }
            }
            else if(KeyState[i] == 2)
            {
                if(CurrentState[i] == KEY_PRESSED) 
                {
                    Key_Flag[i] |= KEY_DOUBLE;
                    KeyState[i] = 3;
                }
                else if(Time[i] == 0)
                {
                    Key_Flag[i] |= KEY_SINGLE;
                    KeyState[i] = 0;
                }
            }
            else if(KeyState[i] == 3)
            {
                if(CurrentState[i] == KEY_UNPRESSED) {KeyState[i] = 0;}
            }
            else if(KeyState[i] == 4)
            {
                if(CurrentState[i] == KEY_UNPRESSED) {KeyState[i] = 0;}
                else if(Time[i] == 0) 
                {
                    Time[i] = KEY_REPEAT_TIME;
                    Key_Flag[i] |= KEY_REPEAT; 
                    KeyState[i] = 4;
                }
            }
        }
    }
}

//void Key_Send(uint8_t KetNum)
//{
//    uint8_t KEY_BUFF[16] = " 0 ";
//	KEY_BUFF[0] = 'K';
//	KEY_BUFF[1] = 'Y';
//    if(KetNum)
//    {
//        if(1 == KetNum) { KEY_BUFF[2] = 1;}
//        else if(2 == KetNum) { KEY_BUFF[3] = 2;}
//        else if(3 == KetNum) { KEY_BUFF[4] = 3;}
//        else if(4 == KetNum) { KEY_BUFF[5] = 4;}
//        else if(5 == KetNum) { KEY_BUFF[6] = 5;}
//        else if(6 == KetNum) { KEY_BUFF[7] = 6;}
//        else if(7 == KetNum) { KEY_BUFF[8] = 7;}
//        else if(8 == KetNum) { KEY_BUFF[9] = 8;}
//        else if(9 == KetNum) { KEY_BUFF[10] = 9;}
//        else if(10 == KetNum) { KEY_BUFF[11] = 10;}
//        else if(11 == KetNum) { KEY_BUFF[12] = 11;}
//		else if(12 == KetNum) { KEY_BUFF[12] = 12;}
//    }
//    KEY_BUFF[15] = '0';
//    NRF24L01_TX_Mode();
//	if(NRF24L01_TxPacket(KEY_BUFF) == NRF24L01_TX_DS)
//	{
//		LED1_TOGGLE;
//	}
//	else
//	{										   	
//  		printf("ROCKER Send failed !\n");	 
//	}
//}

