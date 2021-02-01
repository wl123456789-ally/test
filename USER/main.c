/*************************************************************

程序功能：
①可以完成传统电风扇1、2、3挡调速
②可以拖动屏幕上的滑块进行无极调速（可以实现最大最小范围内任意速度）
③分为手动挡和自动挡
 手动挡：实现①、②功能
 自动挡：实现在25~35摄氏度内自动适合的风速（程序里面调整）
④可以采集室内温度和湿度并且在HMI串口屏上显示出来
⑤可以采用红外遥控实现自动手动切换+三挡调速   （通过屏幕显示和单片机最小系统上LED1表示）
⑥可以使用手机app对电风扇进行三挡调速
⑦可以定时关闭电风扇
⑧LED指示灯，正常工作时，闪烁
引脚说明：
串口1：PA9、PA10   //接蓝牙
串口2：PA2、PA3    //接HMI屏幕
       PB10    温度传感器
       PC4     湿度传感器
       PA6     控制电风扇速度
       PB9     红外传感器


                                                                                                                                         
*************************************************************/
#include "main.h"




float temperature = 100.42;
float DHT11_humi = 0;
float DHT11_temp = 0;



int main(void)
 {		
 	u16 t = 0;  
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
 	LED_init();			     //LED端口初始化
  uart_init(9600);     //蓝牙模块
  uart2_init(9600);	   //HMI串口屏
   
  TIM3_PWM_Init(9999,71);    //100hz
  
  TIM2_Int_Init(999,71);  //1k
   
  DS18B20_Init();
  DHT11_Init();
  Remote_Init();   //红外遥控初始化
  main_disp_init();  //初始化屏幕显示

 	while(1)
	{    
      
    temperature = DS18B20_Get_Temp();
    DHT11_Read_Data(&DHT11_temp,&DHT11_humi);
    send_data(temperature/10.0,DHT11_humi);    //发送温度湿度
    get_key();            ///获取按键值
    t++;
    if(t == 10)
    {
      LED0 = 0;
    }
    else if(t == 20)
    {
      t = 0;
      LED0 = 1;
    }  
	}	
 }

