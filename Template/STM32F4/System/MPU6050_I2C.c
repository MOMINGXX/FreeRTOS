#include "MPU6050_I2C.h"

/****
	* @brief	MPU6050_I2C SCL 引脚写入位数据 0 / 1 			  
	* @param   	BitValue    0 / 1
	* @return   无  	
	* Sample usage:MPU6050_SCL_Write(1);
    */
static void MPU6050_SCL_Write(uint8_t BitValue)
{
    GPIO_WriteBit(MPU6050_SCL_GPIO_PORT, MPU6050_SCL_GPIO_PIN, (BitAction)BitValue);
    Delay_us(10);
}

/****
	* @brief	MPU6050_I2C SDA 引脚写入位数据 0 / 1 			  
	* @param   	BitValue    0 / 1
	* @return   无  	
	* Sample usage:MPU6050_SDA_Write(1);
    */
static void MPU6050_SDA_Write(uint8_t BitValue)
{
    GPIO_WriteBit(MPU6050_SDA_GPIO_PORT, MPU6050_SDA_GPIO_PIN, (BitAction)BitValue);
    Delay_us(10);
}

/****
	* @brief	MPU6050_I2C SDA 读取引脚状态 			  
	* @param   	无
	* @return   BitValue    读取的状态  	
	* Sample usage:MPU6050_SDA_Read();
    */
static uint8_t MPU6050_SDA_Read(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(MPU6050_SDA_GPIO_PORT, MPU6050_SDA_GPIO_PIN);
    Delay_us(10);
    return BitValue;
}

/****
	* @brief	MPU6050_I2C 引脚配置   			  
	* @param   	无
	* @return   无  	
	* Sample usage:MPU6050_I2C_Config();
    */
void MPU6050_I2C_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    MPU6050_I2C_GPIO_APBxCLKCMD(MPU6050_I2C_SCL_GPIO_CLK, ENABLE);  //  使能I2C时钟
    MPU6050_I2C_GPIO_APBxCLKCMD(MPU6050_I2C_SDA_GPIO_CLK, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = MPU6050_SCL_GPIO_PIN | MPU6050_SDA_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       // 输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // 推挽输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        // 上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_Init(MPU6050_SCL_GPIO_PORT, &GPIO_InitStructure);
    GPIO_Init(MPU6050_SDA_GPIO_PORT, &GPIO_InitStructure);

    MPU6050_SCL_Write(1);
    MPU6050_SDA_Write(1);
}

/****
	* @brief	MPU6050_I2C  开始信号			  
	* @param   	无
	* @return   无  	
	* Sample usage:MPU6050_I2C_Start();
    */
void MPU6050_I2C_Start(void)
{
    MPU6050_SDA_Write(1);
    MPU6050_SCL_Write(1);          
    MPU6050_SDA_Write(0);   // 产生一个时钟
    MPU6050_SCL_Write(0);   // 释放总线
}

/****
	* @brief	MPU6050_I2C  停止信号			  
	* @param   	无
	* @return   无  	
	* Sample usage:MPU6050_I2C_Stop();
    */
void MPU6050_I2C_Stop(void)
{
    MPU6050_SCL_Write(0);
    MPU6050_SDA_Write(0);   
    MPU6050_SCL_Write(1);    // SCL 高电平时 SDA 进行跳变    
    MPU6050_SDA_Write(1);   
}

/****
	* @brief	MPU6050_I2C 发送数据    			  
	* @param   	Byte    发送的数据 
	* @return   无  	
	* Sample usage:MPU6050_I2C_SendByte(0x01);
    */
void MPU6050_I2C_SendByte(uint8_t Byte)
{
    uint8_t i = 0 ;
    for(i = 0; i < 8; i++)
    {
        MPU6050_SDA_Write(Byte & (0x80 >> i));
        MPU6050_SCL_Write(1);
        MPU6050_SCL_Write(0);
    }
}

/****
	* @brief	MPU6050_I2C 接收数据    			  
	* @param   	无
	* @return   Byte    接收的数据  	
	* Sample usage:MPU6050_I2C_ReceiveByte();
    */
uint8_t MPU6050_I2C_ReceiveByte()
{
    uint8_t i = 0 ;
    uint8_t Byte = 0x00;
    for(i = 0; i < 8; i++)
    {
        MPU6050_SCL_Write(1);
        if(MPU6050_SDA_Read())
        {
            Byte |= (0x80 >> i);
        }
        MPU6050_SCL_Write(0);
    }
    return Byte;
}

/****
	* @brief	MPU6050_I2C 等待应答   			  
	* @param   	无
	* @return   1 - 无应答  
                0 - 应答	
	* Sample usage:MPU6050_I2C_WaitAck();
    */
uint8_t  MPU6050_I2C_WaitAck()
{
    uint8_t ucErrorTime = 0;    //超时时间
    MPU6050_SDA_Write(1);
    MPU6050_SCL_Write(1);
    while(MPU6050_SDA_Read())
    {
        ucErrorTime++;
        if(ucErrorTime > 250)
        {
            MPU6050_I2C_Stop();
            return 1;
        }   
    }
    MPU6050_SCL_Write(0);
    return 0;
}

/****
	* @brief	MPU6050_I2C 发送应答   			  
	* @param   	Ack     发送的应答   0 / 1
	* @return   无 	
	* Sample usage:MPU6050_I2C_SendAck(1);
    */
void MPU6050_I2C_SendAck(uint8_t Ack)
{
    MPU6050_SCL_Write(0);
    MPU6050_SDA_Write(Ack);
    MPU6050_SCL_Write(1);
    MPU6050_SCL_Write(0);
    
}

/****
	* @brief	MPU6050_I2C 接收应答   			  
	* @param   	无
	* @return   Ack 	接收的应答  0 / 1
	* Sample usage:MPU6050_I2C_SendAck(1);
    */
uint8_t MPU6050_I2C_ReceiveAck()
{
    uint8_t Ack = 0x00;
    MPU6050_SDA_Write(1);
    MPU6050_SCL_Write(1);
    Ack = MPU6050_SDA_Read();
    MPU6050_SCL_Write(0);
    return Ack;
}
/****End of file****/
