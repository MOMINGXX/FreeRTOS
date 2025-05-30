#include "USART.h"

uint8_t Usart_RxHEX[RXHEX_MAX];
char Usart_RxCharacter[RXCHA_MAX];
uint8_t Usart_RxFlag;

/****
	* @brief	USART GPIO 配置     			  
	* @param   	无
	* @return   无  	
	* Sample usage:Usart_GPIO_Config();
    */
static void Usart_GPIO_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;                                //GPIO 初始化结构体	
    SPECIFIE_USART_GPIO_APBxCLKCMD(SPECIFIE_USART_GPIO_CLK,ENABLE);     //使能 USART GPIO 时钟
    
	GPIO_InitStructure.GPIO_Pin = SPECIFIE_USART_TX_GPIO_PIN;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPECIFIE_USART_GPIO_PORT,&GPIO_InitStructure);        	//初始化 USART GPIO	
	
	GPIO_InitStructure.GPIO_Pin = SPECIFIE_USART_RX_GPIO_PIN;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;						//上拉输入
	GPIO_Init(SPECIFIE_USART_GPIO_PORT,&GPIO_InitStructure);        	//初始化 USART GPIO	
}

/****
	* @brief	USART 配置     			  
	* @param   	无
	* @return   无  	
	* Sample usage:Usart_Config();
    */
static void Usart_Config()
{
    USART_InitTypeDef USART_InitStructure;                             //USART 初始化结构体
    SPECIFIE_USART_APBxCLKCMD(SPECIFIE_USART_CLK,ENABLE);              //使能 USART 时钟

    USART_InitStructure.USART_BaudRate = SPECIFIE_USART_BAUDRATE;                       //设置 USART 波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;     //不使用硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                     //发送和接收都使能 
    USART_InitStructure.USART_Parity = USART_Parity_No;                                 //不使用校验位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                              //一个停止位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                         //一个数据帧8位
    USART_Init(SPECIFIE_USARTx,&USART_InitStructure);                                   //初始化 USART

    USART_Cmd(SPECIFIE_USARTx,ENABLE);                                                //使能 USART
}

/****
	* @brief	NVIC 配置      			  
	* @param   	无
	* @return   无  	
	* Sample usage:Usart_NVIC_Config();
    */
static void Usart_NVIC_Config()
{
    NVIC_InitTypeDef NVIC_InitStructure;                                        //NVIC 初始化结构体
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);                             //设置 NVIC 分组2

    NVIC_InitStructure.NVIC_IRQChannel = SPECIFIE_USART_IRQChannel;             //选择 USART 中断
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             //使能 USART 中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SPECIFIE_USART_PreemptionPriority;    //设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SPECIFIE_USART_SubPriority;                  //设置子优先级
    NVIC_Init(&NVIC_InitStructure);                                             //初始化 NVIC   
}

/****
	* @brief	串口 初始化      			  
	* @param   	无
	* @return   无  	
	* Sample usage:Usart_Init();
    */
void Usart_Init()
{
    Usart_GPIO_Config();
    Usart_Config();
    Usart_NVIC_Config();
}

/****
	* @brief	串口 发送字节数据      			  
	* @param   	USARTx  串口号
	* @param   	Byte    发送的数据
	* @return   无  	
	* Sample usage:Usart_SendByte(USART1,0x55);
    */
void Usart_SendByte(USART_TypeDef *USARTx,uint8_t Byte)
{
    USART_SendData(USARTx,Byte);                                    //发送一个字节数据
    while(RESET == USART_GetFlagStatus(USARTx,USART_FLAG_TXE));     //等待发送完成
}

/****
	* @brief	串口 发送16位数据     			  
	* @param   	USARTx  串口号
	* @param   	Byte    发送的数据
	* @return   无  	
	* Sample usage:Usart_SendHalfWorld(USART1,0x5555);
    */
void Usart_SendHalfWorld(USART_TypeDef *USARTx,uint16_t Byte)
{
    uint8_t tempH ,tempL;
    tempH = (Byte & 0xFF00) >> 8;            //高8位
    tempL = Byte & 0xFF;                 //低8位
	
	USART_SendData(USARTx,tempH);  //发送高8位
	while(RESET == USART_GetFlagStatus(USARTx,USART_FLAG_TXE));     //等待发送完成
	USART_SendData(USARTx,tempL);  //发送低8位    
	while(RESET == USART_GetFlagStatus(USARTx,USART_FLAG_TXE));     //等待发送完成
    
}

/****
	* @brief	串口 发送字符串      			  
	* @param   	USARTx  串口号
	* @param   	String  发送的字符串数据
	* @return   无  	
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
	* @brief	串口 数组数据      			  
	* @param   	USARTx  串口号
	* @param   	Buff    数组地址
    * @param   	Length  数组长度
	* @return   无  	
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
	* @brief	平方计算  			  
    * @param   	X        基值
    * @param    Y        指数的值
	* @return   Result   x 的 y 次幂的结果	
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
	* @brief	串口发送数字  			  
	* @param   	USARTx  选择串口获串口外设      USART1, USART2, USART3, UART4 or UART5
    * @param   	Number  要发送的数字
    * @param    Length  数字的长度
	* @return   无  	
	* Sample usage: Usart_SendNumber(SPECIFIE_USARTx,12345,5);
    */
void Usart_SendSpeNumber(USART_TypeDef *USARTx,uint32_t Number,uint8_t Length)
{
    while(Length--)
    {
        Usart_SendByte(USARTx,Number / Usart_Pow(10,Length) % 10 + '0'); //23 2 3
    }
}

//printf重定向 打印    
int fputc(int c,FILE *pf)
{
	(void)pf;		// 显式忽略未使用的参数，避免警告
    if(c == '\n')
    {
        Usart_SendByte(SPECIFIE_USARTx,0x0D);
    }
    Usart_SendByte(SPECIFIE_USARTx,(uint8_t)c);
    return c;
}

//scanf重定向 输入
int fgetc(FILE *pf)
{
	(void)pf;		// 显式忽略未使用的参数，避免警告
	while(RESET == USART_GetFlagStatus(SPECIFIE_USARTx,USART_FLAG_TXE));	//等待串口输入数据
	return (int)USART_ReceiveData(SPECIFIE_USARTx);
}

/****
	* @brief	串口打印 			  
	* @param   	format  字符串，包含了要被写入到字符串 str 的文本。它可以包含嵌入的 format 标签，
    *                   format 标签可被随后的附加参数中指定的值替换，并按需求进行格式化  
	* @return   无  	
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
































