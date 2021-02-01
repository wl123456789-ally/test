#include "led.h"
#include "stm32f10x.h"
void LED_init(void)
 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //配置时钟信息
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;//设置为推挽输出
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;      //设置PA.5为输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输出速度为50mhz
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;//设置为推挽输出
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;      //设置PD.2输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输出速度为50mhz
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
	
}


