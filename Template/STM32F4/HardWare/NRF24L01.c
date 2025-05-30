#include "NRF24L01.h"

/****
	* @brief	NRF24L01 SPI GPIO 配置			  
	* @param   	无
	* @return   无  	
	* Sample usage: NRF24L01_GPIO_Config();
    */
static void NRF24L01_GPIO_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NRF24L01_GPIO_APBxClockCMD(NRF24L01_GPIO_CLK, ENABLE);      //GPIO 时钟使能

    GPIO_InitStructure.GPIO_Pin = NRF24L01_CE_GPIO_PIN | NRF24L01_CS_GPIO_PIN | NRF24L01_IRQ_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;               //GPIO 模式为输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;              //GPIO 输出模式为推挽输出      
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             	//GPIO 上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(NRF24L01_GPIO_PORT, &GPIO_InitStructure);
}

/****
	* @brief	NRF24L01 初始化			  
	* @param   	无
	* @return   无  	
	* Sample usage: NRF24L01_Init();
    */
void NRF24L01_Init()
{
    NRF24L01_GPIO_Config();
    NRF24L01_SPI_Init();
}

/****
	* @brief    NRF24L01 向特定的寄存器写入数据 	  
	* @param   	Reg: NRF24L01 的命令 + 寄存器地址
	* @param    Data: 向寄存器写入的数据 
    * @return   Status：寄存器的状态
	* Sample usage:NRF24L01_WriteReg(NOP,NOP);
    */
static uint8_t NRF24L01_WriteReg(uint8_t Reg,uint8_t Data)
{
    uint8_t Status;
    NRF24L01_CS_LOW();          //片选
    Status = NRF24L01_Send_RecieveByte(Reg);    //发送寄存器地址
    NRF24L01_Send_RecieveByte(Data);            //写入数据
    NRF24L01_CS_HIGH();         //取消片选
    return Status;
}

/****
	* @brief    NRF24L01 向特定的寄存器写入多个数据 	  
	* @param   	Reg: NRF24L01 的命令 + 寄存器地址
	* @param    Buff: 用于存储要写入寄存器数据的数组
    * @param    Len: Buff的数据长度
    * @return   Status：寄存器的状态
	* Sample usage:NRF24L01_WriteBuff(NOP,NOP,6);
    */
static uint8_t NRF24L01_WriteBuff(uint8_t Reg,uint8_t *Buff,uint8_t Len)
{
    uint8_t Status;
    NRF24L01_CS_LOW();          //片选
    Status = NRF24L01_Send_RecieveByte(Reg);    //发送寄存器地址
    for(int i = 0;i < Len;i++)
    {
        NRF24L01_Send_RecieveByte(*Buff++);    //写入数据
    }
    NRF24L01_CS_HIGH();         //取消片选
    return Status;
}

/****
	* @brief    NRF24L01 向特定的寄存器读数据 	  
	* @param   	Reg: NRF24L01 的命令 + 寄存器地址
    * @return   Data：寄存器中的数据
	* Sample usage:NRF24L01_ReadReg(NOP);
    */
static uint8_t NRF24L01_ReadReg(uint8_t reg)
{
    uint8_t Data;
    NRF24L01_CS_LOW();          //片选
    NRF24L01_Send_RecieveByte(reg);    //发送寄存器地址
    Data = NRF24L01_Send_RecieveByte(NOP);    //读取数据
    NRF24L01_CS_HIGH();         //取消片选
    return Data;
}

/****
	* @brief    NRF24L01 向特定的寄存器读多个数据 	  
	* @param   	Reg: NRF24L01 的命令 + 寄存器地址
    * @return   Buff：用于存储被读出寄存器数据的数组
    * @param    Len: Buff的数据长度
    * @return   Status：寄存器的状态
	* Sample usage:NRF24L01_ReadBuff(Reg,Buff,6);
    */
static uint8_t NRF24L01_ReadBuff(uint8_t Reg,uint8_t *Buff,uint8_t Len)
{
    uint8_t Status;
    NRF24L01_CS_LOW();          //片选
    Status = NRF24L01_Send_RecieveByte(Reg);    //发送寄存器地址
    for(int i = 0;i < Len;i++)
    {
        *Buff++ = NRF24L01_Send_RecieveByte(NOP);    //读取数据
    }
    NRF24L01_CS_HIGH();         //取消片选
    return Status;
}

void NRF24L01_RX_Mode()
{

}

void NRF24L01_TX_Mode()
{

}

/****
	* @brief    检测NRF24L01是否存在  
	* @param   	无
    * @return   SUCCESS   成功
    *           ERROR     失败
	* Sample usage:NRF24L01_Check();
    */
uint8_t NRF24L01_Check()
{
	uint8_t Tx_Buff[5] = {0xC2,0xC2,0xC2,0xC2,0xC2};
	uint8_t Rx_Buff[5] = { 0 };
	uint8_t i; 
	 
	//写入5个字节的地址
	NRF24L01_WriteBuff(NRF24L01_W_REGISTER + NRF24L01_TX_ADDR,Tx_Buff,5);

	//读出写入的地址 
	NRF24L01_ReadBuff(NRF24L01_TX_ADDR,Rx_Buff,5); 
	 
	//比较             
	for(i = 0; i < 5; i++)
	{
		if(Tx_Buff[i] != Rx_Buff[i])
		break;
	} 
	       
	if(i != 5)
		return ERROR ;        //MCU与NRF24L01连接失败
	else
		return SUCCESS ;      //MCU与NRF24L01正常连接
}

uint8_t NRF24L01_TxPacket(uint8_t *TxBuff)
{
    return 0;
}

uint8_t NRF24L01_RxPacket(uint8_t *RxBuff)
{
    return 0;   
}

void NRF24L01_Check_detection()
{

}

