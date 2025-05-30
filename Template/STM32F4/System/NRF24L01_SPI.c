#include "NRF24L01_SPI.h"

#define NRF24L01_SPIx_WateTime      ((uint16_t)0x0500)          //等待时间

/****
	* @brief	NRF24L01 SPI GPIO 配置			  
	* @param   	无
	* @return   无  	
	* Sample usage: NRF24L01_SPI_GPIO_Config();
    */
static void NRF24L01_SPI_GPIO_Config()
{
    GPIO_InitTypeDef GPIO_InitStructer;
    NRF24L01_GPIO_AHBxClockCMD(NRF24L01_GPIOx_CLK,ENABLE);

    //SPI 引脚 复用
    GPIO_PinAFConfig(NRF24L01_GPIOx_PORT,NRF24L01_SCK_PINSOURCE,GPIO_AF_SPI1);
    GPIO_PinAFConfig(NRF24L01_GPIOx_PORT,NRF24L01_MISO_PINSOURCE,GPIO_AF_SPI1);
    GPIO_PinAFConfig(NRF24L01_GPIOx_PORT,NRF24L01_MOSI_PINSOURCE,GPIO_AF_SPI1);

    GPIO_InitStructer.GPIO_Mode = GPIO_Mode_AF;         //复用模式
    GPIO_InitStructer.GPIO_OType = GPIO_OType_PP;       //推挽输出
    GPIO_InitStructer.GPIO_Pin = NRF24L01_MISO_GPIO_PIN | NRF24L01_MOSI_GPIO_PIN | NRF24L01_SCK_GPIO_PIN;
    GPIO_InitStructer.GPIO_PuPd = GPIO_PuPd_UP;     	//上拉
    GPIO_InitStructer.GPIO_Speed = GPIO_Speed_100MHz;   
    GPIO_Init(NRF24L01_GPIOx_PORT,&GPIO_InitStructer);
}

/****
	* @brief	NRF24L01 SPI 配置			  
	* @param   	无
	* @return   无  	
	* Sample usage: NRF24L01_SPI_Config();
    */
static void NRF24L01_SPI_Config()
{
    SPI_InitTypeDef SPI_InitStructer;
    NRF24L01_SPIx_APBxClockCMD(NRF24L01_SPIx_CLK,ENABLE);

    SPI_InitStructer.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;      //波特率预分频 16分频
    SPI_InitStructer.SPI_CPHA = SPI_CPHA_1Edge;                             //时钟相位  第二个边沿采样
    SPI_InitStructer.SPI_CPOL = SPI_CPOL_Low;                              //时钟极性  空闲时SCLK为高电平
    SPI_InitStructer.SPI_CRCPolynomial = 7;                                 //CRC校验多项式
    SPI_InitStructer.SPI_DataSize = SPI_DataSize_8b;                        //数据大小 8位
    SPI_InitStructer.SPI_Direction = SPI_Direction_2Lines_FullDuplex;       //全双工
    SPI_InitStructer.SPI_FirstBit = SPI_FirstBit_MSB;                       //数据传输从高位开始
    SPI_InitStructer.SPI_Mode = SPI_Mode_Master;                            //主模式
    SPI_InitStructer.SPI_NSS = SPI_NSS_Soft;                                //NSS信号由软件控制
    SPI_Init(NRF24L01_SPIx,&SPI_InitStructer);

    SPI_Cmd(NRF24L01_SPIx,ENABLE);                   //使能SPI
}

/****
	* @brief	NRF24L01 SPI 初始化			  
	* @param   	无
	* @return   无  	
	* Sample usage: NRF24L01_SPI_Init();
    */
void NRF24L01_SPI_Init()
{
    NRF24L01_SPI_GPIO_Config();
    NRF24L01_SPI_Config();
}

/****
	* @brief	SPI 发送 接收 一个字节数据   		  
	* @param	SendByte	发送的数据
	* @return   ReceiveByte 接收的数据 	
	* Sample usage:SOFT_SPI_Config();
    */
uint8_t NRF24L01_Send_RecieveByte(uint8_t SendByte)
{
    uint16_t TimeOut = 0;
    while(SPI_I2S_GetFlagStatus(NRF24L01_SPIx,SPI_I2S_FLAG_TXE) == RESET)     //等待发送缓冲区为空
    {
        if(++TimeOut >= NRF24L01_SPIx_WateTime)
        {
            break;      //超时退出
        }
    }
    TimeOut = 0;
    SPI_I2S_SendData(NRF24L01_SPIx,SendByte);                                //发送一个字节
    while(SPI_I2S_GetFlagStatus(NRF24L01_SPIx,SPI_I2S_FLAG_RXNE) == RESET)   //等待接收缓冲区非空
    {
        if(++TimeOut >= NRF24L01_SPIx_WateTime)     
        {
            break;      //超时退出
        }
    }
    return SPI_I2S_ReceiveData(NRF24L01_SPIx);                         		//读取一个字节
}
