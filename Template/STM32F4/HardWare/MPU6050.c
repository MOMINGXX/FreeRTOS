#include "MPU6050.h"

/****
	* @brief    ���ᴫ���� д����			  
	* @param   	RegAddress  �Ĵ�����ַ
    * @param   	Data        д�������
	* @return   ��    	
	* Sample usage:MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x01);
    */
void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data)
{
    MPU6050_I2C_Start();                    //I2C��ʼ�ź�
    MPU6050_I2C_SendByte(MPU6050_ADDR);     //���� MPU6050 �豸��ַ
    MPU6050_I2C_ReceiveAck();               //�ȴ�ACK
    MPU6050_I2C_SendByte(RegAddress);       //����Ҫд��ļĴ�����ַ
    MPU6050_I2C_ReceiveAck();               //�ȴ�ACK
    MPU6050_I2C_SendByte(Data);             //����Ҫд�������
    MPU6050_I2C_ReceiveAck();               //�ȴ�ACK
    MPU6050_I2C_Stop();                     //I2Cֹͣ�ź�
}

/****
	* @brief    ���ᴫ���� ��ȡ����		  
	* @param   	RegAddress  �Ĵ�����ַ
	* @return   Data        ��ȡ������   	
	* Sample usage:MPU6050_ReadReg(MPU6050_PWR_MGMT_1);
    */
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data = 0;                           //����һ���������ڴ洢��ȡ������
    MPU6050_I2C_Start();                        //I2C��ʼ�ź�
    MPU6050_I2C_SendByte(MPU6050_ADDR);         //���� MPU6050 �豸��ַ
    MPU6050_I2C_ReceiveAck();            //�ȴ�ACK           //����Ҫ��ȡ�ļĴ�����ַ
    MPU6050_I2C_SendByte(RegAddress);           //����Ҫ��ȡ�ļĴ�����ַ
    MPU6050_I2C_ReceiveAck();                   //�ȴ�ACK

    MPU6050_I2C_Start();                        //I2C��ʼ�ź�
    MPU6050_I2C_SendByte(MPU6050_ADDR|0x01);    //���� MPU6050 �豸��ַ+1 ��
    MPU6050_I2C_ReceiveAck();                   //�ȴ�ACK
    Data = MPU6050_I2C_ReceiveByte();           //��ȡ����
    MPU6050_I2C_SendAck(1);                     //�ȴ�ACK
    MPU6050_I2C_Stop();                         //I2Cֹͣ�ź�
    return Data;                                //���ض�ȡ������
}

/****
	* @brief    ���ᴫ���� ��ʼ��		  
	* @param   	��
	* @return   ��	
	* Sample usage:MPU6050_Init();
    */
void MPU6050_Init(void)
{
    MPU6050_I2C_Config();
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x80);		//��λ
	Delay_ms(100);
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x01);		//��Դ����Ĵ���1  �������״̬ ѡ��X��������ʱ��
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0x00);		//��Դ����Ĵ���2  6���������
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0x07);		//�����ʷ�Ƶ       ������125hz
	MPU6050_WriteReg(MPU6050_CONFIG,0x06);			//��ͨ�˲�Ƶ��
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0x18);		//������        2000��/��  16.4
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0x18);	//���ٶȼ�     16g  2048
}

/****
	* @brief    ���ᴫ���� ID		  
	* @param   	��
	* @return   ID��          0X68
	* Sample usage:MPU6050_GetID();
    */
uint8_t MPU6050_GetID()
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

/****
	* @brief    ���ᴫ���� ��ȡ����		  
	* @param   	Data    �ṹ���ַ
	* @return   ��
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
		sumcheck += buf[i]; //��֡ͷ��ʼ����ÿһ�ֽڽ�����ͣ�ֱ��DATA������
		addcheck += sumcheck; //ÿһ�ֽڵ���Ͳ���������һ��sumcheck���ۼ�
	}
	buf[11]=sumcheck;
	buf[12]=addcheck;
	
	Usart_SendByteBuff(USART1,buf,13);
	
}

//MPU6050�����˲�
void MPU6050_ComplementaryFilter(MPU6050_Data *Data,MPU6050_Calculate *Calculate)
{
    float Accel_X,Accel_Y,Accel_Z;
    float Gyro_X,Gyro_Y,Gyro_Z;
    float AngleX,AngleY,AngleZ;
    MPU6050_GetData(Data);//��ȡ����
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
