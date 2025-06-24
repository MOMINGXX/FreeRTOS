#include "LCD_MENU.h"

//�����˵�ҳ��ṹ������  

MENU_Init LIGHT_DISPLAY_MENU[10]; 
MENU_Init MAIN_MENU[9]; 
MENU_Init SETTINGS_MENU[5]; 
MENU_Init GYROSCOPE_MENU[2]; 

//����˵�������Ҫ��ȫ�ֱ���
MENU_Init *cur_item = MAIN_MENU;  //��ʼ����ǰ�˵�Ϊ��һ��(main_menu)
MENU_Init *prev_item = NULL;	    //��ʼ����һ���˵�Ϊ��
uint8_t item_index = 0;//��ǰ�˵�������

//�ṹ���ʼ��//�˵�����,�����ｫÿһ���˵��Ĺ������ú�
MENU_Init MAIN_MENU[9] = // ��1�� ���˵� 
{
	{9, "���˵�", "ҡ��ģʽ", 	TYPE_PARAM, NULL, NULL, NULL}, 
	{9, "",		  "����ģʽ", 	TYPE_PARAM, NULL, NULL, NULL}, 
	{9, "",		  "����ģʽ", 	TYPE_PARAM, NULL, NULL, NULL}, 
	{9, "",		  "������ģʽ", 	TYPE_SUBMENU, NULL, GYROSCOPE_MENU, NULL}, 
	{9, "",		  "�ƹ�չʾ", 	TYPE_SUBMENU, NULL, LIGHT_DISPLAY_MENU, NULL}, 
	{9, "",		  "���ֲ���", 	TYPE_PARAM, NULL, NULL, NULL}, 
	{9, "",		  "ͼƬ��ʾ", 	TYPE_PARAM, NULL, NULL, NULL}, 
	{9, "",		  "����", 		TYPE_SUBMENU, NULL, SETTINGS_MENU, NULL}, 
	{9, "",		  "����", 		TYPE_PARAM, NULL, NULL, NULL}, 
}; 

MENU_Init LIGHT_DISPLAY_MENU[10] =  // ��2�� �ƹ�˵� 
{
	{10, "�ƹ�չʾ", "��ɫ",  TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "��ɫ", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "��ɫ", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "��ɫ", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "��ɫ", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "��ɫ", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "��ɫ", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "������", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "��ˮ��", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{10, "",     "�ʺ�", 	 TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
};

MENU_Init SETTINGS_MENU[5] =  // ��2�� ���ò˵� 
{ 
	{4, "����","��Ļ����",TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{4, "",	   "����", 	TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{4, "",	   "����", 	TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
	{4, "",	   "�洢", 	TYPE_PARAM, NULL, NULL, MAIN_MENU}, 
};    

MENU_Init GYROSCOPE_MENU[2] =  // ��2�� �����ǲ˵� 
{
	{2, "������ģʽ",	"�Ż���ʾ", 	TYPE_SUBMENU, NULL, NULL, MAIN_MENU}, 
	{2, "",		  		"������ʾ", 	TYPE_SUBMENU, NULL, NULL, MAIN_MENU}, 
};

void DrawTestPage(uint8_t *str)
{
	//���ƹ̶���up
	LCD_Fill(0,0,LCD_W,56,CYAN);
	//���ƹ̶���down
	LCD_Fill(0,LCD_H-48,LCD_W,LCD_H,CYAN);
	Gui_StrCenter(0,24,(char *)str,RED,CYAN,32,1);  //�˵��������� ������ʾ
	Gui_StrCenter(0,LCD_H-32,"",RED,CYAN,24,1);//�˵��ײ� ������ʾ
	LCD_Fill(0,56,LCD_W,LCD_H-48,BLACK);
}

void DispCrtMenu()//���Ƶ�ǰ�˵���
{
	uint8_t menu_num = cur_item[0].Menu_Num;//��ȡ��ǰ�˵�����Ŀ���� 
	
	if(item_index>=menu_num)//�˵�������ѡ���Ƿ�Խ�� 
	{ 
		if(item_index==0XFF)  //�����Ϊ item_index=0 �ٰ��ϼ���0-1=0XFF
		{
			item_index = menu_num - 1;   //ѭ�����ص����һ����������ֵ
		}
		if(item_index>=menu_num)  //�����Ϊ����������һ����������ֵ
		{
			item_index = 0;  //ѭ�����ص���һ����������ֵ
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

#define KEY_ENTER_PRESS  3 //��Ӧԭ�ӿ���������
#define KEY_RETURN_PRESS 4 //��Ӧԭ��F4����Ҽ�

#define KEY_UP_PRESS    1
#define KEY_DOWN_PRESS  2

void Display(uint8_t value) 
{
	if(value==KEY_UP_PRESS || value==KEY_DOWN_PRESS || value==KEY_ENTER_PRESS || value==KEY_RETURN_PRESS)
	{
		switch(value)//��ⰴ����������Ӧ����
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
				switch(cur_item[item_index].Type)//��⹦��������ͣ�������Ӧ����
				{
					case TYPE_SUBMENU: //�����Ӳ˵��Ĳ˵���
						if(cur_item[item_index].Menu_Next != NULL)
						{ 
							prev_item = cur_item;//�˼��˵��������һ���˵� 
							cur_item = cur_item[item_index].Menu_Next;//��ָ�����һ���˵�����Ϊ��ǰ�˵� 
							item_index = 0;//�����Ӳ˵������� 
							DispCrtMenu();
						}
						break; 
					case TYPE_PARAM:  //���в������õĲ˵���
						if(cur_item[item_index].MENU_FUN != NULL)
						{ 
							//������Ӧ�Ķ�������,�����ݲ���
							cur_item[item_index].MENU_FUN((const char *)cur_item[item_index].Menu_Item);
						}
						break; 
					default: 
						break;
				}
				break; 
			case KEY_RETURN_PRESS: 
				if (prev_item != NULL)//������һ���˵��Ĳ��� 
				{
					cur_item = prev_item;  //������һ���˵�Ϊ��ǰ�˵� 
					prev_item = cur_item[0].Menu_Prev;  //���õ�ǰ�˵�����һ���˵� 
					item_index = 0;  //�����Ӳ˵�������
					DispCrtMenu();
				} 
				break; 
		    default: 
		        break;
		}
	}
} 