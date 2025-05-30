#include "USART.h"

uint8_t Usart_RxHEX[RXHEX_MAX];
char Usart_RxCharacter[RXCHA_MAX];
uint8_t Usart_RxFlag;

/****
	* @brief	USART GPIO ����     			  
	* @param   	��
	* @return   ��  	
	* Sample usage:Usart_GPIO_Config();
    */
static void Usart_GPIO_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;                                //GPIO ��ʼ���ṹ��	
    SPECIFIE_USART_GPIO_APBxCLKCMD(SPECIFIE_USART_GPIO_CLK,ENABLE);     //ʹ�� USART GPIO ʱ��
    
	GPIO_InitStructure.GPIO_Pin = SPECIFIE_USART_TX_GPIO_PIN;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPECIFIE_USART_GPIO_PORT,&GPIO_InitStructure);        	//��ʼ�� USART GPIO	
	
	GPIO_InitStructure.GPIO_Pin = SPECIFIE_USART_RX_GPIO_PIN;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;						//��������
	GPIO_Init(SPECIFIE_USART_GPIO_PORT,&GPIO_InitStructure);        	//��ʼ�� USART GPIO	
}

/****
	* @brief	USART ����     			  
	* @param   	��
	* @return   ��  	
	* Sample usage:Usart_Config();
    */
static void Usart_Config()
{
    USART_InitTypeDef USART_InitStructure;                             //USART ��ʼ���ṹ��
    SPECIFIE_USART_APBxCLKCMD(SPECIFIE_USART_CLK,ENABLE);              //ʹ�� USART ʱ��

    USART_InitStructure.USART_BaudRate = SPECIFIE_USART_BAUDRATE;                       //���� USART ������
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;     //��ʹ��Ӳ��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                     //���ͺͽ��ն�ʹ�� 
    USART_InitStructure.USART_Parity = USART_Parity_No;                                 //��ʹ��У��λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                              //һ��ֹͣλ
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                         //һ������֡8λ
    USART_Init(SPECIFIE_USARTx,&USART_InitStructure);                                   //��ʼ�� USART

    USART_Cmd(SPECIFIE_USARTx,ENABLE);                                                //ʹ�� USART
}

/****
	* @brief	NVIC ����      			  
	* @param   	��
	* @return   ��  	
	* Sample usage:Usart_NVIC_Config();
    */
static void Usart_NVIC_Config()
{
    NVIC_InitTypeDef NVIC_InitStructure;                                        //NVIC ��ʼ���ṹ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);                             //���� NVIC ����2

    NVIC_InitStructure.NVIC_IRQChannel = SPECIFIE_USART_IRQChannel;             //ѡ�� USART �ж�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             //ʹ�� USART �ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SPECIFIE_USART_PreemptionPriority;    //������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SPECIFIE_USART_SubPriority;                  //���������ȼ�
    NVIC_Init(&NVIC_InitStructure);                                             //��ʼ�� NVIC   
}

/****
	* @brief	���� ��ʼ��      			  
	* @param   	��
	* @return   ��  	
	* Sample usage:Usart_Init();
    */
void Usart_Init()
{
    Usart_GPIO_Config();
    Usart_Config();
    Usart_NVIC_Config();
}

/****
	* @brief	���� �����ֽ�����      			  
	* @param   	USARTx  ���ں�
	* @param   	Byte    ���͵�����
	* @return   ��  	
	* Sample usage:Usart_SendByte(USART1,0x55);
    */
void Usart_SendByte(USART_TypeDef *USARTx,uint8_t Byte)
{
    USART_SendData(USARTx,Byte);                                    //����һ���ֽ�����
    while(RESET == USART_GetFlagStatus(USARTx,USART_FLAG_TXE));     //�ȴ��������
}

/****
	* @brief	���� ����16λ����     			  
	* @param   	USARTx  ���ں�
	* @param   	Byte    ���͵�����
	* @return   ��  	
	* Sample usage:Usart_SendHalfWorld(USART1,0x5555);
    */
void Usart_SendHalfWorld(USART_TypeDef *USARTx,uint16_t Byte)
{
    uint8_t tempH ,tempL;
    tempH = (Byte & 0xFF00) >> 8;            //��8λ
    tempL = Byte & 0xFF;                 //��8λ
	
	USART_SendData(USARTx,tempH);  //���͸�8λ
	while(RESET == USART_GetFlagStatus(USARTx,USART_FLAG_TXE));     //�ȴ��������
	USART_SendData(USARTx,tempL);  //���͵�8λ    
	while(RESET == USART_GetFlagStatus(USARTx,USART_FLAG_TXE));     //�ȴ��������
    
}

/****
	* @brief	���� �����ַ���      			  
	* @param   	USARTx  ���ں�
	* @param   	String  ���͵��ַ�������
	* @return   ��  	
	* Sample usage:Usart_SendString(USART1,"Hello World");
    */
void Usart_SendString(USART_TypeDef *USARTx,char *String)
{
    while (*String != '\0')
    {
        Usart_SendByte(USARTx,*String++);
    }
}

/****
	* @brief	���� ��������      			  
	* @param   	USARTx  ���ں�
	* @param   	Buff    �����ַ
    * @param   	Length  ���鳤��
	* @return   ��  	
	* Sample usage:Usart_SendByteBuff();
    */
void Usart_SendByteBuff(USART_TypeDef *USARTx ,uint8_t *Buff, uint16_t Length)
{
    while(Length--)
    {
        Usart_SendByte(USARTx,*Buff++);
    }
}

/****
	* @brief	ƽ������  			  
    * @param   	X        ��ֵ
    * @param    Y        ָ����ֵ
	* @return   Result   x �� y ���ݵĽ��	
	* Sample usage: Usart_Pow(10,Length);
    */
static uint32_t Usart_Pow(uint32_t X, uint32_t Y)
{
    //X^Y
    uint32_t Result = 1;
    while(Y--)
    {
        Result *= X;
    }
    return Result;
}

/****
	* @brief	���ڷ�������  			  
	* @param   	USARTx  ѡ�񴮿ڻ񴮿�����      USART1, USART2, USART3, UART4 or UART5
    * @param   	Number  Ҫ���͵�����
    * @param    Length  ���ֵĳ���
	* @return   ��  	
	* Sample usage: Usart_SendNumber(SPECIFIE_USARTx,12345,5);
    */
void Usart_SendSpeNumber(USART_TypeDef *USARTx,uint32_t Number,uint8_t Length)
{
    while(Length--)
    {
        Usart_SendByte(USARTx,Number / Usart_Pow(10,Length) % 10 + '0'); //23 2 3
    }
}

//printf�ض��� ��ӡ    
int fputc(int c,FILE *pf)
{
	(void)pf;		// ��ʽ����δʹ�õĲ��������⾯��
    if(c == '\n')
    {
        Usart_SendByte(SPECIFIE_USARTx,0x0D);
    }
    Usart_SendByte(SPECIFIE_USARTx,(uint8_t)c);
    return c;
}

//scanf�ض��� ����
int fgetc(FILE *pf)
{
	(void)pf;		// ��ʽ����δʹ�õĲ��������⾯��
	while(RESET == USART_GetFlagStatus(SPECIFIE_USARTx,USART_FLAG_TXE));	//�ȴ�������������
	return (int)USART_ReceiveData(SPECIFIE_USARTx);
}

/****
	* @brief	���ڴ�ӡ 			  
	* @param   	format  �ַ�����������Ҫ��д�뵽�ַ��� str ���ı��������԰���Ƕ��� format ��ǩ��
    *                   format ��ǩ�ɱ����ĸ��Ӳ�����ָ����ֵ�滻������������и�ʽ��  
	* @return   ��  	
	* Sample usage: Usart_sprintf("Number=%d\r\n",125);
    */
void Usart_sprintf(char *format, ...)
{
    char String[100] = {0};
    va_list args;
    va_start(args, format);
    vsprintf(String, format, args);
    va_end(args);
    Usart_SendString(SPECIFIE_USARTx,String);
}

#ifdef RX_HEX
uint8_t Usart_GetRxFlag()
{
    if(Usart_RxFlag == 1)
    {
        Usart_RxFlag = 0 ;
        return 1;
    }
    return 0;
}

void SPECIFIE_USART_IRQHNADLER()
{
    static uint8_t RX_State = 0;
    static uint8_t RX_packet = 0;
    if( SET == USART_GetITStatus(SPECIFIE_USARTx,USART_IT_RXNE))
    {
        uint8_t RX_Data = USART_ReceiveData(SPECIFIE_USARTx);
        if(RX_State == 0)
        {
            if(RX_Data == 0xFF)
            {
                RX_State = 1;
                RX_packet = 0;
            }
        }
        else if(RX_State == 1)
        {
            Usart_RxHEX[RX_packet] = RX_Data;
            RX_packet++;
            if(RX_MAX <= RX_packet)
            {
                RX_State = 2;
            }
        }
        else if(RX_State == 2)
        {
            if(RX_Data == 0xFE)
            {
                RX_State = 0;
                Usart_RxFlag = 1 ;
            }
        }
        USART_ClearITPendingBit(SPECIFIE_USARTx,USART_IT_RXNE);
    }
}
#else //RX_CHARACTER
void SPECIFIE_USART_IRQHNADLER()
{
    static uint8_t RX_State = 0;
    static uint8_t RX_packet = 0;
    if( SET == USART_GetITStatus(SPECIFIE_USARTx,USART_IT_RXNE))
    {
        char RX_Data = USART_ReceiveData(SPECIFIE_USARTx);
        if(RX_State == 0)
        {
            if(RX_Data == '@' && Usart_RxFlag == 0)
            {
                RX_State = 1;
                RX_packet = 0;
            }
        }
        else if(RX_State == 1)
        {
            if(RX_Data == '\r')
            {
                RX_State = 2;
            }
            else 
            {
                Usart_RxCharacter[RX_packet] = RX_Data;
                RX_packet++;
            }
        }
        else if(RX_State == 2)
        {
            if(RX_Data == '\n')
            {
                RX_State = 0;
                Usart_RxCharacter[RX_packet] = '\0';
                Usart_RxFlag = 1 ;
            }
        }
        USART_ClearITPendingBit(SPECIFIE_USARTx,USART_IT_RXNE);
    }
}
#endif
































