#ifndef _LCD_MENU_H_
#define _LCD_MENU_H_

#include "stm32f4xx.h"                  // Device header
#include <stdlib.h>
#include "LCD.h"
#include "LCD_Init.h"
#include "USART.h"

#define MENU_MAX_ROW 5		// �˵������ʾ���� 

//����˵��й����������
#define TYPE_SUBMENU 101  //�����Ӳ˵��Ĳ˵���
#define TYPE_PARAM   102  //���������ִ�в������ã�

typedef struct MENU_Init
{
	uint8_t  Menu_Num;                  //��ǰ�˵�������
    char *Menu_Title;                   //��ǰ�˵�����
    char *Menu_Item;                    //��ǰ�˵���
    uint8_t Type;                       //��ǰ�˵�����
    void (*MENU_FUN)(const char *);;    //��ǰ�˵����ܺ���
    struct MENU_Init *Menu_Next;        //��һ���˵�
    struct MENU_Init *Menu_Prev;        //��һ���˵�
}MENU_Init;

void DispCrtMenu();//���Ƶ�ǰ�˵���
void Display(uint8_t value) ;

#endif
