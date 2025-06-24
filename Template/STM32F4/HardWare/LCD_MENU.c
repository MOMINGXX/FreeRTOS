#include "LCD_MENU.h"

//各级菜单页面结构体声明  

MENU_Init LIGHT_DISPLAY_MENU[10]; 
MENU_Init MAIN_MENU[9]; 
MENU_Init SETTINGS_MENU[5]; 
MENU_Init GYROSCOPE_MENU[2]; 

//定义菜单操作需要的全局变量
MENU_Init *cur_item = MAIN_MENU;  //初始化当前菜单为第一级(main_menu)
MENU_Init *prev_item = NULL;	    //初始化上一级菜单为空
uint8_t item_index = 0;//当前菜单项索引

//结构体初始化//菜单定义,在这里将每一个菜单的关联设置好
MENU_Init MAIN_MENU[9] = // 第1级 主菜单 
{
	{9, "主菜单", "摇杆模式", 	TYPE_PARAM, NULL, NULL, NULL}, 
	{9, "",		  "按键模式", 	TYPE_PARAM, NULL, NULL, NULL}, 
	{9, "",		  "避障模式", 	TYPE_PARAM, NULL, NULL, NULL}, 
	{9, "",		  "陀螺仪模式", 	TYPE_SUBMENU, NULL, GYROSCOPE_MENU, NULL}, 
	{9, "",		  "灯光展示", 	TYPE_SUBMENU, NULL, LIGHT_DISPLAY_MENU, NULL}, 
	{9, "",		  "音乐播放", 	TYPE_PARAM, NULL, NULL, NULL}, 
	{9, "",		  "图片显示", 	TYPE_PARAM, NULL, NULL, NULL}, 
	{9, "",		  "设置", 		TYPE_SUBMENU, NULL, SETTINGS_MENU, NULL}, 
	{9, "",		  "帮助", 		TYPE_PARAM, NULL, NULL, NULL}, 
}; 

MENU_Init LIGHT_DISPLAY_MENU[10] =  // 第2级 灯光菜单 
{
	{10, "灯光展示", "红色",  TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "白色", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "黄色", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "蓝色", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "绿色", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "橙色", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "紫色", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "呼吸灯", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "流水灯", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "彩虹", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
};

MENU_Init SETTINGS_MENU[5] =  // 第2级 设置菜单 
{ 
	{4, "设置","屏幕亮度",TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{4, "",	   "语言", 	TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{4, "",	   "电量", 	TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{4, "",	   "存储", 	TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
};    

MENU_Init GYROSCOPE_MENU[2] =  // 第2级 陀螺仪菜单 
{
	{2, "陀螺仪模式",	"优化显示", 	TYPE_SUBMENU, NULL, NULL, MAIN_MENU}, 
	{2, "",		  		"简易显示", 	TYPE_SUBMENU, NULL, NULL, MAIN_MENU}, 
};

void DrawTestPage(uint8_t *str)
{
	//绘制固定栏up
	LCD_Fill(0,0,LCD_W,56,CYAN);
	//绘制固定栏down
	LCD_Fill(0,LCD_H-48,LCD_W,LCD_H,CYAN);
	Gui_StrCenter(0,24,(char *)str,RED,CYAN,32,1);  //菜单顶部标题 居中显示
	Gui_StrCenter(0,LCD_H-32,"",RED,CYAN,24,1);//菜单底部 居中显示
	LCD_Fill(0,56,LCD_W,LCD_H-48,BLACK);
}

void DispCrtMenu()//绘制当前菜单项
{
	uint8_t menu_num = cur_item[0].Menu_Num;//获取当前菜单的项目数量 
	
	if(item_index>=menu_num)//菜单项上下选择是否越界 
	{ 
		if(item_index==0XFF)  //此情况为 item_index=0 再按上键，0-1=0XFF
		{
			item_index = menu_num - 1;   //循环，回到最后一个功能索引值
		}
		if(item_index>=menu_num)  //此情况为到达最下面一个功能索引值
		{
			item_index = 0;  //循环，回到第一个功能索引值
		}
	}
	
	DrawTestPage((uint8_t *)cur_item[0].Menu_Title);	
	
	if(item_index < MENU_MAX_ROW)
	{
		for (int i=0; i<MENU_MAX_ROW; i++)
		{
			LCD_ShowGB2312_Chinese(5,24+(i+1)*34,24,(uint8_t *)cur_item[i].Menu_Item,LIGHTBLUE,WHITE,i==item_index ? 0:1);
		}
	}
	else 
	{
		int current_item  =item_index - (MENU_MAX_ROW - 1);
		for (int i=0; i < MENU_MAX_ROW; i++)
		{
			LCD_ShowGB2312_Chinese(5,24+(i+1)*34,24,(uint8_t *)cur_item[current_item + i].Menu_Item,LIGHTBLUE,WHITE,(current_item + i)==item_index ? 0:1);
		}
	}
}

#define KEY_ENTER_PRESS  3 //对应原子开发板的左键
#define KEY_RETURN_PRESS 4 //对应原子F4板的右键

#define KEY_UP_PRESS    1
#define KEY_DOWN_PRESS  2

void Display(uint8_t value) 
{
	if(value==KEY_UP_PRESS || value==KEY_DOWN_PRESS || value==KEY_ENTER_PRESS || value==KEY_RETURN_PRESS)
	{
		switch(value)//检测按键，进入相应动作
		{
			case KEY_UP_PRESS: 
				item_index--; 
				DispCrtMenu();
				break; 
			case KEY_DOWN_PRESS: 
				item_index++; 
				DispCrtMenu();
				break; 
			case KEY_ENTER_PRESS:
				switch(cur_item[item_index].Type)//检测功能项的类型，进入相应动作
				{
					case TYPE_SUBMENU: //具有子菜单的菜单项
						if(cur_item[item_index].Menu_Next != NULL)
						{ 
							prev_item = cur_item;//此级菜单变成了上一级菜单 
							cur_item = cur_item[item_index].Menu_Next;//将指向的下一级菜单设置为当前菜单 
							item_index = 0;//重置子菜单项索引 
							DispCrtMenu();
						}
						break; 
					case TYPE_PARAM:  //具有参数设置的菜单项
						if(cur_item[item_index].MENU_FUN != NULL)
						{ 
							//调用相应的动作函数,并传递参数
							cur_item[item_index].MENU_FUN((const char *)cur_item[item_index].Menu_Item);
						}
						break; 
					default: 
						break;
				}
				break; 
			case KEY_RETURN_PRESS: 
				if (prev_item != NULL)//返回上一级菜单的操作 
				{
					cur_item = prev_item;  //设置上一级菜单为当前菜单 
					prev_item = cur_item[0].Menu_Prev;  //设置当前菜单的上一级菜单 
					item_index = 0;  //重置子菜单项索引
					DispCrtMenu();
				} 
				break; 
		    default: 
		        break;
		}
	}
} 