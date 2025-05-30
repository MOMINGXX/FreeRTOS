#include "LCD_SPI.h"

uint16_t DMA1_MEM_LEN;//保存DMA每次数据传送的长度 

/****
	* @brief    LCD GPIO配置		  
	* @param   	无
	* @return   无
	* Sample usage:LCD_GPIO_Config();
    */
void LCD_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	LCD_GPIO_AHBxClockCMD(LCD_GPIO_CLK, ENABLE);	 //使能

	GPIO_PinAFConfig(LCD_GPIO_PORT, LCD_SCL_GPIO_PINSOURCE, LCD_SPI_SCLK_AF);	//复用
	GPIO_PinAFConfig(LCD_GPIO_PORT, LCD_SDA_GPIO_PINSOURCE, LCD_SPI_MOSI_AF);	//复用
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 		 	//复用	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽
	GPIO_InitStructure.GPIO_Pin = LCD_SCL_GPIO_PIN | LCD_SDA_GPIO_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//速度 100MHz
	GPIO_Init(LCD_GPIO_PORT, &GPIO_InitStructure);	  		

	GPIO_SetBits(LCD_GPIO_PORT, LCD_SCL_GPIO_PIN | LCD_SDA_GPIO_PIN);	//上拉
}

/****
	* @brief    LCD SPI配置	  
	* @param   	无
	* @return   无
	* Sample usage:LCD_SPIx_Config();
    */
void LCD_SPIx_Config()
{
	SPI_InitTypeDef SPI_InitStruct;
	LCD_SPIx_APBxClockCMD(LCD_SPIx_CLK,ENABLE);	 //使能 SPI
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;     //波特率预分频值
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;                           //(时钟相位)第二个跳变沿（即上升沿）数据被采样
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;                            //(时钟极性)空闲状态为高电平
	SPI_InitStruct.SPI_CRCPolynomial = 7;                               //CRC校验  不使用
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;                      //数据帧大小为8位
	SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx;              //SPI 设置为单线仅发送模式
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;                     //数据传输从 MSB 位开始
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;                          //为主机
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;                              //内部 NSS 信号有 SSI 位控制
	SPI_Init(LCD_SPIx,&SPI_InitStruct);

	SPI_Cmd(LCD_SPIx,ENABLE);
}

/****
	* @brief    LCD SPI 初始化			  
	* @param   	无
	* @return   无
	* Sample usage:LCD_SPIx_Init();
    */
void LCD_SPIx_Init()
{
	LCD_GPIO_Config();
	LCD_SPIx_Config();
}

/****
	* @brief    LCD SPI SPI 发送 一个字节数据 			  
	* @param   	Byte	发送的数据
	* @return   无
	* Sample usage:LCD_SPI_WriteByte(0x56);
    */
void LCD_SPI_WriteByte(uint8_t Byte)
{
	while(RESET == SPI_I2S_GetFlagStatus(LCD_SPIx,SPI_I2S_FLAG_TXE));	//等待发送缓冲区为空
	SPI_I2S_SendData(LCD_SPIx,Byte);									//发送一个字节的数据
	while(RESET != SPI_I2S_GetFlagStatus(LCD_SPIx,SPI_I2S_FLAG_BSY));  	//等待发送完成
}
 
