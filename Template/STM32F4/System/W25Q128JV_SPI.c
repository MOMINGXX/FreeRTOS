#include "W25Q128JV_SPI.h"

const uint16_t W25Q128JV_SPI_Wait_TimeOut = 0x1000;

/****
	* @brief	W25Q128JV SPI GPIO 配置			  
	* @param   	无
	* @return   无  	
	* Sample usage: W25Q128JV_SPI_GPIO_Config();
    */
static void W25Q128JV_SPI_GPIO_Config()
{
    GPIO_InitTypeDef GPIO_InitStructer;

    // 使能GPIO时钟
    W25Q128JV_GPIO_APBxCLKCMD(W25Q128JV_GPIO_CLK, ENABLE);

    //SPI 复用引脚
    GPIO_PinAFConfig(W25Q128JV_SPI_GPIO_PORT, W25Q128JV_SCK_PINSOURCE, GPIO_AF_SPI3);
    GPIO_PinAFConfig(W25Q128JV_SPI_GPIO_PORT, W25Q128JV_MISO_PINSOURCE, GPIO_AF_SPI3);
    GPIO_PinAFConfig(W25Q128JV_SPI_GPIO_PORT, W25Q128JV_MOSI_PINSOURCE, GPIO_AF_SPI3);

    // 配置CS引脚
    GPIO_InitStructer.GPIO_Pin = W25Q128JV_CS_GPIO_PIN;        //CS引脚
    GPIO_InitStructer.GPIO_Mode = GPIO_Mode_OUT;               //输出模式
    GPIO_InitStructer.GPIO_OType = GPIO_OType_PP;              //推挽输出
    GPIO_InitStructer.GPIO_PuPd = GPIO_PuPd_UP;            //上拉
    GPIO_InitStructer.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(W25Q128JV_CS_GPIO_PORT, &GPIO_InitStructer);
    // 配置SPI GPIO
    GPIO_InitStructer.GPIO_Pin = W25Q128JV_SCK_GPIO_PIN | W25Q128JV_MISO_GPIO_PIN | W25Q128JV_MOSI_GPIO_PIN;    //SCK、MISO、MOSI引脚
    GPIO_InitStructer.GPIO_Mode = GPIO_Mode_AF;                //复用模式
    GPIO_InitStructer.GPIO_OType = GPIO_OType_PP;              //推挽输出
    GPIO_InitStructer.GPIO_PuPd = GPIO_PuPd_UP;            //上拉
    GPIO_InitStructer.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(W25Q128JV_SPI_GPIO_PORT, &GPIO_InitStructer);

    W25Q128JV_CS_SET(1);    //CS拉高
}

/****
	* @brief	W25Q128JV SPI 配置			  
	* @param   	无
	* @return   无  	
	* Sample usage: W25Q128JV_SPI_Config();
    */
static void W25Q128JV_SPI_Config()
{
    SPI_InitTypeDef SPI_InitStructer;
    //SPI_DeInit(W25Q128JV_SPIx);    //复位SPI
    W25Q128JV_SPIx_APBxClockCMD(W25Q128JV_SPIx_CLK, ENABLE);    // 使能SPI时钟

    SPI_InitStructer.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;       //波特率预分频值 2分频
    SPI_InitStructer.SPI_CPHA = SPI_CPHA_2Edge;         //时钟相位 第二个边沿捕获
    SPI_InitStructer.SPI_CPOL = SPI_CPOL_High;          //时钟极性 高电平
    SPI_InitStructer.SPI_CRCPolynomial = 7;             //CRC不校验
    SPI_InitStructer.SPI_DataSize = SPI_DataSize_8b;    //数据大小 8位
    SPI_InitStructer.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //全双工
    SPI_InitStructer.SPI_FirstBit = SPI_FirstBit_MSB;   //高位在前
    SPI_InitStructer.SPI_Mode = SPI_Mode_Master;        //主模式
    SPI_InitStructer.SPI_NSS = SPI_NSS_Soft;            //NSS信号由软件控制
    SPI_Init(W25Q128JV_SPIx, &SPI_InitStructer);

    SPI_Cmd(W25Q128JV_SPIx, ENABLE);        //使能SPI
}

/****
	* @brief	W25Q128JV DMA 接收 配置			  
	* @param   	无
	* @return   无  	
	* Sample usage: W25Q128JV_DMA_RX_Config();
    */
static void W25Q128JV_DMA_RX_Config()
{
    DMA_InitTypeDef DMA_InitStructer;

    DMA_DeInit(W25Q128JV_DMAx_SPI_RX_Stream);    //复位DMA
    W25Q128JV_DMAxAHBxClockCMD(W25Q128JV_DMAx_CLK,ENABLE);          //使能DMA时钟

    //DMA_InitStructer.DMA_BufferSize = BuffSize;                   //缓冲区大小
    DMA_InitStructer.DMA_Channel = W25Q128JV_DMAxChannel;           //DMA通道
    DMA_InitStructer.DMA_DIR = DMA_DIR_PeripheralToMemory;          //传输方向 外设到内存
    DMA_InitStructer.DMA_FIFOMode = DMA_FIFOMode_Disable;           //FIFO模式 关闭
    DMA_InitStructer.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;        //FIFO阈值 半满
    //DMA_InitStructer.DMA_Memory0BaseAddr = ;
    DMA_InitStructer.DMA_MemoryBurst = DMA_MemoryBurst_Single;      //内存突发单次传输
    DMA_InitStructer.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  //内存数据大小 字节
    DMA_InitStructer.DMA_MemoryInc = DMA_MemoryInc_Enable;          //内存地址自增    
    DMA_InitStructer.DMA_Mode = DMA_Mode_Normal;                    //工作模式 正常模式 
    DMA_InitStructer.DMA_PeripheralBaseAddr = (uint32_t)&W25Q128JV_SPIx->DR;    //外设地址
    DMA_InitStructer.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;          //外设突发单次传输
    DMA_InitStructer.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;      //外设数据大小 字节
    DMA_InitStructer.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //外设地址不自增
    DMA_InitStructer.DMA_Priority = DMA_Priority_Medium;                        //优先级 中等
    DMA_Init(W25Q128JV_DMAx_SPI_RX_Stream, &DMA_InitStructer);
}

/****
	* @brief	W25Q128JV DMA 发送 配置			  
	* @param   	无
	* @return   无  	
	* Sample usage: W25Q128JV_DMA_TX_Config();
    */
static void W25Q128JV_DMA_TX_Config()
{
    DMA_InitTypeDef DMA_InitStructer;

    DMA_DeInit(W25Q128JV_DMAx_SPI_TX_Stream);    //复位DMA
    W25Q128JV_DMAxAHBxClockCMD(W25Q128JV_DMAx_CLK,ENABLE);          //使能DMA时钟

    //DMA_InitStructer.DMA_BufferSize = BuffSize;                   //缓冲区大小
    DMA_InitStructer.DMA_Channel = W25Q128JV_DMAxChannel;           //DMA通道
    DMA_InitStructer.DMA_DIR = DMA_DIR_MemoryToPeripheral;          //传输方向 外设到内存
    DMA_InitStructer.DMA_FIFOMode = DMA_FIFOMode_Disable;           //FIFO模式 关闭
    DMA_InitStructer.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;        //FIFO阈值 半满
    //DMA_InitStructer.DMA_Memory0BaseAddr = ;
    DMA_InitStructer.DMA_MemoryBurst = DMA_MemoryBurst_Single;      //内存突发单次传输
    DMA_InitStructer.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  //内存数据大小 字节
    DMA_InitStructer.DMA_MemoryInc = DMA_MemoryInc_Enable;          //内存地址自增    
    DMA_InitStructer.DMA_Mode = DMA_Mode_Normal;                    //工作模式 正常模式 
    DMA_InitStructer.DMA_PeripheralBaseAddr = (uint32_t)&W25Q128JV_SPIx->DR;    //外设地址
    DMA_InitStructer.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;          //外设突发单次传输
    DMA_InitStructer.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;      //外设数据大小 字节
    DMA_InitStructer.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //外设地址不自增
    DMA_InitStructer.DMA_Priority = DMA_Priority_Medium;                        //优先级 中等
    DMA_Init(W25Q128JV_DMAx_SPI_TX_Stream, &DMA_InitStructer);
}

/****
	* @brief	W25Q128JV 初始化		  
	* @param   	无
	* @return   无  	
	* Sample usage: W25Q128JV_Config_Init();
    */
void W25Q128JV_Config_Init()
{
    W25Q128JV_SPI_GPIO_Config();
    W25Q128JV_SPI_Config();
//    W25Q128JV_DMA_RX_Config();
//    W25Q128JV_DMA_TX_Config();
}

/****
	* @brief	W25Q128JV SPI DMA 使能/失能		  
	* @param   	NewState    ENABLE/DISABLE
	* @return   无  	
	* Sample usage: W25Q128JV_SPIx_DMA_CMD(enable);
    */
void W25Q128JV_SPIx_DMA_CMD(FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        SPI_I2S_DMACmd(W25Q128JV_SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
        SPI_I2S_DMACmd(W25Q128JV_SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
    }
    else
    {
        SPI_I2S_DMACmd(W25Q128JV_SPIx, SPI_I2S_DMAReq_Rx, DISABLE);
        SPI_I2S_DMACmd(W25Q128JV_SPIx, SPI_I2S_DMAReq_Tx, DISABLE);
    }
}

/****
	* @brief	W25Q128JV DMA 使能/失能		  
	* @param   	NewState    ENABLE/DISABLE
	* @return   无  	
	* Sample usage: W25Q128JV__DMA_CMD(enable);
    */
void W25Q128JV_DMA_CMD(FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        DMA_Cmd(W25Q128JV_DMAx_SPI_RX_Stream, ENABLE);
        DMA_Cmd(W25Q128JV_DMAx_SPI_TX_Stream, ENABLE);
    }
    else
    {
        DMA_Cmd(W25Q128JV_DMAx_SPI_RX_Stream, DISABLE);
        DMA_Cmd(W25Q128JV_DMAx_SPI_TX_Stream, DISABLE);
    }
}

/****
	* @brief	SPI 发送 接收 一个字节数据   		  
	* @param	SendByte	发送的数据
	* @return   ReceiveByte 接收的数据 	
	* Sample usage:SOFT_SPI_Config();
    */
uint8_t W25Q128JV_SPI_SendReceiveByte(uint8_t SendByte)
{
    uint8_t ReceiveByte = 0;
    uint16_t Timeout = 0;
    while (SPI_I2S_GetFlagStatus(W25Q128JV_SPIx, SPI_I2S_FLAG_TXE) == RESET)    //等待发送缓冲区为空
    {
        if (Timeout++ >= W25Q128JV_SPI_Wait_TimeOut)
        {
            //break;         //超时退出
        }
    }
    SPI_I2S_SendData(W25Q128JV_SPIx, SendByte);    //发送一个字节
    Timeout = W25Q128JV_SPI_Wait_TimeOut / 2;
    while (SPI_I2S_GetFlagStatus(W25Q128JV_SPIx, SPI_I2S_FLAG_RXNE) == RESET)    //等待接收缓冲区非空
    {
        if (Timeout++ >= W25Q128JV_SPI_Wait_TimeOut)
        {
            //break;         //超时退出
        }
    }
    ReceiveByte = SPI_I2S_ReceiveData(W25Q128JV_SPIx);    //接收一个字节
    return ReceiveByte;
}

