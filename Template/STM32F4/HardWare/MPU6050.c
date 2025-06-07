#include "MPU6050.h"

/****
	* @brief    六轴传感器 写数据			  
	* @param   	RegAddress  寄存器地址
    * @param   	Data        写入的数据
	* @return   无    	
	* Sample usage:MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x01);
    */
void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data)
{
    MPU6050_I2C_Start();                    //I2C起始信号
    MPU6050_I2C_SendByte(MPU6050_ADDR);     //发送 MPU6050 设备地址
    MPU6050_I2C_ReceiveAck();               //等待ACK
    MPU6050_I2C_SendByte(RegAddress);       //发送要写入的寄存器地址
    MPU6050_I2C_ReceiveAck();               //等待ACK
    MPU6050_I2C_SendByte(Data);             //发送要写入的数据
    MPU6050_I2C_ReceiveAck();               //等待ACK
    MPU6050_I2C_Stop();                     //I2C停止信号
}

/****
	* @brief    六轴传感器 读取数据		  
	* @param   	RegAddress  寄存器地址
	* @return   Data        读取的数据   	
	* Sample usage:MPU6050_ReadReg(MPU6050_PWR_MGMT_1);
    */
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data = 0;                           //定义一个变量用于存储读取的数据
    MPU6050_I2C_Start();                        //I2C起始信号
    MPU6050_I2C_SendByte(MPU6050_ADDR);         //发送 MPU6050 设备地址
    MPU6050_I2C_ReceiveAck();            //等待ACK           //发送要读取的寄存器地址
    MPU6050_I2C_SendByte(RegAddress);           //发送要读取的寄存器地址
    MPU6050_I2C_ReceiveAck();                   //等待ACK

    MPU6050_I2C_Start();                        //I2C起始信号
    MPU6050_I2C_SendByte(MPU6050_ADDR|0x01);    //发送 MPU6050 设备地址+1 读
    MPU6050_I2C_ReceiveAck();                   //等待ACK
    Data = MPU6050_I2C_ReceiveByte();           //读取数据
    MPU6050_I2C_SendAck(1);                     //等待ACK
    MPU6050_I2C_Stop();                         //I2C停止信号
    return Data;                                //返回读取的数据
}

/****
	* @brief    六轴传感器 初始化		  
	* @param   	无
	* @return   无	
	* Sample usage:MPU6050_Init();
    */
void MPU6050_Init(void)
{
    MPU6050_I2C_Config();
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x80);		//复位
	Delay_ms(100);
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x01);		//电源管理寄存器1  解除休眠状态 选用X轴陀螺仪时钟
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0x00);		//电源管理寄存器2  6轴均不待机
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0x07);		//采样率分频       陀螺仪125hz
	MPU6050_WriteReg(MPU6050_CONFIG,0x06);			//低通滤波频率
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0x18);		//陀螺仪        2000度/秒  16.4
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0x18);	//加速度计     16g  2048
}

/****
	* @brief    六轴传感器 ID		  
	* @param   	无
	* @return   ID号          0X68
	* Sample usage:MPU6050_GetID();
    */
uint8_t MPU6050_GetID()
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

/****
	* @brief    六轴传感器 读取数据		  
	* @param   	Data    结构体地址
	* @return   无
	* Sample usage:MPU6050_GetData(&Data);
    */
void MPU6050_GetData(MPU6050_Data *Data)
{
    int16_t Temp;
    Data->ACCEL_XOUT = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H)<<8|MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
    Data->ACCEL_YOUT = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H)<<8|MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    Data->ACCEL_ZOUT = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H)<<8|MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);

    Data->GYRO_XOUT = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H)<<8|MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
    Data->GYRO_YOUT = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H)<<8|MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
    Data->GYRO_ZOUT = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H)<<8|MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);

    Temp = MPU6050_ReadReg(MPU6050_TEMP_OUT_H)<<8|MPU6050_ReadReg(MPU6050_TEMP_OUT_L);
    Data->Temperature = (float)((Temp-65536)/340+36.53);
}

void SEND_OULA_ANGLE(int row,int pit,int yaw)
{
	u8 i;
	u8 sumcheck = 0;
	u8 addcheck = 0;
	u8 buf[13]={0};
	
	buf[0]=0xAA;
	buf[1]=0xFF;
	buf[2]=0x03;
	buf[3]=0x07;
	
	buf[4]=(u8)row;
	buf[5]=(u8)(row>>8);
	
	buf[6]=(u8)pit;
	buf[7]=(u8)(pit>>8);

	buf[8]=(u8)yaw;
	buf[9]=(u8)(yaw>>8);
	
	buf[10]=0x00;
	
	for(i=0; i < (buf[3]+4); i++)
	{
		sumcheck += buf[i]; //从帧头开始，对每一字节进行求和，直到DATA区结束
		addcheck += sumcheck; //每一字节的求和操作，进行一次sumcheck的累加
	}
	buf[11]=sumcheck;
	buf[12]=addcheck;
	
	Usart_SendByteBuff(USART1,buf,13);
	
}

//MPU6050互补滤波
void MPU6050_ComplementaryFilter(MPU6050_Data *Data,MPU6050_Calculate *Calculate)
{
    float Accel_X,Accel_Y,Accel_Z;
    float Gyro_X,Gyro_Y,Gyro_Z;
    float AngleX,AngleY,AngleZ;
    MPU6050_GetData(Data);//读取数据
    Accel_X = Data->ACCEL_XOUT/16384.0;
    Accel_Y = Data->ACCEL_YOUT/16384.0;
    Accel_Z = Data->ACCEL_ZOUT/16384.0;
    Gyro_X = Data->GYRO_XOUT/16.4;

    Gyro_Y = Data->GYRO_YOUT/16.4;
    Gyro_Z = Data->GYRO_ZOUT/16.4;
    AngleX = atan2(Accel_Y,Accel_Z)*180/3.1415926;
    AngleY = atan2(-Accel_X,sqrt(Accel_Y*Accel_Y+Accel_Z*Accel_Z))*180/3.1415926;
    AngleZ = 0;
    Calculate->Pitch = 0.98*(Calculate->Pitch+Gyro_X*0.01)+0.02*AngleX;
    Calculate->Roll = 0.98*(Calculate->Roll+Gyro_Y*0.01)+0.02*AngleY;
    Calculate->Yaw = 0.98*(Calculate->Yaw+Gyro_Z*0.01)+0.02*AngleZ;
	
	//test
	SEND_OULA_ANGLE((int)Calculate->Roll,(int)Calculate->Pitch,(int)Calculate->Yaw);
}
