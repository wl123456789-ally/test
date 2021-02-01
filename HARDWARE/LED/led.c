#include "led.h"
#include "stm32f10x.h"
void LED_init(void)
 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //����ʱ����Ϣ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;//����Ϊ�������
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;      //����PA.5Ϊ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�Ϊ50mhz
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;//����Ϊ�������
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;      //����PD.2���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�Ϊ50mhz
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
	
}


