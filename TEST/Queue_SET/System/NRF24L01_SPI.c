#include "NRF24L01_SPI.h"

#define NRF24L01_SPIx_WateTime      ((uint16_t)0x0500)          //�ȴ�ʱ��

/****
	* @brief	NRF24L01 SPI GPIO ����			  
	* @param   	��
	* @return   ��  	
	* Sample usage: NRF24L01_SPI_GPIO_Config();
    */
static void NRF24L01_SPI_GPIO_Config()
{
    GPIO_InitTypeDef GPIO_InitStructer;
    NRF24L01_GPIO_AHBxClockCMD(NRF24L01_GPIOx_CLK,ENABLE);

    //SPI ���� ����
    GPIO_PinAFConfig(NRF24L01_GPIOx_PORT,NRF24L01_SCK_PINSOURCE,GPIO_AF_SPI1);
    GPIO_PinAFConfig(NRF24L01_GPIOx_PORT,NRF24L01_MISO_PINSOURCE,GPIO_AF_SPI1);
    GPIO_PinAFConfig(NRF24L01_GPIOx_PORT,NRF24L01_MOSI_PINSOURCE,GPIO_AF_SPI1);

    GPIO_InitStructer.GPIO_Mode = GPIO_Mode_AF;         //����ģʽ
    GPIO_InitStructer.GPIO_OType = GPIO_OType_PP;       //�������
    GPIO_InitStructer.GPIO_Pin = NRF24L01_MISO_GPIO_PIN | NRF24L01_MOSI_GPIO_PIN | NRF24L01_SCK_GPIO_PIN;
    GPIO_InitStructer.GPIO_PuPd = GPIO_PuPd_UP;     	//����
    GPIO_InitStructer.GPIO_Speed = GPIO_Speed_100MHz;   
    GPIO_Init(NRF24L01_GPIOx_PORT,&GPIO_InitStructer);
}

/****
	* @brief	NRF24L01 SPI ����			  
	* @param   	��
	* @return   ��  	
	* Sample usage: NRF24L01_SPI_Config();
    */
static void NRF24L01_SPI_Config()
{
    SPI_InitTypeDef SPI_InitStructer;
    NRF24L01_SPIx_APBxClockCMD(NRF24L01_SPIx_CLK,ENABLE);

    SPI_InitStructer.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;      //������Ԥ��Ƶ 16��Ƶ
    SPI_InitStructer.SPI_CPHA = SPI_CPHA_1Edge;                             //ʱ����λ  �ڶ������ز���
    SPI_InitStructer.SPI_CPOL = SPI_CPOL_Low;                              //ʱ�Ӽ���  ����ʱSCLKΪ�ߵ�ƽ
    SPI_InitStructer.SPI_CRCPolynomial = 7;                                 //CRCУ�����ʽ
    SPI_InitStructer.SPI_DataSize = SPI_DataSize_8b;                        //���ݴ�С 8λ
    SPI_InitStructer.SPI_Direction = SPI_Direction_2Lines_FullDuplex;       //ȫ˫��
    SPI_InitStructer.SPI_FirstBit = SPI_FirstBit_MSB;                       //���ݴ���Ӹ�λ��ʼ
    SPI_InitStructer.SPI_Mode = SPI_Mode_Master;                            //��ģʽ
    SPI_InitStructer.SPI_NSS = SPI_NSS_Soft;                                //NSS�ź����������
    SPI_Init(NRF24L01_SPIx,&SPI_InitStructer);

    SPI_Cmd(NRF24L01_SPIx,ENABLE);                   //ʹ��SPI
}

/****
	* @brief	NRF24L01 SPI ��ʼ��			  
	* @param   	��
	* @return   ��  	
	* Sample usage: NRF24L01_SPI_Init();
    */
void NRF24L01_SPI_Init()
{
    NRF24L01_SPI_GPIO_Config();
    NRF24L01_SPI_Config();
}

/****
	* @brief	SPI ���� ���� һ���ֽ�����   		  
	* @param	SendByte	���͵�����
	* @return   ReceiveByte ���յ����� 	
	* Sample usage:SOFT_SPI_Config();
    */
uint8_t NRF24L01_Send_RecieveByte(uint8_t SendByte)
{
    uint16_t TimeOut = 0;
    while(SPI_I2S_GetFlagStatus(NRF24L01_SPIx,SPI_I2S_FLAG_TXE) == RESET)     //�ȴ����ͻ�����Ϊ��
    {
        if(++TimeOut >= NRF24L01_SPIx_WateTime)
        {
            break;      //��ʱ�˳�
        }
    }
    TimeOut = 0;
    SPI_I2S_SendData(NRF24L01_SPIx,SendByte);                                //����һ���ֽ�
    while(SPI_I2S_GetFlagStatus(NRF24L01_SPIx,SPI_I2S_FLAG_RXNE) == RESET)   //�ȴ����ջ������ǿ�
    {
        if(++TimeOut >= NRF24L01_SPIx_WateTime)     
        {
            break;      //��ʱ�˳�
        }
    }
    return SPI_I2S_ReceiveData(NRF24L01_SPIx);                         		//��ȡһ���ֽ�
}
