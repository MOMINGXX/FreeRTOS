#include "W25Q128JV.h"

/****
	* @brief	W25Q128JV ��ʼ��  
	* @param   	��
	* @return   W25Q128JV_ID  	W25Q128JV
	* Sample usage:W25Q128JV_Init();
    */
void W25Q128JV_Init()
{
    W25Q128JV_Config_Init();
}

/****
	* @brief	W25Q128JV ���� 
	* @param   	��
	* @return   ��
	* Sample usage:W25Q128JV_WakeUP();
    */
void W25Q128JV_WakeUP()
{
	W25Q128JV_CS_SET(0);    
	W25Q128JV_SPI_SendReceiveByte(W25Q128JV_RELEASE_POWER_DOWN);
	W25Q128JV_CS_SET(1);    
}

/****
	* @brief	W25Q128JV ����
	* @param   	��
	* @return   ��
	* Sample usage:W25Q128JV_WakeUP();
    */
void W25Q128JV_Power_Down()
{
    W25Q128JV_CS_SET(0);   
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_POWER_DOWN);
    W25Q128JV_CS_SET(1);    
}

/****
	* @brief	W25Q128JV дʹ��	  
	* @param   	��
	* @return   ��
	* Sample usage:W25Q128JV_Write_ENABLE();
    */
void W25Q128JV_Write_ENABLE()
{
    W25Q128JV_CS_SET(0);
    //���� дʹ�� ָ��
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_WRITE_ENABLE);

    W25Q128JV_CS_SET(1);
}

/****
	* @brief	W25Q128JV ��ֹд	  
	* @param   	��
	* @return   ��
	* Sample usage:W25Q128JV_Write_DISABLE();
    */
void W25Q128JV_Write_DISABLE()
{
    W25Q128JV_CS_SET(0);
    //���� дʧ�� ָ��
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_WRITE_DISABLE);

    W25Q128JV_CS_SET(1);
}

/****
	* @brief	�ж�W25Q128JV�Ƿ�д�����  ��  �ȴ� WIP(BUSY) ��־ �� 0	  
	* @param   	��
	* @return   ��
	* Sample usage:W25Q128JV_WaitForWrite();
    */
void W25Q128JV_WaitForWrite()
{
    uint8_t Reg_Status = 0;
    W25Q128JV_SPIx_DMA_CMD(DISABLE);            //�ر�DMA
    W25Q128JV_CS_SET(0);
    //���Ͷ� �Ĵ���״̬ ָ��
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_READ_STATUS_REGISTER);

    do
    {
        Reg_Status =W25Q128JV_SPI_SendReceiveByte(DUMMY);
    } while ((Reg_Status & 0x01) == SET);
    
    W25Q128JV_CS_SET(1);
}

/****
	* @brief	���� W25Q128JV 
	* @param   	��
	* @return   ��
	* Sample usage:W25Q128JV_Chip_Erase();
    */
void W25Q128JV_Chip_Erase()
{
    W25Q128JV_SPIx_DMA_CMD(DISABLE);            //�ر�DMA
    //дʹ��
    W25Q128JV_Write_ENABLE();
    W25Q128JV_WaitForWrite();

    W25Q128JV_CS_SET(0);

    //���� Ƭ���� ָ��
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_CHIP_ERASE);

	W25Q128JV_CS_SET(1);
    W25Q128JV_WaitForWrite();
}

/****
	* @brief	���� W25Q128JV ����
	* @param   	Addr    ������ַ    4KB
	* @return   ��
	* Sample usage:W25Q128JV_Sector_Erase();
    */
void W25Q128JV_Sector_Erase(uint32_t Addr)
{
    W25Q128JV_SPIx_DMA_CMD(DISABLE);            //�ر�DMA
    W25Q128JV_Write_ENABLE();    //дʹ��
    W25Q128JV_WaitForWrite();
    W25Q128JV_CS_SET(0);    //Ƭѡ
    //���� �������� ָ��
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_SECTOR_ERASE); 
	W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF0000) >> 16);		//���͵�ַ ��8λ
    W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF00) >> 8);        //���͵�ַ ��8λ
    W25Q128JV_SPI_SendReceiveByte(Addr & 0xFF);                 //���͵�ַ ��8λ
    W25Q128JV_CS_SET(1);
    W25Q128JV_WaitForWrite();   
}

/****
	* @brief	���� W25Q128JV ��
	* @param   	Addr    ���ַ  32KB
	* @return   ��
	* Sample usage:W25Q128JV_Sector_Erase();
    */
void W25Q128JV_Block_Erase(uint32_t Addr)
{
    W25Q128JV_SPIx_DMA_CMD(DISABLE);            //�ر�DMA
    W25Q128JV_Write_ENABLE();    //дʹ��
    W25Q128JV_WaitForWrite();
    W25Q128JV_CS_SET(0);    //Ƭѡ
    //���� ����� ָ��
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_BLOCK_ERASE1);
	W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF0000) >> 16);		//���͵�ַ ��8λ
    W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF00) >> 8);        //���͵�ַ ��8λ
    W25Q128JV_SPI_SendReceiveByte(Addr & 0xFF);                 //���͵�ַ ��8λ
    W25Q128JV_CS_SET(1);
    W25Q128JV_WaitForWrite();
}

/****
	* @brief	W25Q128JV ��ID	    0xEF4018	  
	* @param   	��
	* @return   W25Q128JV_ID  	W25Q128JV��ID
	* Sample usage:W25Q128JV_Read_JEDEC_ID();
    */
uint32_t W25Q128JV_Read_JEDEC_ID()
{
    uint32_t W25Q128JV_ID = 0;
    W25Q128JV_SPIx_DMA_CMD(DISABLE);            //�ر�DMA
    W25Q128JV_CS_SET(0);                        //Ƭѡ
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_JEDEC_ID);    //���Ͷ�ȡIDָ��
    W25Q128JV_ID = W25Q128JV_SPI_SendReceiveByte(DUMMY) << 16;    //����ID ��ʶ
    W25Q128JV_ID |= W25Q128JV_SPI_SendReceiveByte(DUMMY) << 8;    //�豸ID ��8λ
    W25Q128JV_ID |= W25Q128JV_SPI_SendReceiveByte(DUMMY);         //�豸ID ��8λ
    W25Q128JV_CS_SET(1);                        //ȡ��Ƭѡ
    return W25Q128JV_ID;
}

/****
	* @brief	W25Q128JV ��ID	    0x15	  
	* @param   	��
	* @return   W25Q128JV_ID  	W25Q128JV��ID
	* Sample usage:W25Q128JV_Read_Device_ID();
    */
uint8_t W25Q128JV_Read_Device_ID()
{
	uint8_t Device_ID;
	W25Q128JV_SPIx_DMA_CMD(DISABLE);           //�ر�DMA
    W25Q128JV_CS_SET(0);   
	W25Q128JV_SPI_SendReceiveByte(W25Q128JV_DEVICE_ID);    //���Ͷ�ȡIDָ��
	W25Q128JV_SPI_SendReceiveByte(DUMMY);
	W25Q128JV_SPI_SendReceiveByte(DUMMY);
	W25Q128JV_SPI_SendReceiveByte(DUMMY);
	Device_ID = W25Q128JV_SPI_SendReceiveByte(DUMMY);     //�豸ID
	W25Q128JV_CS_SET(1);                        //ȡ��Ƭѡ
    return Device_ID;
}

/****
	* @brief	��ȡ W25Q128JV ����
	* @param   	Addr    ��ȡ�ĵ�ַ      0x000000~0x7FFFFF
    * @param   	Data    �洢��ȡ������
    * @param   	Num     ��ȡ���ݵĳ���  0~0x800000
	* @return   ��
	* Sample usage:W25Q128JV_Read_Data(0,buff,4096);
    */
#ifdef W25Q128JV_USE_DMADIS        //��ʹ��DMA
void W25Q128JV_Read_Data(uint32_t Addr,uint8_t *Data,uint32_t Num)
{
    W25Q128JV_CS_SET(0);               //Ƭѡ
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_READ_DATA);    //���Ͷ� ���� ָ��

	W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF0000) >> 16);		//���͵�ַ ��8λ
    W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF00) >> 8);        //���͵�ַ ��8λ
    W25Q128JV_SPI_SendReceiveByte(Addr & 0xFF);                 //���͵�ַ ��8λ

    while(Num--)
    {
        *Data++ = W25Q128JV_SPI_SendReceiveByte(DUMMY);    //��������
    }
    W25Q128JV_CS_SET(1);                //ȡ��Ƭѡ
}

/****
	* @brief	W25Q128JV ҳд������
	* @param   	Addr    ��ȡ�ĵ�ַ
    * @param   	Data    Ҫд�������
    * @param   	Num     д�����ݵĳ���	1-256
	* @return   ��
	* Sample usage:W25Q128JV_Page_Program(0,buff,256);
    */
void W25Q128JV_Page_Program(uint32_t Addr,uint8_t *Data,uint16_t Num)
{
    W25Q128JV_Write_ENABLE();           //дʹ��
    W25Q128JV_WaitForWrite();           //�ȴ�д�����
    W25Q128JV_CS_SET(0);                //Ƭѡ
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_PAGE_PROGRAM);    	//����ҳд��ָ��

	W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF0000) >> 16);		//���͵�ַ ��8λ
    W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF00) >> 8);        //���͵�ַ ��8λ
    W25Q128JV_SPI_SendReceiveByte(Addr & 0xFF);                 //���͵�ַ ��8λ
																
    while(Num--)
    {
        W25Q128JV_SPI_SendReceiveByte(*Data++);            //��������
    }
    W25Q128JV_CS_SET(1);                //ȡ��Ƭѡ
    W25Q128JV_WaitForWrite();           //�ȴ�д�����
}

#else //ʹ��DMA
/****
	* @brief	��ȡ W25Q128JV ����
	* @param   	Addr    ��ȡ�ĵ�ַ      0x000000~0x7FFFFF
    * @param   	Data    �洢��ȡ������
    * @param   	Num     ��ȡ���ݵĳ���  0~0x800000
	* @return   ��
	* Sample usage:W25Q128JV_Read_Data(0,buff,4096);
    */
void W25Q128JV_Read_Data(uint32_t Addr,uint8_t *Data,uint32_t Num)
{
    //�����ֽ�
    uint8_t DummyData[Num];     //����һ���������ڴ洢Dummy����
    memset(DummyData,0,Num);    //��DummyData����ȫ����Ϊ0
	
    W25Q128JV_SPIx_DMA_CMD(DISABLE);
    W25Q128JV_CS_SET(0);
    //���Ͷ� ���� ָ��
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_READ_DATA);

    //��ַ��λ
    W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF0000) >> 16);
    W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF00) >> 8);
    //��ַ��λ
    W25Q128JV_SPI_SendReceiveByte(Addr & 0xFF);
    
    W25Q128JV_DMA_CMD(DISABLE);    //�ر�DMA

    //��������
    DMA_SetCurrDataCounter(W25Q128JV_DMAx_SPI_RX_Stream,Num);    //����DMA�������ݳ���
    W25Q128JV_DMAx_SPI_RX_Stream->M0AR = (uint32_t)Data;        //����DMA�������ݵ�ַ

    //��������
    DMA_SetCurrDataCounter(W25Q128JV_DMAx_SPI_TX_Stream,Num);    //����DMA�������ݳ���
    W25Q128JV_DMAx_SPI_TX_Stream->M0AR = (uint32_t)DummyData;    //����DMA�������ݵ�ַ
   
    W25Q128JV_SPIx_DMA_CMD(ENABLE);      //����SPI DMA
    W25Q128JV_DMA_CMD(ENABLE);           //����DMA

    //�ȴ��������
    while(DMA_GetFlagStatus(W25Q128JV_DMAx_SPI_RX_Stream,DMA_FLAG_TCIF2) == RESET);    //�ȴ�DMA�������
    DMA_ClearFlag(W25Q128JV_DMAx_SPI_RX_Stream,DMA_FLAG_TCIF2);    //���DMA������ɱ�־λ
    //�ȴ��������
    while(DMA_GetFlagStatus(W25Q128JV_DMAx_SPI_TX_Stream,DMA_FLAG_TCIF5) == RESET);    //�ȴ�DMA�������
    DMA_ClearFlag(W25Q128JV_DMAx_SPI_TX_Stream,DMA_FLAG_TCIF5);    //���DMA������ɱ�־λ
    
    W25Q128JV_CS_SET(1);            //ȡ��Ƭѡ
}

/****
	* @brief	W25Q128JV ҳд������
	* @param   	Addr    ��ȡ�ĵ�ַ
    * @param   	Data    Ҫд�������
    * @param   	Num     д�����ݵĳ���	1-256
	* @return   ��
	* Sample usage:W25Q128JV_Page_Program(0,buff,256);
    */
void W25Q128JV_Page_Program(uint32_t Addr,uint8_t *Data,uint16_t Num)
{
    uint8_t DummyData[Num];     //����һ���������ڴ洢Dummy����
    memset(DummyData,0,Num);    //��DummyData����ȫ����Ϊ0

    W25Q128JV_Write_ENABLE();   //дʹ��

    W25Q128JV_SPIx_DMA_CMD(DISABLE);    //�ر�SPI DMA

    W25Q128JV_WaitForWrite();           //�ȴ�д�����

    W25Q128JV_CS_SET(0);

    //���� ҳд�� ָ��
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_PAGE_PROGRAM);
	
	//���Ϳ��λ��ַ
    W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF0000) >> 16);
    //���Ϳ���λ��ַ
    W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF00) >> 8);
    //���Ϳ��λ��ַ
    W25Q128JV_SPI_SendReceiveByte(Addr & 0xFF);
	
    W25Q128JV_DMA_CMD(DISABLE);    //�ر�DMA

    //��������
    DMA_SetCurrDataCounter(W25Q128JV_DMAx_SPI_TX_Stream,Num);    //����DMA�������ݳ���
    W25Q128JV_DMAx_SPI_TX_Stream->M0AR = (uint32_t)Data;        //����DMA�������ݵ�ַ

    //��������
    DMA_SetCurrDataCounter(W25Q128JV_DMAx_SPI_RX_Stream,Num);    //����DMA�������ݳ���
    W25Q128JV_DMAx_SPI_RX_Stream->M0AR = (uint32_t)DummyData;    //����DMA�������ݵ�ַ

    W25Q128JV_SPIx_DMA_CMD(ENABLE);            //����SPI DMA
    W25Q128JV_DMA_CMD(ENABLE);                  //����DMA

    //�ȴ��������
    while(DMA_GetFlagStatus(W25Q128JV_DMAx_SPI_TX_Stream,DMA_FLAG_TCIF5) == RESET);    //�ȴ�DMA�������
    DMA_ClearFlag(W25Q128JV_DMAx_SPI_TX_Stream,DMA_FLAG_TCIF5);    //���DMA������ɱ�־λ

    //�ȴ��������
    while(DMA_GetFlagStatus(W25Q128JV_DMAx_SPI_RX_Stream,DMA_FLAG_TCIF2) == RESET);    //�ȴ�DMA�������
    DMA_ClearFlag(W25Q128JV_DMAx_SPI_RX_Stream,DMA_FLAG_TCIF2);    //���DMA������ɱ�־λ

	W25Q128JV_CS_SET(1);
    W25Q128JV_WaitForWrite();
}
#endif //W25Q128JV_USE_DMADIS

/****
	* @brief	W25Q128JV д������
	* @param   	Addr    ��ȡ�ĵ�ַ
    * @param   	Data    Ҫд�������
    * @param   	Num     д�����ݵĳ���	
	* @return   ��
	* Sample usage:W25Q128JV_Buff_Write(0,buff,256);
    */
void W25Q128JV_Buff_Write(uint32_t Addr,uint8_t *Data,uint32_t Num)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr_Temp = 0, count = 0, temp = 0;
    /*mod�������࣬��Addr��W25Q128JV_PageSize��������������Addr_TempֵΪ0*/
    Addr_Temp = Addr % W25Q128JV_PageSize;
	
	/*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
    count = W25Q128JV_PageSize - Addr_Temp;
	/*�����Ҫд��������ҳ*/
    NumOfPage =  Num / W25Q128JV_PageSize;
	/*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
    NumOfSingle = Num % W25Q128JV_PageSize;
	
	/* Addr_Temp=0,��WriteAddr �պð�ҳ���� aligned  */
    if (Addr_Temp == 0)
    {
        /* Num < W25Q128JV_PageSize */
        if (NumOfPage == 0) 
        {
            W25Q128JV_Page_Program(Addr,Data,Num);
        }
        else /* Num > W25Q128JV_PageSize */
        { 
            /*�Ȱ�����ҳ��д��*/
            while (NumOfPage--)
            {
                W25Q128JV_Page_Program(Addr,Data,W25Q128JV_PageSize);
                Addr += W25Q128JV_PageSize;
                Data += W25Q128JV_PageSize;
            }
            /*���ж���Ĳ���һҳ�����ݣ�����д��*/
            W25Q128JV_Page_Program(Addr,Data,NumOfSingle);
        }
    }
    /* ����ַ�� W25Q128JV_PageSize ������  */
    else 
    {
        /* Num < W25Q128JV_PageSize */
        if (NumOfPage == 0)
        {
            /*��ǰҳʣ���count��λ�ñ�NumOfSingleС��һҳд����*/
            if (NumOfSingle > count) 
            {
                temp = NumOfSingle - count;
                 /*��д����ǰҳ*/
                W25Q128JV_Page_Program(Addr,Data,count);
                        
                Addr +=  count;
                Data += count;
                /*��дʣ�������*/
                W25Q128JV_Page_Program(Addr,Data,temp);
            }
            else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
            {
                W25Q128JV_Page_Program(Addr,Data,Num);
            }
        }
        else /* Num > W25Q128JV_PageSize */
        {
            /*��ַ����������count�ֿ������������������*/
            Num -= count;
            NumOfPage = Num / W25Q128JV_PageSize;
            NumOfSingle = Num % W25Q128JV_PageSize;
                    
            /* ��д��count�����ݣ�Ϊ��������һ��Ҫд�ĵ�ַ���� */
            W25Q128JV_Page_Program(Addr,Data,count);
                    
            /* ���������ظ���ַ�������� */
            Addr +=  count;
            Data += count;
                /*������ҳ��д��*/
            while (NumOfPage--)
            {
                W25Q128JV_Page_Program(Addr,Data,W25Q128JV_PageSize);
                Addr += W25Q128JV_PageSize;
                Data += W25Q128JV_PageSize;
            }
            /*���ж���Ĳ���һҳ�����ݣ�����д��*/
            if (NumOfSingle != 0)
            {
                W25Q128JV_Page_Program(Addr,Data,NumOfSingle);
            }
        }
    }
}

