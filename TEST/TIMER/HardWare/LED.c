#include "LED.h"

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