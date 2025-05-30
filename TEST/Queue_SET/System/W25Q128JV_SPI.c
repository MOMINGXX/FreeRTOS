#include "W25Q128JV_SPI.h"

const uint16_t W25Q128JV_SPI_Wait_TimeOut = 0x1000;

/****
	* @brief	W25Q128JV SPI GPIO ����			  
	* @param   	��
	* @return   ��  	
	* Sample usage: W25Q128JV_SPI_GPIO_Config();
    */
static void W25Q128JV_SPI_GPIO_Config()
{
    GPIO_InitTypeDef GPIO_InitStructer;

    // ʹ��GPIOʱ��
    W25Q128JV_GPIO_APBxCLKCMD(W25Q128JV_GPIO_CLK, ENABLE);

    //SPI ��������
    GPIO_PinAFConfig(W25Q128JV_SPI_GPIO_PORT, W25Q128JV_SCK_PINSOURCE, GPIO_AF_SPI3);
    GPIO_PinAFConfig(W25Q128JV_SPI_GPIO_PORT, W25Q128JV_MISO_PINSOURCE, GPIO_AF_SPI3);
    GPIO_PinAFConfig(W25Q128JV_SPI_GPIO_PORT, W25Q128JV_MOSI_PINSOURCE, GPIO_AF_SPI3);

    // ����CS����
    GPIO_InitStructer.GPIO_Pin = W25Q128JV_CS_GPIO_PIN;        //CS����
    GPIO_InitStructer.GPIO_Mode = GPIO_Mode_OUT;               //���ģʽ
    GPIO_InitStructer.GPIO_OType = GPIO_OType_PP;              //�������
    GPIO_InitStructer.GPIO_PuPd = GPIO_PuPd_UP;            //����
    GPIO_InitStructer.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(W25Q128JV_CS_GPIO_PORT, &GPIO_InitStructer);
    // ����SPI GPIO
    GPIO_InitStructer.GPIO_Pin = W25Q128JV_SCK_GPIO_PIN | W25Q128JV_MISO_GPIO_PIN | W25Q128JV_MOSI_GPIO_PIN;    //SCK��MISO��MOSI����
    GPIO_InitStructer.GPIO_Mode = GPIO_Mode_AF;                //����ģʽ
    GPIO_InitStructer.GPIO_OType = GPIO_OType_PP;              //�������
    GPIO_InitStructer.GPIO_PuPd = GPIO_PuPd_UP;            //����
    GPIO_InitStructer.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(W25Q128JV_SPI_GPIO_PORT, &GPIO_InitStructer);

    W25Q128JV_CS_SET(1);    //CS����
}

/****
	* @brief	W25Q128JV SPI ����			  
	* @param   	��
	* @return   ��  	
	* Sample usage: W25Q128JV_SPI_Config();
    */
static void W25Q128JV_SPI_Config()
{
    SPI_InitTypeDef SPI_InitStructer;
    //SPI_DeInit(W25Q128JV_SPIx);    //��λSPI
    W25Q128JV_SPIx_APBxClockCMD(W25Q128JV_SPIx_CLK, ENABLE);    // ʹ��SPIʱ��

    SPI_InitStructer.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;       //������Ԥ��Ƶֵ 2��Ƶ
    SPI_InitStructer.SPI_CPHA = SPI_CPHA_2Edge;         //ʱ����λ �ڶ������ز���
    SPI_InitStructer.SPI_CPOL = SPI_CPOL_High;          //ʱ�Ӽ��� �ߵ�ƽ
    SPI_InitStructer.SPI_CRCPolynomial = 7;             //CRC��У��
    SPI_InitStructer.SPI_DataSize = SPI_DataSize_8b;    //���ݴ�С 8λ
    SPI_InitStructer.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //ȫ˫��
    SPI_InitStructer.SPI_FirstBit = SPI_FirstBit_MSB;   //��λ��ǰ
    SPI_InitStructer.SPI_Mode = SPI_Mode_Master;        //��ģʽ
    SPI_InitStructer.SPI_NSS = SPI_NSS_Soft;            //NSS�ź����������
    SPI_Init(W25Q128JV_SPIx, &SPI_InitStructer);

    SPI_Cmd(W25Q128JV_SPIx, ENABLE);        //ʹ��SPI
}

/****
	* @brief	W25Q128JV DMA ���� ����			  
	* @param   	��
	* @return   ��  	
	* Sample usage: W25Q128JV_DMA_RX_Config();
    */
static void W25Q128JV_DMA_RX_Config()
{
    DMA_InitTypeDef DMA_InitStructer;

    DMA_DeInit(W25Q128JV_DMAx_SPI_RX_Stream);    //��λDMA
    W25Q128JV_DMAxAHBxClockCMD(W25Q128JV_DMAx_CLK,ENABLE);          //ʹ��DMAʱ��

    //DMA_InitStructer.DMA_BufferSize = BuffSize;                   //��������С
    DMA_InitStructer.DMA_Channel = W25Q128JV_DMAxChannel;           //DMAͨ��
    DMA_InitStructer.DMA_DIR = DMA_DIR_PeripheralToMemory;          //���䷽�� ���赽�ڴ�
    DMA_InitStructer.DMA_FIFOMode = DMA_FIFOMode_Disable;           //FIFOģʽ �ر�
    DMA_InitStructer.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;        //FIFO��ֵ ����
    //DMA_InitStructer.DMA_Memory0BaseAddr = ;
    DMA_InitStructer.DMA_MemoryBurst = DMA_MemoryBurst_Single;      //�ڴ�ͻ�����δ���
    DMA_InitStructer.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  //�ڴ����ݴ�С �ֽ�
    DMA_InitStructer.DMA_MemoryInc = DMA_MemoryInc_Enable;          //�ڴ��ַ����    
    DMA_InitStructer.DMA_Mode = DMA_Mode_Normal;                    //����ģʽ ����ģʽ 
    DMA_InitStructer.DMA_PeripheralBaseAddr = (uint32_t)&W25Q128JV_SPIx->DR;    //�����ַ
    DMA_InitStructer.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;          //����ͻ�����δ���
    DMA_InitStructer.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;      //�������ݴ�С �ֽ�
    DMA_InitStructer.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //�����ַ������
    DMA_InitStructer.DMA_Priority = DMA_Priority_Medium;                        //���ȼ� �е�
    DMA_Init(W25Q128JV_DMAx_SPI_RX_Stream, &DMA_InitStructer);
}

/****
	* @brief	W25Q128JV DMA ���� ����			  
	* @param   	��
	* @return   ��  	
	* Sample usage: W25Q128JV_DMA_TX_Config();
    */
static void W25Q128JV_DMA_TX_Config()
{
    DMA_InitTypeDef DMA_InitStructer;

    DMA_DeInit(W25Q128JV_DMAx_SPI_TX_Stream);    //��λDMA
    W25Q128JV_DMAxAHBxClockCMD(W25Q128JV_DMAx_CLK,ENABLE);          //ʹ��DMAʱ��

    //DMA_InitStructer.DMA_BufferSize = BuffSize;                   //��������С
    DMA_InitStructer.DMA_Channel = W25Q128JV_DMAxChannel;           //DMAͨ��
    DMA_InitStructer.DMA_DIR = DMA_DIR_MemoryToPeripheral;          //���䷽�� ���赽�ڴ�
    DMA_InitStructer.DMA_FIFOMode = DMA_FIFOMode_Disable;           //FIFOģʽ �ر�
    DMA_InitStructer.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;        //FIFO��ֵ ����
    //DMA_InitStructer.DMA_Memory0BaseAddr = ;
    DMA_InitStructer.DMA_MemoryBurst = DMA_MemoryBurst_Single;      //�ڴ�ͻ�����δ���
    DMA_InitStructer.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  //�ڴ����ݴ�С �ֽ�
    DMA_InitStructer.DMA_MemoryInc = DMA_MemoryInc_Enable;          //�ڴ��ַ����    
    DMA_InitStructer.DMA_Mode = DMA_Mode_Normal;                    //����ģʽ ����ģʽ 
    DMA_InitStructer.DMA_PeripheralBaseAddr = (uint32_t)&W25Q128JV_SPIx->DR;    //�����ַ
    DMA_InitStructer.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;          //����ͻ�����δ���
    DMA_InitStructer.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;      //�������ݴ�С �ֽ�
    DMA_InitStructer.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //�����ַ������
    DMA_InitStructer.DMA_Priority = DMA_Priority_Medium;                        //���ȼ� �е�
    DMA_Init(W25Q128JV_DMAx_SPI_TX_Stream, &DMA_InitStructer);
}

/****
	* @brief	W25Q128JV ��ʼ��		  
	* @param   	��
	* @return   ��  	
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
	* @brief	W25Q128JV SPI DMA ʹ��/ʧ��		  
	* @param   	NewState    ENABLE/DISABLE
	* @return   ��  	
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
	* @brief	W25Q128JV DMA ʹ��/ʧ��		  
	* @param   	NewState    ENABLE/DISABLE
	* @return   ��  	
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
	* @brief	SPI ���� ���� һ���ֽ�����   		  
	* @param	SendByte	���͵�����
	* @return   ReceiveByte ���յ����� 	
	* Sample usage:SOFT_SPI_Config();
    */
uint8_t W25Q128JV_SPI_SendReceiveByte(uint8_t SendByte)
{
    uint8_t ReceiveByte = 0;
    uint16_t Timeout = 0;
    while (SPI_I2S_GetFlagStatus(W25Q128JV_SPIx, SPI_I2S_FLAG_TXE) == RESET)    //�ȴ����ͻ�����Ϊ��
    {
        if (Timeout++ >= W25Q128JV_SPI_Wait_TimeOut)
        {
            //break;         //��ʱ�˳�
        }
    }
    SPI_I2S_SendData(W25Q128JV_SPIx, SendByte);    //����һ���ֽ�
    Timeout = W25Q128JV_SPI_Wait_TimeOut / 2;
    while (SPI_I2S_GetFlagStatus(W25Q128JV_SPIx, SPI_I2S_FLAG_RXNE) == RESET)    //�ȴ����ջ������ǿ�
    {
        if (Timeout++ >= W25Q128JV_SPI_Wait_TimeOut)
        {
            //break;         //��ʱ�˳�
        }
    }
    ReceiveByte = SPI_I2S_ReceiveData(W25Q128JV_SPIx);    //����һ���ֽ�
    return ReceiveByte;
}

