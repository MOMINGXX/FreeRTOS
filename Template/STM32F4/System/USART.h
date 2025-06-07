#ifndef _USART_H_
#define _USART_H_ 			   

#include "stm32f4xx.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

#define RXHEX_MAX 4
#define RXCHA_MAX 100

//#define RX_HEX

extern uint8_t Usart_RxHEX[];
extern char Usart_RxCharacter[];
extern uint8_t Usart_RxFlag;

//串口
#define SPECIFIE_USARTx                     USART1
#define SPECIFIE_USART_CLK                  RCC_APB2Periph_USART1
#define SPECIFIE_USART_APBxCLKCMD           RCC_APB2PeriphClockCmd
#define SPECIFIE_USART_BAUDRATE             115200
#define SPECIFIE_USART_RX_AF                GPIO_AF_USART1
#define SPECIFIE_USART_RX_PinSource         GPIO_PinSource10
#define SPECIFIE_USART_TX_AF                GPIO_AF_USART1
#define SPECIFIE_USART_TX_PinSource         GPIO_PinSource9

#define SPECIFIE_USART_PreemptionPriority   5
#define SPECIFIE_USART_SubPriority          0

//串口引脚
#define SPECIFIE_USART_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define SPECIFIE_USART_GPIO_APBxCLKCMD      RCC_AHB1PeriphClockCmd

#define SPECIFIE_USART_GPIO_PORT            GPIOA
#define SPECIFIE_USART_TX_GPIO_PIN          GPIO_Pin_9
#define SPECIFIE_USART_RX_GPIO_PIN          GPIO_Pin_10

//中断
#define SPECIFIE_USART_IRQChannel           USART1_IRQn
#define SPECIFIE_USART_IRQHNADLER           USART1_IRQHandler

void Usart_Init();
void Usart_sprintf(char *format, ...);
void Usart_SendByte(USART_TypeDef *USARTx,uint8_t Byte);
void Usart_SendHalfWorld(USART_TypeDef *USARTx,uint16_t Byte);
void Usart_SendString(USART_TypeDef *USARTx,char *String);
void Usart_SendByteBuff(USART_TypeDef *USARTx ,uint8_t *Buff, uint16_t Length);
void Usart_SendSpeNumber(USART_TypeDef *USARTx,uint32_t Number,uint8_t Length);

#endif

