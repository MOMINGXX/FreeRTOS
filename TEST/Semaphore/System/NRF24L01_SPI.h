#ifndef _NRF24L01_SPI_H_
#define _NRF24L01_SPI_H_

#include "stm32f4xx.h"                  // Device header

//GPIO
#define NRF24L01_GPIOx_CLK				RCC_AHB1Periph_GPIOA
#define NRF24L01_GPIO_AHBxClockCMD		RCC_AHB1PeriphClockCmd
#define NRF24L01_GPIOx_PORT				GPIOA
#define NRF24L01_SCK_GPIO_PIN           GPIO_Pin_5
#define NRF24L01_SCK_PINSOURCE          GPIO_PinSource5
#define NRF24L01_MISO_GPIO_PIN          GPIO_Pin_6
#define NRF24L01_MISO_PINSOURCE         GPIO_PinSource6
#define NRF24L01_MOSI_GPIO_PIN          GPIO_Pin_7
#define NRF24L01_MOSI_PINSOURCE         GPIO_PinSource7

//SPI 
#define NRF24L01_SPIx					SPI1
#define NRF24L01_SPIx_CLK				RCC_APB2Periph_SPI1
#define NRF24L01_SPIx_APBxClockCMD		RCC_APB2PeriphClockCmd

//DMA
#define NRF24L01_DMAx                   DMA2
#define NRF24L01_DMAx_CLK               RCC_AHB1Periph_DMA2
#define NRF24L01_DMAxAHBxClockCMD       RCC_AHB1PeriphClockCmd
#define NRF24L01_DMAxChannel            DMA_Channel_3
#define NRF24L01_DMAx_SPI_RX_Stream     DMA2_Stream2
#define NRF24L01_DMAx_SPI_TX_Stream     DMA2_Stream3

void NRF24L01_SPI_Init();
uint8_t NRF24L01_Send_RecieveByte(uint8_t SendByte);

#endif
