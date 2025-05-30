#ifndef _LCD_SPI_H_
#define _LCD_SPI_H_

#include "stm32f4xx.h"                  // Device header

//GPIO
#define LCD_GPIO_CLK				RCC_AHB1Periph_GPIOB
#define LCD_GPIO_AHBxClockCMD		RCC_AHB1PeriphClockCmd
#define LCD_GPIO_PORT				GPIOB
#define LCD_SCL_GPIO_PIN			GPIO_Pin_13
#define LCD_SCL_GPIO_PINSOURCE		GPIO_PinSource13
#define LCD_SPI_SCLK_AF			    GPIO_AF_SPI2
#define LCD_SDA_GPIO_PIN			GPIO_Pin_15
#define LCD_SDA_GPIO_PINSOURCE		GPIO_PinSource15
#define LCD_SPI_MOSI_AF			    GPIO_AF_SPI2

//SPI
#define LCD_SPIx					SPI2
#define LCD_SPIx_CLK				RCC_APB1Periph_SPI2
#define LCD_SPIx_APBxClockCMD		RCC_APB1PeriphClockCmd

//DMA
#define LCD_DMAx                    DMA1
#define LCD_DMAx_CLK                RCC_AHB1Periph_DMA1
#define LCD_DMAxAHBxClockCMD        RCC_AHB1PeriphClockCmd
#define LCD_DMAxChannel             DMA_Channel_0
#define LCD_DMAxStream              DMA1_Stream4

void LCD_SPIx_Init();
void LCD_DMA_Enable(void);
void LCD_SPI_WriteByte(uint8_t Byte);
void LCD_DMA_Config(uint32_t cpar,uint32_t cmar,uint16_t cndtr);
void LCD_DMA_Config1(uint32_t cpar,uint32_t cmar,uint16_t cndtr);

#endif
