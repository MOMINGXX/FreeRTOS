#ifndef _MPU6050_I2C_H_
#define _MPU6050_I2C_H_ 			   

#include "stm32f4xx.h"                  // Device header
#include "Delay.h"

//MPU6050 IIC GPIO
#define MPU6050_I2C_SCL_GPIO_CLK        RCC_AHB1Periph_GPIOC
#define MPU6050_I2C_SDA_GPIO_CLK        RCC_AHB1Periph_GPIOD
#define MPU6050_I2C_GPIO_APBxCLKCMD     RCC_AHB1PeriphClockCmd

#define MPU6050_SCL_GPIO_PORT           GPIOC
#define MPU6050_SDA_GPIO_PORT           GPIOD
#define MPU6050_SCL_GPIO_PIN            GPIO_Pin_12
#define MPU6050_SDA_GPIO_PIN            GPIO_Pin_2

void MPU6050_I2C_Config(void);
void MPU6050_I2C_Start();
void MPU6050_I2C_Stop();
void MPU6050_I2C_SendByte(uint8_t Byte);
uint8_t MPU6050_I2C_ReceiveByte();
uint8_t MPU6050_I2C_ReceiveAck();
uint8_t MPU6050_I2C_WaitAck();
void MPU6050_I2C_SendAck(uint8_t Ack);

#endif

