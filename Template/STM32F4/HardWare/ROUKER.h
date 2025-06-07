#ifndef __ROCKER_H__
#define __ROCKER_H__

#include "stm32f4xx.h"                  // Device header
#include "USART.h"

//ADC 数目
#define ADC_CHANNELS 5

#define Y_ADC_MAX 4095
#define Y_ADC_MIN 0
#define Y_COORDINATE_MIN 0
#define Y_COORDINATE_MAX 100

//Rocker / Electricity  value
typedef struct 
{
    uint16_t ROCKER_RX_Value;
    uint16_t ROCKER_RY_Value;
    uint16_t ROCKER_LX_Value;
    uint16_t ROCKER_LY_Value;
    float Electricity;
}ROCKER_Value;

//Rocker  GPIO
#define ROCKER_ADCx_GPIO_CLK 					RCC_AHB1Periph_GPIOA
#define ROCKER_ADCx_GPIO_AHBxClockCMD			RCC_AHB1PeriphClockCmd
#define ROCKER_ADCx_GPIO_PORT 					GPIOA
#define ROCKER_ADCL1_Pin 						GPIO_Pin_0			//左右
#define ROCKER_ADCL2_Pin 						GPIO_Pin_1			//前后
#define ROCKER_ADCR1_Pin 						GPIO_Pin_2			//前后
#define ROCKER_ADCR2_Pin 						GPIO_Pin_3			//左右

//Electircity GPIO
#define Electricity_ADCx_GPIO_CLK 				RCC_AHB1Periph_GPIOB
#define Electricity_ADCx_GPIO_AHBxlockCMD		RCC_AHB1PeriphClockCmd
#define Electricity_ADCx_GPIO_PORT 				GPIOB
#define Electricity_ADC_Pin 					GPIO_Pin_0

//ADC
#define EQUIPMENT_ADCx							ADC1
#define ROCKER_ADCx_CLK							RCC_APB2Periph_ADC1
#define ROCKER_ADCx_APBxClockCMD				RCC_APB2PeriphClockCmd
#define ROCKER_ADCx_Channel1					ADC_Channel_1
#define ROCKER_ADCx_Channel2                    ADC_Channel_0
#define ROCKER_ADCx_Channel3                    ADC_Channel_2
#define ROCKER_ADCx_Channel4                    ADC_Channel_3
#define Electricity_ADCx_Channel5               ADC_Channel_8

//DMA
#define PERIPHERAL_DMAx_CLK 					RCC_AHB1Periph_DMA2
#define PERIPHERAL_DMAx_AHBxClockCMD			RCC_AHB1PeriphClockCmd
#define PERIPHERAL_DMAx_Channel               	DMA_Channel_0
#define PERIPHERAL_DMAX_Stream                	DMA2_Stream4
void EQUIPMENT_Init();
void ROCKER_Calculate(ROCKER_Value *Value);

#endif


