/*
使用定时器的方法：
1、使能定时器时钟RCC_APB1PeriphClockCmd();
2、初始化定时器，配置ARR，PSC，TIM_TimeBaseInit();
    T(out)(溢出时间) = (ARR+1)(PSC+1)/Tclk
3、开启定时器中断，配置NVIC函数 TIM_ITConfig();NVIC_Init();
4、使能定时器 TIM_Cmd();
5、编写中断服务函数


注意：
1、中断结束时，必须要清除中断标志，否则程序会一直进入中断，主程序无法运行，
本程序出现的后果是，两个LED灯都不两个
2、由于LED是位带操作，所以，在本程序的头文件中必须包含sys.h的头文件，位带
操作的宏定义在里面

PWM输出配置步骤：
1、使能定时器三和相关io口的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  //使能定时器3的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);  //使能GPIO和AFIO时钟
2、初始化io口为复用功能输出GPIO_Init(); 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;//设置为复用推挽输出
3、将TIM3的输出重映射到PB5(这里用的是部分重映射)，同时使能AFIO时钟
   GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);  //将TIM3部分映射在PB5上面 
4、初始化定时器
5、初始化输出比较参数
   TIM_OC2Init(TIM3,&TIM_OCInitStruce);
   注意：里面需要初始化的成员变量是：TIM_OCMode、TIM_OutputState、TIM_OCNPolarity
6、使能预装载寄存器
   TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);    //使能预装载器
7、使能定时器
    TIM_Cmd(TIM3,ENABLE);                           //使能定时器            
*/

#include "main.h"






void TIMER_Init(u16 arr,u16 pros)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruce;
    NVIC_InitTypeDef NVIC_InitStruce;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    
    TIM_TimeBaseInitStruce.TIM_Period =arr;        //初始化定时器，配置ARR，PSC（ARR是自动重装寄存器，PSC，分频寄存器）
    TIM_TimeBaseInitStruce.TIM_Prescaler = pros;
    TIM_TimeBaseInitStruce.TIM_CounterMode = TIM_CounterMode_Up;     
    TIM_TimeBaseInitStruce.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruce);
    
    TIM_ITConfig(TIM3,TIM_FLAG_Update,ENABLE);       //开启定时器中断
    
    NVIC_InitStruce.NVIC_IRQChannel = TIM3_IRQn;         //初始化NVIC
    NVIC_InitStruce.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruce.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruce.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_InitStruce);
    
    TIM_Cmd(TIM3,ENABLE);           //使能定时器
       
}



void TIM3_PWM_Init(u16 arr,u16 pros)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruce;
    TIM_OCInitTypeDef TIM_OCInitStruce;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  //使能定时器3的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);  //使能GPIO和AFIO时钟
        
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;//设置为复用推挽输出
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;      //设置PA.5为输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输出速度为50mhz
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
 //   GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);  //将TIM3部分映射在PB5上面

    TIM_TimeBaseInitStruce.TIM_Period =arr;        //初始化定时器，配置ARR，PSC（ARR是自动重装寄存器，PSC，分频寄存器）
    TIM_TimeBaseInitStruce.TIM_Prescaler = pros;
    TIM_TimeBaseInitStruce.TIM_CounterMode = TIM_CounterMode_Up;     
    TIM_TimeBaseInitStruce.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruce);
    
    TIM_OCInitStruce.TIM_OCMode = TIM_OCMode_PWM1;          //初始化比较参数
    TIM_OCInitStruce.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruce.TIM_OCNPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM3,&TIM_OCInitStruce);
    
    TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);    //使能预装载器
    
    TIM_Cmd(TIM3,ENABLE);                           //使能定时器
    TIM_SetCompare1(TIM3,0); 
       
}
void TIM5_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM5, ENABLE);  //使能TIMx					 
}
void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM2, ENABLE);  //使能TIMx					 
}
void TIM2_IRQHandler(void)   //TIM3中断
{
  static int i,number = 0;
  int miao;
  char str1[50];
  miao =  start_time*timing*60;                      //计算定时的秒数
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
  {
    if(timing_flag == 0)
      i = 0;
    else if(timing_flag == 1)
      number++;
    if(number >= 1000)          //1s到
    {
      number = 0;
      i++;
      if(i >= miao)           //定时时间到，定时标志位timing_flag置2，关闭风扇，恢复到初始化状态
      {
        timing_flag = 2;
        start_time = 0;
        i = 0;      
      }
      sprintf(str1,"t7.txt=\"%d:%d:%d\"",(miao-i)/3600,(miao-i)%3600/60,(miao-i)%60);    //将倒计时发送到屏幕上显示
      USARTx_Send(USART2,(u8 *)str1,strlen(str1));
      USARTx_Send(USART2,(u8 *)hex_END,3);
      
    } 
  }
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
}

void TIM3_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update) == 1)
    {
      
    }
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}    

