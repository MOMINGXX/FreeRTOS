#ifndef _W25Q128JV_H_
#define _W25Q128JV_H_

#include "stm32f4xx.h"                  // Device header
#include "W25Q128JV_SPI.h"
#include "string.h"

#define W25Q128JV_PageSize                  256
#define W25Q128JV_PerWritePageSize          256

//#define W25Q128JV_USE_DMAEN
#define W25Q128JV_USE_DMADIS

//W25Q128JV  命令
#define DUMMY                            	0x00
#define W25Q128JV_JEDEC_ID                  0x9F
#define W25Q128JV_WRITE_ENABLE              0x06
#define W25Q128JV_WRITE_DISABLE             0x04
#define W25Q128JV_READ_STATUS_REGISTER      0x05
#define W25Q128JV_READ_STATUS_REGISTER1     0x35
#define W25Q128JV_WRITE_STATUS_REGISTER     0x01
#define W25Q128JV_READ_DATA                 0x03
#define W25Q128JV_FAST_READ                 0x0B
#define W25Q128JV_PAGE_PROGRAM              0x02
#define W25Q128JV_BLOCK_ERASE1			    0x52       //32KB
#define W25Q128JV_BLOCK_ERASE2			    0xD8       //64KB
#define W25Q128JV_SECTOR_ERASE              0x20
#define W25Q128JV_CHIP_ERASE		        0xC7 
#define W25Q128JV_ManufactDeviceID   	    0x90
#define W25Q128JV_POWER_DOWN                0xB9
#define W25Q128JV_RELEASE_POWER_DOWN        0xAB
#define W25Q128JV_DEVICE_ID                 0xAB
#define W25Q128JV_ENABLE_RESET              0x66
#define W25Q128JV_RESET                     0x99

/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT                	((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT                	((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

//#define W25QX_ERROR(fmt,arg...)          printf("W25QX_ERROR:>"fmt"\n",##arg)
#define W25QX_ERROR(n)         			 	printf("W25QX_ERRO: 等待超时! ERROR_CODE:> "#n"\n")
		
#define W25Q128JV_SPI_CS(x)        		  	GPIO_WriteBit(W25Q128JV_SPI_CS_GPIO_PORT ,W25Q128JV_SPI_CS_GPIO_PIN,(BitAction)(x))
//#define W25Q128JV_SPI_CSLOW				  GPIO_ResetBits(W25Q128JV_SPI_CS_GPIO_PORT ,W25Q128JV_SPI_CS_GPIO_PIN )
//#define W25Q128JV_SPI_CSHIGH				GPIO_SetBits(W25Q128JV_SPI_CS_GPIO_PORT ,W25Q128JV_SPI_CS_GPIO_PIN)

void W25Q128JV_Init();
uint32_t W25Q128JV_Read_JEDEC_ID();
uint8_t W25Q128JV_Read_Device_ID();

void W25Q128JV_Chip_Erase();
void W25Q128JV_Sector_Erase(uint32_t Addr);
void W25Q128JV_Block_Erase(uint32_t Addr);

void W25Q128JV_Read_Data(uint32_t Addr,uint8_t *Data,uint32_t Num);
void W25Q128JV_Page_Program(uint32_t Addr,uint8_t *Data,uint16_t Num);
void W25Q128JV_Buff_Write(uint32_t Addr,uint8_t *Data,uint32_t Num);

void W25Q128JV_Power_Down();
void W25Q128JV_WakeUP();

#endif