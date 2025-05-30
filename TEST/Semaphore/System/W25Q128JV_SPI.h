#ifndef _W25Q128JV_SPI_H_
#define _W25Q128JV_SPI_H_

#include "stm32f4xx.h"                  // Device header

//W25Q128JV GPIO	
#define W25Q128JV_GPIO_CLK             	RCC_AHB1Periph_GPIOB
#define W25Q128JV_GPIO_APBxCLKCMD      	RCC_AHB1PeriphClockCmd
	
#define W25Q128JV_SPI_GPIO_PORT         GPIOB	
#define W25Q128JV_CS_GPIO_PORT         	GPIOB
#define W25Q128JV_CS_GPIO_PIN          	GPIO_Pin_6		
#define W25Q128JV_SCK_GPIO_PIN         	GPIO_Pin_3
#define W25Q128JV_SCK_PINSOURCE         GPIO_PinSource3
#define W25Q128JV_MISO_GPIO_PIN        	GPIO_Pin_4
#define W25Q128JV_MISO_PINSOURCE        GPIO_PinSource4
#define W25Q128JV_MOSI_GPIO_PIN        	GPIO_Pin_5
#define W25Q128JV_MOSI_PINSOURCE        GPIO_PinSource5

//W25Q128JV SPI
#define W25Q128JV_SPIx					SPI3
#define W25Q128JV_SPIx_CLK				RCC_APB1Periph_SPI3
#define W25Q128JV_SPIx_APBxClockCMD		RCC_APB1PeriphClockCmd

//DMA
#define W25Q128JV_DMAx                  DMA1
#define W25Q128JV_DMAx_CLK              RCC_AHB1Periph_DMA1
#define W25Q128JV_DMAxAHBxClockCMD      RCC_AHB1PeriphClockCmd
#define W25Q128JV_DMAxChannel           DMA_Channel_0
#define W25Q128JV_DMAx_SPI_RX_Stream    DMA1_Stream2
#define W25Q128JV_DMAx_SPI_TX_Stream    DMA1_Stream5
	
#define W25Q128JV_CS_SET(x)				GPIO_WriteBit(W25Q128JV_CS_GPIO_PORT,W25Q128JV_CS_GPIO_PIN,(BitAction)x)
//#define W25Q128JV_SCK_SET(x)			GPIO_WriteBit(W25Q128JV_SPI_GPIO_PORT,W25Q128JV_SCK_GPIO_PIN,(BitAction)x)
//#define W25Q128JV_MISO_SET(x)			GPIO_WriteBit(W25Q128JV_SPI_GPIO_PORT,W25Q128JV_MISO_GPIO_PIN,(BitAction)x)
//#define W25Q128JV_MOSI_SET(x)			GPIO_WriteBit(W25Q128JV_SPI_GPIO_PORT,W25Q128JV_MOSI_GPIO_PIN,(BitAction)x)

void W25Q128JV_Config_Init();
void W25Q128JV_SPIx_DMA_CMD(FunctionalState NewState);
void W25Q128JV_DMA_CMD(FunctionalState NewState);
uint8_t W25Q128JV_SPI_SendReceiveByte(uint8_t SendByte);

#endif
