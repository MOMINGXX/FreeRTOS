#include "NRF24L01.h"

/****
	* @brief	NRF24L01 SPI GPIO ����			  
	* @param   	��
	* @return   ��  	
	* Sample usage: NRF24L01_GPIO_Config();
    */
static void NRF24L01_GPIO_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NRF24L01_GPIO_APBxClockCMD(NRF24L01_GPIO_CLK, ENABLE);      //GPIO ʱ��ʹ��

    GPIO_InitStructure.GPIO_Pin = NRF24L01_CE_GPIO_PIN | NRF24L01_CS_GPIO_PIN | NRF24L01_IRQ_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;               //GPIO ģʽΪ���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;              //GPIO ���ģʽΪ�������      
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             	//GPIO ����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(NRF24L01_GPIO_PORT, &GPIO_InitStructure);
}

/****
	* @brief	NRF24L01 ��ʼ��			  
	* @param   	��
	* @return   ��  	
	* Sample usage: NRF24L01_Init();
    */
void NRF24L01_Init()
{
    NRF24L01_GPIO_Config();
    NRF24L01_SPI_Init();
}

/****
	* @brief    NRF24L01 ���ض��ļĴ���д������ 	  
	* @param   	Reg: NRF24L01 ������ + �Ĵ�����ַ
	* @param    Data: ��Ĵ���д������� 
    * @return   Status���Ĵ�����״̬
	* Sample usage:NRF24L01_WriteReg(NOP,NOP);
    */
static uint8_t NRF24L01_WriteReg(uint8_t Reg,uint8_t Data)
{
    uint8_t Status;
    NRF24L01_CS_LOW();          //Ƭѡ
    Status = NRF24L01_Send_RecieveByte(Reg);    //���ͼĴ�����ַ
    NRF24L01_Send_RecieveByte(Data);            //д������
    NRF24L01_CS_HIGH();         //ȡ��Ƭѡ
    return Status;
}

/****
	* @brief    NRF24L01 ���ض��ļĴ���д�������� 	  
	* @param   	Reg: NRF24L01 ������ + �Ĵ�����ַ
	* @param    Buff: ���ڴ洢Ҫд��Ĵ������ݵ�����
    * @param    Len: Buff�����ݳ���
    * @return   Status���Ĵ�����״̬
	* Sample usage:NRF24L01_WriteBuff(NOP,NOP,6);
    */
static uint8_t NRF24L01_WriteBuff(uint8_t Reg,uint8_t *Buff,uint8_t Len)
{
    uint8_t Status;
    NRF24L01_CS_LOW();          //Ƭѡ
    Status = NRF24L01_Send_RecieveByte(Reg);    //���ͼĴ�����ַ
    for(int i = 0;i < Len;i++)
    {
        NRF24L01_Send_RecieveByte(*Buff++);    //д������
    }
    NRF24L01_CS_HIGH();         //ȡ��Ƭѡ
    return Status;
}

/****
	* @brief    NRF24L01 ���ض��ļĴ��������� 	  
	* @param   	Reg: NRF24L01 ������ + �Ĵ�����ַ
    * @return   Data���Ĵ����е�����
	* Sample usage:NRF24L01_ReadReg(NOP);
    */
static uint8_t NRF24L01_ReadReg(uint8_t reg)
{
    uint8_t Data;
    NRF24L01_CS_LOW();          //Ƭѡ
    NRF24L01_Send_RecieveByte(reg);    //���ͼĴ�����ַ
    Data = NRF24L01_Send_RecieveByte(NOP);    //��ȡ����
    NRF24L01_CS_HIGH();         //ȡ��Ƭѡ
    return Data;
}

/****
	* @brief    NRF24L01 ���ض��ļĴ������������ 	  
	* @param   	Reg: NRF24L01 ������ + �Ĵ�����ַ
    * @return   Buff�����ڴ洢�������Ĵ������ݵ�����
    * @param    Len: Buff�����ݳ���
    * @return   Status���Ĵ�����״̬
	* Sample usage:NRF24L01_ReadBuff(Reg,Buff,6);
    */
static uint8_t NRF24L01_ReadBuff(uint8_t Reg,uint8_t *Buff,uint8_t Len)
{
    uint8_t Status;
    NRF24L01_CS_LOW();          //Ƭѡ
    Status = NRF24L01_Send_RecieveByte(Reg);    //���ͼĴ�����ַ
    for(int i = 0;i < Len;i++)
    {
        *Buff++ = NRF24L01_Send_RecieveByte(NOP);    //��ȡ����
    }
    NRF24L01_CS_HIGH();         //ȡ��Ƭѡ
    return Status;
}

void NRF24L01_RX_Mode()
{

}

void NRF24L01_TX_Mode()
{

}

/****
	* @brief    ���NRF24L01�Ƿ����  
	* @param   	��
    * @return   SUCCESS   �ɹ�
    *           ERROR     ʧ��
	* Sample usage:NRF24L01_Check();
    */
uint8_t NRF24L01_Check()
{
	uint8_t Tx_Buff[5] = {0xC2,0xC2,0xC2,0xC2,0xC2};
	uint8_t Rx_Buff[5] = { 0 };
	uint8_t i; 
	 
	//д��5���ֽڵĵ�ַ
	NRF24L01_WriteBuff(NRF24L01_W_REGISTER + NRF24L01_TX_ADDR,Tx_Buff,5);

	//����д��ĵ�ַ 
	NRF24L01_ReadBuff(NRF24L01_TX_ADDR,Rx_Buff,5); 
	 
	//�Ƚ�             
	for(i = 0; i < 5; i++)
	{
		if(Tx_Buff[i] != Rx_Buff[i])
		break;
	} 
	       
	if(i != 5)
		return ERROR ;        //MCU��NRF24L01����ʧ��
	else
		return SUCCESS ;      //MCU��NRF24L01��������
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

