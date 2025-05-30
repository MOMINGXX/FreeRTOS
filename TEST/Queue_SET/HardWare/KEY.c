#include "KEY.h"

uint8_t KeyNumber;
QueueHandle_t Key_QueueHandle;    // ʵ�ʶ��岢��ʼ��ΪNULL

/****
	* @brief    KEY ���� ����	
	* @param   	��
	* @return   ��    	
	* Sample usage:KEY_Config(); 
    */
static void KEY_Config()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    //����GPIOʱ��
	KEY_APBx_ClockCMD(KEYx_CLK,ENABLE);
    //��������                                                                                   
    GPIO_InitStruct.GPIO_Pin =  ROUKER_RIGHT_GPIO_PIN | KEY_X_GPIO_PIN;                    
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;                                                 
    GPIO_Init(KEY1x_GPIO_PORT,&GPIO_InitStruct);                                                   
																							
    GPIO_InitStruct.GPIO_Pin = KEY_ON_GPIO_PIN | KEY_LEFT_GPIO_PIN | 
							   KEY_FRONTR_GPIO_PIN | ROUKER_LEFT_GPIO_PIN;        
    GPIO_Init(KEY2x_GPIO_PORT,&GPIO_InitStruct);                                                   
																									
    GPIO_InitStruct.GPIO_Pin = KEY_DOWN_GPIO_PIN | KEY_RIGHT_GPIO_PIN |KEY_X_GPIO_PIN |
							   KEY_Y_GPIO_PIN | KEY_A_GPIO_PIN | KEY_B_GPIO_PIN;                                                     
    GPIO_Init(KEY1_GPIO_PORT,&GPIO_InitStruct);                                                    
}                                                                                                  

/****
	* @brief    KEY ��ʼ��		  
	* @param   	��
	* @return   ��    	
	* Sample usage:KEY_Init(); 
    */
void KEY_Init()
{
    TIMEx_Init();
    KEY_Config();
	Key_QueueHandle = xQueueCreate(5,sizeof(uint8_t));		//������������
	if(Key_QueueHandle == NULL)							    //����ʧ��
	{
		printf("Key_QueueHandle Create Fail!\r\n");          
	}
}

#ifdef KEY_USE_TIMER
/****
	* @brief    ���ذ�����			  
	* @param   	��
	* @return   Temp   ������ 	
	* Sample usage:Key_GetNumber(); 
    */
uint8_t Key_GetNumber()
{
    uint8_t Temp = 0;
    Temp = KeyNumber;
    KeyNumber = 0;
    return Temp;
}

/****
	* @brief    ��ȡ����״̬			    
	* @param   	��
	* @return   Number  ���ذ���״̬    	  	
	* Sample usage:KEY_ReadNumber(); 
    */
static uint8_t KEY_ReadNumber()
{
    uint8_t Number = 0;
    if(GPIO_ReadInputDataBit(KEY1x_GPIO_PORT,KEY_FRONTL_GPIO_PIN) == RESET) {Number = 1;}
    else if(GPIO_ReadInputDataBit(KEY1x_GPIO_PORT,ROUKER_RIGHT_GPIO_PIN) == RESET) {Number = 2;}
    else if(GPIO_ReadInputDataBit(KEY2x_GPIO_PORT,KEY_ON_GPIO_PIN) == RESET) {Number = 3;}
    else if(GPIO_ReadInputDataBit(KEY2x_GPIO_PORT,KEY_LEFT_GPIO_PIN) == RESET) {Number = 4;}
    else if(GPIO_ReadInputDataBit(KEY2x_GPIO_PORT,KEY_FRONTR_GPIO_PIN) == RESET) {Number = 5;}
    else if(GPIO_ReadInputDataBit(KEY2x_GPIO_PORT,ROUKER_LEFT_GPIO_PIN) == RESET) {Number = 6;}
    else if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_DOWN_GPIO_PIN) == RESET) {Number = 7;}
    else if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_RIGHT_GPIO_PIN) == RESET) {Number = 8;}
    else if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_X_GPIO_PIN) == RESET) {Number = 9;}
    else if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_Y_GPIO_PIN) == RESET) {Number = 10;}
    else if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_A_GPIO_PIN) == RESET) {Number = 11;}
    else if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY_B_GPIO_PIN) == RESET) {Number = 12;}
	return Number;                                
}

/****
	* @brief    ��ʱ��ɨ��			  
	* @param   	��
	* @return   ��   	
	* Sample usage:KEY_LOOP(); 
    */
void KEY_LOOP()
{
    uint8_t NowState,LastState;
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

/****
	* @brief    ��ʱ�� �жϺ���		
	* @param   	��
	* @return   ��    	
    */
void TIMEx_IRQHandler()
{
    static uint8_t Count = 0;
    if(SET == TIM_GetITStatus(TIM2,TIM_IT_Update))
    {
        Count ++;
        if(Count >= 10)
        {
			Count = 0;
            KEY_LOOP();
		}
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
    }
}

#else 
/****
	* @brief    ��ȡ����״̬	��ʱ��������	
	* @param   	��
	* @return   Number  ���ذ���״̬    	
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
	* @brief    ��ȡ����			 	  
	* @param   	��
	* @return   Number  ���ذ���    	
	* Sample usage:Key_GetNumber(); 
    */
uint8_t Key_GetNumber()
{
    uint8_t Number = 0;
    if(Key_ReadNumber(KEY1x_GPIO_PORT,KEY_FRONTL_GPIO_PIN)) 		{Number = 1;}  
    else if(Key_ReadNumber(KEY1x_GPIO_PORT,ROUKER_RIGHT_GPIO_PIN)) 	{Number = 2;}
    else if(Key_ReadNumber(KEY2x_GPIO_PORT,KEY_ON_GPIO_PIN)) 		{Number = 3;}
    else if(Key_ReadNumber(KEY2x_GPIO_PORT,KEY_LEFT_GPIO_PIN)) 		{Number = 4;}
    else if(Key_ReadNumber(KEY2x_GPIO_PORT,KEY_FRONTR_GPIO_PIN)) 	{Number = 5;}
    else if(Key_ReadNumber(KEY2x_GPIO_PORT,ROUKER_LEFT_GPIO_PIN)) 	{Number = 6;}
    else if(Key_ReadNumber(KEY1_GPIO_PORT,KEY_DOWN_GPIO_PIN)) 		{Number = 7;}
    else if(Key_ReadNumber(KEY1_GPIO_PORT,KEY_RIGHT_GPIO_PIN)) 		{Number = 8;}
    else if(Key_ReadNumber(KEY1_GPIO_PORT,KEY_X_GPIO_PIN)) 			{Number = 9;}
    else if(Key_ReadNumber(KEY1_GPIO_PORT,KEY_Y_GPIO_PIN)) 			{Number = 10;}
    else if(Key_ReadNumber(KEY1_GPIO_PORT,KEY_A_GPIO_PIN)) 			{Number = 11;}
    else if(Key_ReadNumber(KEY1_GPIO_PORT,KEY_B_GPIO_PIN)) 			{Number = 12;}
	return Number; 
}

#endif


/****
	* @brief   FreeRTOS��ȡ����	�������		
	* @param   ��
	* @return  ��  
    */
void vKey_Task(void *pvParameters)
{
	uint8_t KEY_NUMBER = 0;
    while(1)
    {
		KEY_NUMBER = Key_GetNumber();		//��ȡ����
		if(KEY_NUMBER != 0)
		{
			xQueueSend(Key_QueueHandle,&KEY_NUMBER,0);		//��ȡ��ֵ�������
		}
		vTaskDelay(pdMS_TO_TICKS(5)); 
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

