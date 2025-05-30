#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "Delay.h"
#include "LCD_SPI.h"

#define USE_HORIZONTAL 1  //���ú�������������ʾ,0Ϊ����,1Ϊ������,2Ϊ����,3Ϊ������

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 320

#else
#define LCD_W 320
#define LCD_H 240
#endif

//GPIO
#define LCD_GPIO_CLK				RCC_AHB1Periph_GPIOB
#define LCD_DC_GPIO_CLK				RCC_AHB1Periph_GPIOC
#define LCD_GPIO_AHBxClockCMD		RCC_AHB1PeriphClockCmd
#define LCD_GPIO_PORT				GPIOB
#define LCD_DC_GPIO_PORT			GPIOC
#define LCD_RES_GPIO_PIN			GPIO_Pin_14
#define LCD_DC_GPIO_PIN				GPIO_Pin_6
#define LCD_CS_GPIO_PIN				GPIO_Pin_12
#define LCD_BLK_GPIO_PIN			GPIO_Pin_10


/*************** LCD�˿ڶ��� **************/ 
#define LCD_RES_Clr()  GPIO_ResetBits(LCD_GPIO_PORT,LCD_RES_GPIO_PIN)//RES����λ���͵�ƽ��Ч
#define LCD_RES_Set()  GPIO_SetBits(LCD_GPIO_PORT,LCD_RES_GPIO_PIN)

#define LCD_DC_Clr()   GPIO_ResetBits(LCD_DC_GPIO_PORT,LCD_DC_GPIO_PIN)//DC������/����ѡ������,�͵�ƽд����,�ߵ�ƽд����
#define LCD_DC_Set()   GPIO_SetBits(LCD_DC_GPIO_PORT,LCD_DC_GPIO_PIN)
 		     
#define LCD_CS_Clr()   GPIO_ResetBits(LCD_GPIO_PORT,LCD_CS_GPIO_PIN)//CS��Ƭѡ�źţ��͵�ƽ��Ч
#define LCD_CS_Set()   GPIO_SetBits(LCD_GPIO_PORT,LCD_CS_GPIO_PIN)

#define LCD_BLK_Clr()  GPIO_ResetBits(LCD_GPIO_PORT,LCD_BLK_GPIO_PIN)//BLK��������ƿ��أ�Ĭ�ϴ򿪱��⣬�͵�ƽ�ر�
#define LCD_BLK_Set()  GPIO_SetBits(LCD_GPIO_PORT,LCD_BLK_GPIO_PIN)


void LCD_GPIO_Init(void);  //��ʼ��GPIO
void LCD_Writ_Bus(uint8_t dat); //ģ��SPIʱ��
void LCD_WR_DATA8(uint8_t dat); //д��һ���ֽ�
void LCD_WR_DATA(uint16_t dat); //д�������ֽ�
void LCD_WR_REG(uint8_t dat);   //д��һ��ָ��
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2); //�������꺯��
void LCD_Init(void);       //LCD��ʼ��

#endif

