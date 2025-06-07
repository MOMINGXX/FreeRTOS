#include "LCD_SPI.h"

uint16_t DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ��� 

/****
	* @brief    LCD GPIO����		  
	* @param   	��
	* @return   ��
	* Sample usage:LCD_GPIO_Config();
    */
void LCD_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	LCD_GPIO_AHBxClockCMD(LCD_GPIO_CLK, ENABLE);	 //ʹ��

	GPIO_PinAFConfig(LCD_GPIO_PORT, LCD_SCL_GPIO_PINSOURCE, LCD_SPI_SCLK_AF);	//����
	GPIO_PinAFConfig(LCD_GPIO_PORT, LCD_SDA_GPIO_PINSOURCE, LCD_SPI_MOSI_AF);	//����
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 		 	//����	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//����
	GPIO_InitStructure.GPIO_Pin = LCD_SCL_GPIO_PIN | LCD_SDA_GPIO_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//�ٶ� 100MHz
	GPIO_Init(LCD_GPIO_PORT, &GPIO_InitStructure);	  		

	GPIO_SetBits(LCD_GPIO_PORT, LCD_SCL_GPIO_PIN | LCD_SDA_GPIO_PIN);	//����
}

/****
	* @brief    LCD SPI����	  
	* @param   	��
	* @return   ��
	* Sample usage:LCD_SPIx_Config();
    */
void LCD_SPIx_Config()
{
	SPI_InitTypeDef SPI_InitStruct;
	LCD_SPIx_APBxClockCMD(LCD_SPIx_CLK,ENABLE);	 //ʹ�� SPI
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;     //������Ԥ��Ƶֵ
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;                           //(ʱ����λ)�ڶ��������أ��������أ����ݱ�����
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;                            //(ʱ�Ӽ���)����״̬Ϊ�ߵ�ƽ
	SPI_InitStruct.SPI_CRCPolynomial = 7;                               //CRCУ��  ��ʹ��
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;                      //����֡��СΪ8λ
	SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx;              //SPI ����Ϊ���߽�����ģʽ
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;                     //���ݴ���� MSB λ��ʼ
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;                          //Ϊ����
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;                              //�ڲ� NSS �ź��� SSI λ����
	SPI_Init(LCD_SPIx,&SPI_InitStruct);

	SPI_Cmd(LCD_SPIx,ENABLE);
}

/****
	* @brief    LCD SPI ��ʼ��			  
	* @param   	��
	* @return   ��
	* Sample usage:LCD_SPIx_Init();
    */
void LCD_SPIx_Init()
{
	LCD_GPIO_Config();
	LCD_SPIx_Config();
}

/****
	* @brief    LCD SPI SPI ���� һ���ֽ����� 			  
	* @param   	Byte	���͵�����
	* @return   ��
	* Sample usage:LCD_SPI_WriteByte(0x56);
    */
void LCD_SPI_WriteByte(uint8_t Byte)
{
	while(RESET == SPI_I2S_GetFlagStatus(LCD_SPIx,SPI_I2S_FLAG_TXE));	//�ȴ����ͻ�����Ϊ��
	SPI_I2S_SendData(LCD_SPIx,Byte);									//����һ���ֽڵ�����
	while(RESET != SPI_I2S_GetFlagStatus(LCD_SPIx,SPI_I2S_FLAG_BSY));  	//�ȴ��������
}
 
