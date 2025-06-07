#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "Delay.h"
#include "LCD_SPI.h"

#define USE_HORIZONTAL 1  //设置横屏或者竖屏显示,0为竖屏,1为倒竖屏,2为横屏,3为倒横屏

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


/*************** LCD端口定义 **************/ 
#define LCD_RES_Clr()  GPIO_ResetBits(LCD_GPIO_PORT,LCD_RES_GPIO_PIN)//RES，复位，低电平有效
#define LCD_RES_Set()  GPIO_SetBits(LCD_GPIO_PORT,LCD_RES_GPIO_PIN)

#define LCD_DC_Clr()   GPIO_ResetBits(LCD_DC_GPIO_PORT,LCD_DC_GPIO_PIN)//DC，数据/命令选择引脚,低电平写命令,高电平写数据
#define LCD_DC_Set()   GPIO_SetBits(LCD_DC_GPIO_PORT,LCD_DC_GPIO_PIN)
 		     
#define LCD_CS_Clr()   GPIO_ResetBits(LCD_GPIO_PORT,LCD_CS_GPIO_PIN)//CS，片选信号，低电平有效
#define LCD_CS_Set()   GPIO_SetBits(LCD_GPIO_PORT,LCD_CS_GPIO_PIN)

#define LCD_BLK_Clr()  GPIO_ResetBits(LCD_GPIO_PORT,LCD_BLK_GPIO_PIN)//BLK，背光控制开关，默认打开背光，低电平关闭
#define LCD_BLK_Set()  GPIO_SetBits(LCD_GPIO_PORT,LCD_BLK_GPIO_PIN)


void LCD_GPIO_Init(void);  //初始化GPIO
void LCD_Writ_Bus(uint8_t dat); //模拟SPI时序
void LCD_WR_DATA8(uint8_t dat); //写入一个字节
void LCD_WR_DATA(uint16_t dat); //写入两个字节
void LCD_WR_REG(uint8_t dat);   //写入一个指令
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2); //设置坐标函数
void LCD_Init(void);       //LCD初始化

#endif

