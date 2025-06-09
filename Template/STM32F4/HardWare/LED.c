#include "LED.h"

LED_LIGHT_MODE LED_Mode[LED_NUM];      //LED模式
uint32_t LED_Count[LED_NUM];           //LED计数    
uint8_t LED_Compare = 0;
uint8_t LED_Flag = 1;

/****
	* @brief    LED  初始化		
	* @param   	无
	* @return   无    	
    * Sample usage:LED_Init(); 
    */
void LED_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    LED_APBxClockCMD(LEDx_CLK,ENABLE);      //使能端口时钟

    GPIO_InitStructure.GPIO_Pin = LED_LEFT_GPIO_PIN | LED_RIGHT_GPIO_PIN | LED_MIDDLE_GPIO_PIN | LED_DOWN_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      //设置模式为输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);
    GPIO_Init(LEDx_GPIO_PORT, &GPIO_InitStructure);

    LED_LEFT(OFF);
    LED_RIGHT(OFF);
    LED_MIDDLE(OFF);
    LED_DOWN(OFF);
}

/****
	* @brief    LED  设置亮灭模式		
	* @param   	Num  LED ID   
    * @param   	Mode 亮灭模式
	* @return   无    	
    * Sample usage:LED_SetMode(); 
    */
void LED_SetMode(uint8_t Num,LED_LIGHT_MODE Mode)
{
    if(Mode != LED_Mode[Num])
    {
        LED_Mode[Num] = Mode;
        LED_Count[Num] = 0;
    }
}

/****
	* @brief    控制 LED 亮灭	
	* @param   	Led_Num LED ID
    * @param   	Status 状态 ON/OFF  
	* @return   无    	
    * Sample usage:LED_Control(); 
    */
static void LED_Control(uint8_t Led_Num, uint8_t Status)
{
    if(Led_Num == LED_LEFT)
    {
        LED_LEFT(Status);
    }
    else if(Led_Num == LED_RIGHT)
    {
        LED_RIGHT(Status);
    }
    else if(Led_Num == LED_MIDDLE)
    {
        LED_MIDDLE(Status);
    }
    else if(Led_Num == LED_DOWN)
    {
        LED_DOWN(Status);
    }
}

/****
	* @brief    LED  亮灭控制		
	* @param   	无
	* @return   无    	
    * Sample usage:LED_Tick(); 
    */
void LED_Tick()
{
    for(uint8_t i = 0;i < LED_NUM;i++)
    {
        switch(LED_Mode[i])
        {
            case LED_OFF:
                LED_Control(i,OFF);
                break;
            case LED_ON:
                LED_Control(i,ON);
                break;
            case LED_SLOW_BLINK:
                {
                    LED_Count[i]++;
                    LED_Count[i] %= 1000;
                    if(LED_Count[i] < 500)     {LED_Control(i,ON);}
                    else {LED_Control(i,OFF);}
                }
                break;
            case LED_FAST_BLINK:
                {
                    LED_Count[i]++;
                    LED_Count[i] %= 100;
                    if(LED_Count[i] < 50)    {LED_Control(i,ON);}
                    else {LED_Control(i,OFF);}
                }
                break;
            case LED_BIT_BLINK:
                {
                    LED_Count[i]++;
                    LED_Count[i] %= 1000;
                    if(LED_Count[i] < 50)    {LED_Control(i,ON);}
                    else {LED_Control(i,OFF);}
                }
                break;
            case LED_BREATHE:
                {
                    LED_Count[i]++;
                    LED_Count[i] %= 255;
                    if(LED_Count[i] < LED_Compare) {LED_Control(i,ON);}
                    else {LED_Control(i,OFF);}
                    if (LED_Flag)
                    {
                        LED_Compare++;
                    if(LED_Compare > 255) {LED_Flag = 0;}
                    }
                    else 
                    {
                        LED_Compare--;
                        if(LED_Compare < 0) {LED_Flag = 1;}
                    }
                }
                break;
            default:
                break;
        }
    }
}
