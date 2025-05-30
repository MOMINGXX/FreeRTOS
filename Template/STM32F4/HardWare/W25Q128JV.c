#include "W25Q128JV.h"

/****
	* @brief	W25Q128JV 初始化  
	* @param   	无
	* @return   W25Q128JV_ID  	W25Q128JV
	* Sample usage:W25Q128JV_Init();
    */
void W25Q128JV_Init()
{
    W25Q128JV_Config_Init();
}

/****
	* @brief	W25Q128JV 唤醒 
	* @param   	无
	* @return   无
	* Sample usage:W25Q128JV_WakeUP();
    */
void W25Q128JV_WakeUP()
{
	W25Q128JV_CS_SET(0);    
	W25Q128JV_SPI_SendReceiveByte(W25Q128JV_RELEASE_POWER_DOWN);
	W25Q128JV_CS_SET(1);    
}

/****
	* @brief	W25Q128JV 休眠
	* @param   	无
	* @return   无
	* Sample usage:W25Q128JV_WakeUP();
    */
void W25Q128JV_Power_Down()
{
    W25Q128JV_CS_SET(0);   
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_POWER_DOWN);
    W25Q128JV_CS_SET(1);    
}

/****
	* @brief	W25Q128JV 写使能	  
	* @param   	无
	* @return   无
	* Sample usage:W25Q128JV_Write_ENABLE();
    */
void W25Q128JV_Write_ENABLE()
{
    W25Q128JV_CS_SET(0);
    //发送 写使能 指令
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_WRITE_ENABLE);

    W25Q128JV_CS_SET(1);
}

/****
	* @brief	W25Q128JV 禁止写	  
	* @param   	无
	* @return   无
	* Sample usage:W25Q128JV_Write_DISABLE();
    */
void W25Q128JV_Write_DISABLE()
{
    W25Q128JV_CS_SET(0);
    //发送 写失能 指令
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_WRITE_DISABLE);

    W25Q128JV_CS_SET(1);
}

/****
	* @brief	判断W25Q128JV是否写入完成  及  等待 WIP(BUSY) 标志 置 0	  
	* @param   	无
	* @return   无
	* Sample usage:W25Q128JV_WaitForWrite();
    */
void W25Q128JV_WaitForWrite()
{
    uint8_t Reg_Status = 0;
    W25Q128JV_SPIx_DMA_CMD(DISABLE);            //关闭DMA
    W25Q128JV_CS_SET(0);
    //发送读 寄存器状态 指令
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_READ_STATUS_REGISTER);

    do
    {
        Reg_Status =W25Q128JV_SPI_SendReceiveByte(DUMMY);
    } while ((Reg_Status & 0x01) == SET);
    
    W25Q128JV_CS_SET(1);
}

/****
	* @brief	擦除 W25Q128JV 
	* @param   	无
	* @return   无
	* Sample usage:W25Q128JV_Chip_Erase();
    */
void W25Q128JV_Chip_Erase()
{
    W25Q128JV_SPIx_DMA_CMD(DISABLE);            //关闭DMA
    //写使能
    W25Q128JV_Write_ENABLE();
    W25Q128JV_WaitForWrite();

    W25Q128JV_CS_SET(0);

    //发送 片擦除 指令
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_CHIP_ERASE);

	W25Q128JV_CS_SET(1);
    W25Q128JV_WaitForWrite();
}

/****
	* @brief	擦除 W25Q128JV 扇区
	* @param   	Addr    扇区地址    4KB
	* @return   无
	* Sample usage:W25Q128JV_Sector_Erase();
    */
void W25Q128JV_Sector_Erase(uint32_t Addr)
{
    W25Q128JV_SPIx_DMA_CMD(DISABLE);            //关闭DMA
    W25Q128JV_Write_ENABLE();    //写使能
    W25Q128JV_WaitForWrite();
    W25Q128JV_CS_SET(0);    //片选
    //发送 扇区擦除 指令
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_SECTOR_ERASE); 
	W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF0000) >> 16);		//发送地址 高8位
    W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF00) >> 8);        //发送地址 中8位
    W25Q128JV_SPI_SendReceiveByte(Addr & 0xFF);                 //发送地址 低8位
    W25Q128JV_CS_SET(1);
    W25Q128JV_WaitForWrite();   
}

/****
	* @brief	擦除 W25Q128JV 块
	* @param   	Addr    块地址  32KB
	* @return   无
	* Sample usage:W25Q128JV_Sector_Erase();
    */
void W25Q128JV_Block_Erase(uint32_t Addr)
{
    W25Q128JV_SPIx_DMA_CMD(DISABLE);            //关闭DMA
    W25Q128JV_Write_ENABLE();    //写使能
    W25Q128JV_WaitForWrite();
    W25Q128JV_CS_SET(0);    //片选
    //发送 块擦除 指令
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_BLOCK_ERASE1);
	W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF0000) >> 16);		//发送地址 高8位
    W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF00) >> 8);        //发送地址 中8位
    W25Q128JV_SPI_SendReceiveByte(Addr & 0xFF);                 //发送地址 低8位
    W25Q128JV_CS_SET(1);
    W25Q128JV_WaitForWrite();
}

/****
	* @brief	W25Q128JV 的ID	    0xEF4018	  
	* @param   	无
	* @return   W25Q128JV_ID  	W25Q128JV的ID
	* Sample usage:W25Q128JV_Read_JEDEC_ID();
    */
uint32_t W25Q128JV_Read_JEDEC_ID()
{
    uint32_t W25Q128JV_ID = 0;
    W25Q128JV_SPIx_DMA_CMD(DISABLE);            //关闭DMA
    W25Q128JV_CS_SET(0);                        //片选
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_JEDEC_ID);    //发送读取ID指令
    W25Q128JV_ID = W25Q128JV_SPI_SendReceiveByte(DUMMY) << 16;    //工厂ID 标识
    W25Q128JV_ID |= W25Q128JV_SPI_SendReceiveByte(DUMMY) << 8;    //设备ID 高8位
    W25Q128JV_ID |= W25Q128JV_SPI_SendReceiveByte(DUMMY);         //设备ID 低8位
    W25Q128JV_CS_SET(1);                        //取消片选
    return W25Q128JV_ID;
}

/****
	* @brief	W25Q128JV 的ID	    0x15	  
	* @param   	无
	* @return   W25Q128JV_ID  	W25Q128JV的ID
	* Sample usage:W25Q128JV_Read_Device_ID();
    */
uint8_t W25Q128JV_Read_Device_ID()
{
	uint8_t Device_ID;
	W25Q128JV_SPIx_DMA_CMD(DISABLE);           //关闭DMA
    W25Q128JV_CS_SET(0);   
	W25Q128JV_SPI_SendReceiveByte(W25Q128JV_DEVICE_ID);    //发送读取ID指令
	W25Q128JV_SPI_SendReceiveByte(DUMMY);
	W25Q128JV_SPI_SendReceiveByte(DUMMY);
	W25Q128JV_SPI_SendReceiveByte(DUMMY);
	Device_ID = W25Q128JV_SPI_SendReceiveByte(DUMMY);     //设备ID
	W25Q128JV_CS_SET(1);                        //取消片选
    return Device_ID;
}

/****
	* @brief	读取 W25Q128JV 数据
	* @param   	Addr    读取的地址      0x000000~0x7FFFFF
    * @param   	Data    存储读取的数据
    * @param   	Num     读取数据的长度  0~0x800000
	* @return   无
	* Sample usage:W25Q128JV_Read_Data(0,buff,4096);
    */
#ifdef W25Q128JV_USE_DMADIS        //不使用DMA
void W25Q128JV_Read_Data(uint32_t Addr,uint8_t *Data,uint32_t Num)
{
    W25Q128JV_CS_SET(0);               //片选
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_READ_DATA);    //发送读 数据 指令

	W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF0000) >> 16);		//发送地址 高8位
    W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF00) >> 8);        //发送地址 中8位
    W25Q128JV_SPI_SendReceiveByte(Addr & 0xFF);                 //发送地址 低8位

    while(Num--)
    {
        *Data++ = W25Q128JV_SPI_SendReceiveByte(DUMMY);    //接收数据
    }
    W25Q128JV_CS_SET(1);                //取消片选
}

/****
	* @brief	W25Q128JV 页写入数据
	* @param   	Addr    读取的地址
    * @param   	Data    要写入的数据
    * @param   	Num     写入数据的长度	1-256
	* @return   无
	* Sample usage:W25Q128JV_Page_Program(0,buff,256);
    */
void W25Q128JV_Page_Program(uint32_t Addr,uint8_t *Data,uint16_t Num)
{
    W25Q128JV_Write_ENABLE();           //写使能
    W25Q128JV_WaitForWrite();           //等待写入完成
    W25Q128JV_CS_SET(0);                //片选
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_PAGE_PROGRAM);    	//发送页写入指令

	W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF0000) >> 16);		//发送地址 高8位
    W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF00) >> 8);        //发送地址 中8位
    W25Q128JV_SPI_SendReceiveByte(Addr & 0xFF);                 //发送地址 低8位
																
    while(Num--)
    {
        W25Q128JV_SPI_SendReceiveByte(*Data++);            //发送数据
    }
    W25Q128JV_CS_SET(1);                //取消片选
    W25Q128JV_WaitForWrite();           //等待写入完成
}

#else //使用DMA
/****
	* @brief	读取 W25Q128JV 数据
	* @param   	Addr    读取的地址      0x000000~0x7FFFFF
    * @param   	Data    存储读取的数据
    * @param   	Num     读取数据的长度  0~0x800000
	* @return   无
	* Sample usage:W25Q128JV_Read_Data(0,buff,4096);
    */
void W25Q128JV_Read_Data(uint32_t Addr,uint8_t *Data,uint32_t Num)
{
    //发送字节
    uint8_t DummyData[Num];     //定义一个数组用于存储Dummy数据
    memset(DummyData,0,Num);    //将DummyData数组全部置为0
	
    W25Q128JV_SPIx_DMA_CMD(DISABLE);
    W25Q128JV_CS_SET(0);
    //发送读 数据 指令
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_READ_DATA);

    //地址高位
    W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF0000) >> 16);
    W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF00) >> 8);
    //地址低位
    W25Q128JV_SPI_SendReceiveByte(Addr & 0xFF);
    
    W25Q128JV_DMA_CMD(DISABLE);    //关闭DMA

    //接收数据
    DMA_SetCurrDataCounter(W25Q128JV_DMAx_SPI_RX_Stream,Num);    //设置DMA传输数据长度
    W25Q128JV_DMAx_SPI_RX_Stream->M0AR = (uint32_t)Data;        //设置DMA传输数据地址

    //发送数据
    DMA_SetCurrDataCounter(W25Q128JV_DMAx_SPI_TX_Stream,Num);    //设置DMA传输数据长度
    W25Q128JV_DMAx_SPI_TX_Stream->M0AR = (uint32_t)DummyData;    //设置DMA传输数据地址
   
    W25Q128JV_SPIx_DMA_CMD(ENABLE);      //开启SPI DMA
    W25Q128JV_DMA_CMD(ENABLE);           //开启DMA

    //等待接收完成
    while(DMA_GetFlagStatus(W25Q128JV_DMAx_SPI_RX_Stream,DMA_FLAG_TCIF2) == RESET);    //等待DMA传输完成
    DMA_ClearFlag(W25Q128JV_DMAx_SPI_RX_Stream,DMA_FLAG_TCIF2);    //清除DMA传输完成标志位
    //等待发送完成
    while(DMA_GetFlagStatus(W25Q128JV_DMAx_SPI_TX_Stream,DMA_FLAG_TCIF5) == RESET);    //等待DMA传输完成
    DMA_ClearFlag(W25Q128JV_DMAx_SPI_TX_Stream,DMA_FLAG_TCIF5);    //清除DMA传输完成标志位
    
    W25Q128JV_CS_SET(1);            //取消片选
}

/****
	* @brief	W25Q128JV 页写入数据
	* @param   	Addr    读取的地址
    * @param   	Data    要写入的数据
    * @param   	Num     写入数据的长度	1-256
	* @return   无
	* Sample usage:W25Q128JV_Page_Program(0,buff,256);
    */
void W25Q128JV_Page_Program(uint32_t Addr,uint8_t *Data,uint16_t Num)
{
    uint8_t DummyData[Num];     //定义一个数组用于存储Dummy数据
    memset(DummyData,0,Num);    //将DummyData数组全部置为0

    W25Q128JV_Write_ENABLE();   //写使能

    W25Q128JV_SPIx_DMA_CMD(DISABLE);    //关闭SPI DMA

    W25Q128JV_WaitForWrite();           //等待写入完成

    W25Q128JV_CS_SET(0);

    //发送 页写入 指令
    W25Q128JV_SPI_SendReceiveByte(W25Q128JV_PAGE_PROGRAM);
	
	//发送块高位地址
    W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF0000) >> 16);
    //发送块中位地址
    W25Q128JV_SPI_SendReceiveByte((Addr & 0xFF00) >> 8);
    //发送块低位地址
    W25Q128JV_SPI_SendReceiveByte(Addr & 0xFF);
	
    W25Q128JV_DMA_CMD(DISABLE);    //关闭DMA

    //发送数据
    DMA_SetCurrDataCounter(W25Q128JV_DMAx_SPI_TX_Stream,Num);    //设置DMA传输数据长度
    W25Q128JV_DMAx_SPI_TX_Stream->M0AR = (uint32_t)Data;        //设置DMA传输数据地址

    //接收数据
    DMA_SetCurrDataCounter(W25Q128JV_DMAx_SPI_RX_Stream,Num);    //设置DMA传输数据长度
    W25Q128JV_DMAx_SPI_RX_Stream->M0AR = (uint32_t)DummyData;    //设置DMA传输数据地址

    W25Q128JV_SPIx_DMA_CMD(ENABLE);            //开启SPI DMA
    W25Q128JV_DMA_CMD(ENABLE);                  //开启DMA

    //等待发送完成
    while(DMA_GetFlagStatus(W25Q128JV_DMAx_SPI_TX_Stream,DMA_FLAG_TCIF5) == RESET);    //等待DMA传输完成
    DMA_ClearFlag(W25Q128JV_DMAx_SPI_TX_Stream,DMA_FLAG_TCIF5);    //清除DMA传输完成标志位

    //等待接收完成
    while(DMA_GetFlagStatus(W25Q128JV_DMAx_SPI_RX_Stream,DMA_FLAG_TCIF2) == RESET);    //等待DMA传输完成
    DMA_ClearFlag(W25Q128JV_DMAx_SPI_RX_Stream,DMA_FLAG_TCIF2);    //清除DMA传输完成标志位

	W25Q128JV_CS_SET(1);
    W25Q128JV_WaitForWrite();
}
#endif //W25Q128JV_USE_DMADIS

/****
	* @brief	W25Q128JV 写入数据
	* @param   	Addr    读取的地址
    * @param   	Data    要写入的数据
    * @param   	Num     写入数据的长度	
	* @return   无
	* Sample usage:W25Q128JV_Buff_Write(0,buff,256);
    */
void W25Q128JV_Buff_Write(uint32_t Addr,uint8_t *Data,uint32_t Num)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr_Temp = 0, count = 0, temp = 0;
    /*mod运算求余，若Addr是W25Q128JV_PageSize整数倍，运算结果Addr_Temp值为0*/
    Addr_Temp = Addr % W25Q128JV_PageSize;
	
	/*差count个数据值，刚好可以对齐到页地址*/
    count = W25Q128JV_PageSize - Addr_Temp;
	/*计算出要写多少整数页*/
    NumOfPage =  Num / W25Q128JV_PageSize;
	/*mod运算求余，计算出剩余不满一页的字节数*/
    NumOfSingle = Num % W25Q128JV_PageSize;
	
	/* Addr_Temp=0,则WriteAddr 刚好按页对齐 aligned  */
    if (Addr_Temp == 0)
    {
        /* Num < W25Q128JV_PageSize */
        if (NumOfPage == 0) 
        {
            W25Q128JV_Page_Program(Addr,Data,Num);
        }
        else /* Num > W25Q128JV_PageSize */
        { 
            /*先把整数页都写了*/
            while (NumOfPage--)
            {
                W25Q128JV_Page_Program(Addr,Data,W25Q128JV_PageSize);
                Addr += W25Q128JV_PageSize;
                Data += W25Q128JV_PageSize;
            }
            /*若有多余的不满一页的数据，把它写完*/
            W25Q128JV_Page_Program(Addr,Data,NumOfSingle);
        }
    }
    /* 若地址与 W25Q128JV_PageSize 不对齐  */
    else 
    {
        /* Num < W25Q128JV_PageSize */
        if (NumOfPage == 0)
        {
            /*当前页剩余的count个位置比NumOfSingle小，一页写不完*/
            if (NumOfSingle > count) 
            {
                temp = NumOfSingle - count;
                 /*先写满当前页*/
                W25Q128JV_Page_Program(Addr,Data,count);
                        
                Addr +=  count;
                Data += count;
                /*再写剩余的数据*/
                W25Q128JV_Page_Program(Addr,Data,temp);
            }
            else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
            {
                W25Q128JV_Page_Program(Addr,Data,Num);
            }
        }
        else /* Num > W25Q128JV_PageSize */
        {
            /*地址不对齐多出的count分开处理，不加入这个运算*/
            Num -= count;
            NumOfPage = Num / W25Q128JV_PageSize;
            NumOfSingle = Num % W25Q128JV_PageSize;
                    
            /* 先写完count个数据，为的是让下一次要写的地址对齐 */
            W25Q128JV_Page_Program(Addr,Data,count);
                    
            /* 接下来就重复地址对齐的情况 */
            Addr +=  count;
            Data += count;
                /*把整数页都写了*/
            while (NumOfPage--)
            {
                W25Q128JV_Page_Program(Addr,Data,W25Q128JV_PageSize);
                Addr += W25Q128JV_PageSize;
                Data += W25Q128JV_PageSize;
            }
            /*若有多余的不满一页的数据，把它写完*/
            if (NumOfSingle != 0)
            {
                W25Q128JV_Page_Program(Addr,Data,NumOfSingle);
            }
        }
    }
}

