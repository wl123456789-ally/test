/*************************************************************

�����ܣ�
�ٿ�����ɴ�ͳ�����1��2��3������
�ڿ����϶���Ļ�ϵĻ�������޼����٣�����ʵ�������С��Χ�������ٶȣ�
�۷�Ϊ�ֶ������Զ���
 �ֶ�����ʵ�֢١��ڹ���
 �Զ�����ʵ����25~35���϶����Զ��ʺϵķ��٣��������������
�ܿ��Բɼ������¶Ⱥ�ʪ�Ȳ�����HMI����������ʾ����
�ݿ��Բ��ú���ң��ʵ���Զ��ֶ��л�+��������   ��ͨ����Ļ��ʾ�͵�Ƭ����Сϵͳ��LED1��ʾ��
�޿���ʹ���ֻ�app�Ե���Ƚ�����������
�߿��Զ�ʱ�رյ����
��LEDָʾ�ƣ���������ʱ����˸
����˵����
����1��PA9��PA10   //������
����2��PA2��PA3    //��HMI��Ļ
       PB10    �¶ȴ�����
       PC4     ʪ�ȴ�����
       PA6     ���Ƶ�����ٶ�
       PB9     ���⴫����


                                                                                                                                         
*************************************************************/
#include "main.h"




float temperature = 100.42;
float DHT11_humi = 0;
float DHT11_temp = 0;



int main(void)
 {		
 	u16 t = 0;  
	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
 	LED_init();			     //LED�˿ڳ�ʼ��
  uart_init(9600);     //����ģ��
  uart2_init(9600);	   //HMI������
   
  TIM3_PWM_Init(9999,71);    //100hz
  
  TIM2_Int_Init(999,71);  //1k
   
  DS18B20_Init();
  DHT11_Init();
  Remote_Init();   //����ң�س�ʼ��
  main_disp_init();  //��ʼ����Ļ��ʾ

 	while(1)
	{    
      
    temperature = DS18B20_Get_Temp();
    DHT11_Read_Data(&DHT11_temp,&DHT11_humi);
    send_data(temperature/10.0,DHT11_humi);    //�����¶�ʪ��
    get_key();            ///��ȡ����ֵ
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
