#ifndef _LCD_MENU_H_
#define _LCD_MENU_H_

#include "stm32f4xx.h"                  // Device header
#include <stdlib.h>
#include "LCD.h"
#include "LCD_Init.h"
#include "USART.h"

#define MENU_MAX_ROW 5		// 菜单最大显示行数 

//定义菜单中功能项的类型
#define TYPE_SUBMENU 101  //具有子菜单的菜单项
#define TYPE_PARAM   102  //参数项（用于执行参数设置）

typedef struct MENU_Init
{
	uint8_t  Menu_Num;                  //当前菜单项数量
    char *Menu_Title;                   //当前菜单标题
    char *Menu_Item;                    //当前菜单项
    uint8_t Type;                       //当前菜单类型
    void (*MENU_FUN)(const char *);;    //当前菜单功能函数
    struct MENU_Init *Menu_Next;        //下一级菜单
    struct MENU_Init *Menu_Prev;        //上一级菜单
}MENU_Init;

void DispCrtMenu();//绘制当前菜单项
void Display(uint8_t value) ;

#endif
