/*
ʹ�ö�ʱ���ķ�����
1��ʹ�ܶ�ʱ��ʱ��RCC_APB1PeriphClockCmd();
2����ʼ����ʱ��������ARR��PSC��TIM_TimeBaseInit();
    T(out)(���ʱ��) = (ARR+1)(PSC+1)/Tclk
3��������ʱ���жϣ�����NVIC���� TIM_ITConfig();NVIC_Init();
4��ʹ�ܶ�ʱ�� TIM_Cmd();
5����д�жϷ�����


ע�⣺
1���жϽ���ʱ������Ҫ����жϱ�־����������һֱ�����жϣ��������޷����У�
��������ֵĺ���ǣ�����LED�ƶ�������
2������LED��λ�����������ԣ��ڱ������ͷ�ļ��б������sys.h��ͷ�ļ���λ��
�����ĺ궨��������

PWM������ò��裺
1��ʹ�ܶ�ʱ���������io�ڵ�ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  //ʹ�ܶ�ʱ��3��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);  //ʹ��GPIO��AFIOʱ��
2����ʼ��io��Ϊ���ù������GPIO_Init(); 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;//����Ϊ�����������
3����TIM3�������ӳ�䵽PB5(�����õ��ǲ�����ӳ��)��ͬʱʹ��AFIOʱ��
   GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);  //��TIM3����ӳ����PB5���� 
4����ʼ����ʱ��
5����ʼ������Ƚϲ���
   TIM_OC2Init(TIM3,&TIM_OCInitStruce);
   ע�⣺������Ҫ��ʼ���ĳ�Ա�����ǣ�TIM_OCMode��TIM_OutputState��TIM_OCNPolarity
6��ʹ��Ԥװ�ؼĴ���
   TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);    //ʹ��Ԥװ����
7��ʹ�ܶ�ʱ��
    TIM_Cmd(TIM3,ENABLE);                           //ʹ�ܶ�ʱ��            
*/

#include "main.h"






void TIMER_Init(u16 arr,u16 pros)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruce;
    NVIC_InitTypeDef NVIC_InitStruce;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    
    TIM_TimeBaseInitStruce.TIM_Period =arr;        //��ʼ����ʱ��������ARR��PSC��ARR���Զ���װ�Ĵ�����PSC����Ƶ�Ĵ�����
    TIM_TimeBaseInitStruce.TIM_Prescaler = pros;
    TIM_TimeBaseInitStruce.TIM_CounterMode = TIM_CounterMode_Up;     
    TIM_TimeBaseInitStruce.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruce);
    
    TIM_ITConfig(TIM3,TIM_FLAG_Update,ENABLE);       //������ʱ���ж�
    
    NVIC_InitStruce.NVIC_IRQChannel = TIM3_IRQn;         //��ʼ��NVIC
    NVIC_InitStruce.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruce.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruce.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_InitStruce);
    
    TIM_Cmd(TIM3,ENABLE);           //ʹ�ܶ�ʱ��
       
}



void TIM3_PWM_Init(u16 arr,u16 pros)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruce;
    TIM_OCInitTypeDef TIM_OCInitStruce;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  //ʹ�ܶ�ʱ��3��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);  //ʹ��GPIO��AFIOʱ��
        
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;//����Ϊ�����������
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;      //����PA.5Ϊ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�Ϊ50mhz
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
 //   GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);  //��TIM3����ӳ����PB5����

    TIM_TimeBaseInitStruce.TIM_Period =arr;        //��ʼ����ʱ��������ARR��PSC��ARR���Զ���װ�Ĵ�����PSC����Ƶ�Ĵ�����
    TIM_TimeBaseInitStruce.TIM_Prescaler = pros;
    TIM_TimeBaseInitStruce.TIM_CounterMode = TIM_CounterMode_Up;     
    TIM_TimeBaseInitStruce.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruce);
    
    TIM_OCInitStruce.TIM_OCMode = TIM_OCMode_PWM1;          //��ʼ���Ƚϲ���
    TIM_OCInitStruce.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruce.TIM_OCNPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM3,&TIM_OCInitStruce);
    
    TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);    //ʹ��Ԥװ����
    
    TIM_Cmd(TIM3,ENABLE);                           //ʹ�ܶ�ʱ��
    TIM_SetCompare1(TIM3,0); 
       
}
void TIM5_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx					 
}
void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx					 
}
void TIM2_IRQHandler(void)   //TIM3�ж�
{
  static int i,number = 0;
  int miao;
  char str1[50];
  miao =  start_time*timing*60;                      //���㶨ʱ������
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
  {
    if(timing_flag == 0)
      i = 0;
    else if(timing_flag == 1)
      number++;
    if(number >= 1000)          //1s��
    {
      number = 0;
      i++;
      if(i >= miao)           //��ʱʱ�䵽����ʱ��־λtiming_flag��2���رշ��ȣ��ָ�����ʼ��״̬
      {
        timing_flag = 2;
        start_time = 0;
        i = 0;      
      }
      sprintf(str1,"t7.txt=\"%d:%d:%d\"",(miao-i)/3600,(miao-i)%3600/60,(miao-i)%60);    //������ʱ���͵���Ļ����ʾ
      USARTx_Send(USART2,(u8 *)str1,strlen(str1));
      USARTx_Send(USART2,(u8 *)hex_END,3);
      
    } 
  }
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
}

void TIM3_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update) == 1)
    {
      
    }
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}    

