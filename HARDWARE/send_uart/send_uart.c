/**************************************
�����ܣ�
1���������ݵ�TJC4024T032��Ļ��ʾ�������ݣ����ĸ�����������
2������TJC4024T032�·��������������key[2]

***********************************************/

#include "main.h"




int start_time = 0;   //��ʱʱ��    1������ʾ��Сʱ ��ൽ16�����ʱʱ��8Сʱ
int timing_flag = 0;  //��ʱ��־
int switch_flag = 0;  //�л����ٱ�־




char key[key_number]={0,0,0,0,0,0,0};   //����װ������־λ0��δ���£�1������      key[4]ȷ���ǹ������Զ������ֶ���λ
                                    //0~2װ��λ���ݣ�3װ�ر�����

char hex_END[]={0xff,0xff,0xff};   //���͵��������Ľ�����

int OUT_PWM = 1;       //װ���PWM��ֵ�������Ǽ�һ����ģ���������Ǵ�1��ʼ�������һ������65535��



/*****************************************
��������:���ڷ������ݺ���

******************************************/
void USARTx_Send(USART_TypeDef* USARTx, u8 *Data, u8 Len)  
{  
    u8 i = 0;  
    for(i = 0; i < Len; i++)  
    {  
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != SET);     
        USART_SendData(USARTx, Data[i]);                                                     
    }  
}

/*******************************
�������ܣ�
�����������ε���Ļ�������������0����
ch0����ɫ
ch1����ɫ
ch2����ɫ

ע���˳�����ʹ��
*******************************/
void send_wave(int ch0,int ch1,int ch2)
{
  char str1[50];
  sprintf(str1,"add 1,0,%d",ch0);
  USARTx_Send(USART2,(u8 *)str1,strlen(str1));
  USARTx_Send(USART2,(u8 *)hex_END,3);
  
  sprintf(str1,"add 1,1,%d",ch1);
  USARTx_Send(USART2,(u8 *)str1,strlen(str1));
  USARTx_Send(USART2,(u8 *)hex_END,3);
  
  sprintf(str1,"add 1,2,%d",ch2);
  USARTx_Send(USART2,(u8 *)str1,strlen(str1));
  USARTx_Send(USART2,(u8 *)hex_END,3);
}



/*******************************
�������ܣ�
�����ĸ���ͬ���ݵ���Ļ��ʾ

*******************************/
void send_data(float C,float F)
{
  char str1[50]; 
  static int state = 0;
  switch(state)              //����һ������̫�����ֳ�2�δ���
  {
    case 0:
      sprintf(str1,"t2.txt=\"%.1f\"",C);
    break;
    case 1:
      sprintf(str1,"t3.txt=\"%d\"",(int)F);
    break;
  }
 USARTx_Send(USART2,(u8 *)str1,strlen(str1));
  USARTx_Send(USART2,(u8 *)hex_END,3);
  state++;
  state %= 2;
 
}
/********************
�������ܣ�����str1���Ƿ���str2���ַ��������ص�һ����ͬ�ַ��ĵ�ַ


**********************/
int find_str(char* str1,char* str2)
{
    int i,j,flag=-1;
    for(i=0,j=0;str1[i]!=NULL;i++)
    {
        while(str1[i]==str2[j]&&str1[i]&&str2[j])
        {
            i++;
            j++;
            if(str2[j]==NULL)
            {
                flag=i-j;
                return flag;
            }
        }
        j=0;
    }
    return flag;
}
/**********************************************
��������:��ʼ����Ļ��ʾ����


************************************************/
void main_disp_init(void)
{
  char str1[50];
  sprintf(str1,"b0.pic=7");                   //ȫ���������л�Ϊ�ر�                //ͬ����Ļ��ʾͼ��
  USARTx_Send(USART2,(u8 *)str1,strlen(str1));
  USARTx_Send(USART2,(u8 *)hex_END,3);
  sprintf(str1,"b1.pic=7");
  USARTx_Send(USART2,(u8 *)str1,strlen(str1));
  USARTx_Send(USART2,(u8 *)hex_END,3);
  sprintf(str1,"b2.pic=7");
  USARTx_Send(USART2,(u8 *)str1,strlen(str1));
  USARTx_Send(USART2,(u8 *)hex_END,3);
  sprintf(str1,"b3.pic=8");
  USARTx_Send(USART2,(u8 *)str1,strlen(str1));
  USARTx_Send(USART2,(u8 *)hex_END,3);
  sprintf(str1,"page0.h0.val=%d",0);               //���ý�����ֵ
  USARTx_Send(USART2,(u8 *)str1,strlen(str1));
  USARTx_Send(USART2,(u8 *)hex_END,3);
  
  sprintf(str1,"page0.t7.pco=31735");
  USARTx_Send(USART2,(u8 *)str1,strlen(str1));
  USARTx_Send(USART2,(u8 *)hex_END,3);
  
  sprintf(str1,"t7.txt=\"0:0:0\"");
  USARTx_Send(USART2,(u8 *)str1,strlen(str1));
  USARTx_Send(USART2,(u8 *)hex_END,3);
  
  sprintf(str1,"t1.txt=\"�ֶ�ģʽ\"");      //��ʼ����ʾ�ֶ�ģʽ
  USARTx_Send(USART2,(u8 *)str1,strlen(str1));
  USARTx_Send(USART2,(u8 *)hex_END,3);
  
  auto_manual_LED = 1;    //�Զ��ֶ�ָʾ�ƹرձ�ʾ�������ֶ�ģʽ��
  
}
/********************
�������ܣ�����ϵͳ�������ݵĴ���


ע����λ�����͵���KEYx*p
����xΪ�����ţ���Ӧ����key[x]�����飬��*p��Ϊ���͵����ݵ���β
ռʱֻд������
ע��jz��0~2��ʾ1~3����3��ʾ�ر�
    4��ʾ�Զ��ֶ�ģʽ
    5��6��ʾ�Ӽ���ʱʱ��
���������͸�ʽ��valxxxKEY*p 

**********************/
void get_key(void)
{
  // int i = 0;
   int flag = 0,flag1 = 0;
   char str1[50];
   char jz = 50;     //��ʼֵ50��ʾ��Ч��ֵ��������50�������Ӱ�������ط�
   static u8 last_remote_jz = 0;
   deal_remote_val();      //��ȡ����ң������ 

  
   if(USART_RX_STA2&0x8000)            //������Ļ���ܵ�����
   {
     flag = find_str(USART_RX_BUF2,"KEY");   //�����Ƿ��յ�������֡ͷ
     flag1 = find_str(USART_RX_BUF2,"val");   //�����Ƿ��յ���������֡ͷ
     if(flag1 != -1)
     {
        OUT_PWM = 100*USART_RX_BUF2[flag1+3];
        if(OUT_PWM == 0) OUT_PWM = 1;
          switch_flag = 1;               //�������PWM��־
     }
     if(flag != -1)                  //��������ֵ����key�У��ֶ�/�Զ�������������
     {
        key[0] = key[1] = key[2] = key[3] = 0; 
        jz = USART_RX_BUF2[flag+3] - 0x30;
        if(jz == 4)              //�Զ�/�ֶ����ܼ�����
        {
          key[4] = !key[4];  
          OUT_PWM=1; 
          switch_flag=1;
        }
        else if(jz < key_number)    
          key[jz] = 1;      
     }
     USART_RX_STA2 = 0;                           //������壬Ϊ�´���׼��
     memset(USART_RX_BUF2,0,strlen(USART_RX_BUF2));   
   }
   
   if(USART_RX_STA&0x8000)      //�����ֻ����յ�������
   {
      flag = find_str(USART_RX_BUF,"KEY");
      key[0] = key[1] = key[2] = key[3] = 0; 
      jz = USART_RX_BUF[flag+3] - 0x30;
      if(jz < key_number)    
        key[jz] = 1;
                
      USART_RX_STA = 0;
      memset(USART_RX_BUF,0,strlen(USART_RX_BUF));   
   }   
   
   if( (remote_jz == 1 || remote_jz == 2 || remote_jz == 3 || remote_jz == 11 || remote_jz == 10))        //�������������ݣ�������������
   {
     key[0] = key[1] = key[2] = key[3] = 0; 
     switch(remote_jz)
     {
       case 1:key[0] = 1;jz = 0;break;
       case 2:key[1] = 1;jz = 1;break;
       case 3:key[2] = 1;jz = 2;break;
       case 11:key[3] = 1;jz = 3;break;    //'#'��ʾ�رհ���
       case 10:key[4] = !key[4];OUT_PWM=1;switch_flag=1; break;  //'*'��ʾ�л��ֶ��Զ�ģʽ
     }
     last_remote_jz = remote_jz;
     remote_jz = 0;
   }
   
   
//���ϻ�ȡ��λ����Ϣ�����´�����λ����Ϣ   

   if(key[4] == 0)   //�ֶ�ģʽ��
   {
      sprintf(str1,"b4.pic=3");                   //���ָʾ��־
      USARTx_Send(USART2,(u8 *)str1,strlen(str1));
      USARTx_Send(USART2,(u8 *)hex_END,3);

      sprintf(str1,"t1.txt=\"�ֶ�ģʽ\"");  
      USARTx_Send(USART2,(u8 *)str1,strlen(str1));
      USARTx_Send(USART2,(u8 *)hex_END,3); 

      auto_manual_LED = 1;    
      if(key[0] == 1 || key[1] == 1 || key[2] == 1 || key[3] == 1)    //�κ�һ�����԰������¶�ִ��һ��
      {
        sprintf(str1,"b0.pic=7");                   //ȫ���������л�Ϊ�ر�                //ͬ����Ļ��ʾͼ��
        USARTx_Send(USART2,(u8 *)str1,strlen(str1));
        USARTx_Send(USART2,(u8 *)hex_END,3);
        sprintf(str1,"b1.pic=7");
        USARTx_Send(USART2,(u8 *)str1,strlen(str1));
        USARTx_Send(USART2,(u8 *)hex_END,3);
        sprintf(str1,"b2.pic=7");
        USARTx_Send(USART2,(u8 *)str1,strlen(str1));
        USARTx_Send(USART2,(u8 *)hex_END,3);
        sprintf(str1,"b3.pic=7");
        USARTx_Send(USART2,(u8 *)str1,strlen(str1));
        USARTx_Send(USART2,(u8 *)hex_END,3);;
           
        sprintf(str1,"b%d.pic=8",jz);         //���µİ����л�ͼ��    
        USARTx_Send(USART2,(u8 *)str1,strlen(str1));
        USARTx_Send(USART2,(u8 *)hex_END,3); 
        
        switch_flag = 1;                                     
      } 
       if(jz != 50)   //50��Ч��ֵ
       {    
                   
          switch(jz)       
          {
            case 0:OUT_PWM = 2000;break;             //ȷ����ͬ��λ���PWMֵ
            case 1:OUT_PWM = 5000;break;
            case 2:OUT_PWM = 8000;break;
            case 3:OUT_PWM = 1;break;     //�ر�
            case 5:
                   if(OUT_PWM != 1)                 //���ֹͣ״̬���������ö�ʱʱ��
                   {
                       timing_flag = 1;
                       start_time--;
                      if(start_time == -1)    //Ϊ0��Ϊ1ʱ������Ч
                        start_time = timing_max;
                    }
                    break;
            case 6:
                   if(OUT_PWM != 1)
                   {                    
                      timing_flag = 1; 
                      start_time++;
                      if(start_time>timing_max)     //����10��
                        start_time = 0;
                   }
                   break;
          }  
          
          if(timing_flag == 1 && (jz == 5 || jz == 6))   //
          {              
            if(start_time == 0)                        //δ��ʱ״̬������������Ϊ��ɫ
            {
              sprintf(str1,"page0.t7.pco=31735");           
              USARTx_Send(USART2,(u8 *)str1,strlen(str1));
              USARTx_Send(USART2,(u8 *)hex_END,3);
            }
            else
            {
              sprintf(str1,"page0.t7.pco=65535");     //δ��ʱ״̬������������Ϊ��ɫ
              USARTx_Send(USART2,(u8 *)str1,strlen(str1));
              USARTx_Send(USART2,(u8 *)hex_END,3);
            }
            
            sprintf(str1,"t7.txt=\"%d:%d:0\"",(start_time*timing)/60,(start_time*timing)%60);    //���Ͷ�ʱʱ������Ļ����ʾ
            USARTx_Send(USART2,(u8 *)str1,strlen(str1));
            USARTx_Send(USART2,(u8 *)hex_END,3);
          }
          key[0] = key[1] = key[2] = key[3] = key[5]= key[6] = 0; 
          jz = 50;
        }
        
   }  
   else   //�Զ�ģʽ��
   {
      sprintf(str1,"b4.pic=4");                       //���ָʾ��־
      USARTx_Send(USART2,(u8 *)str1,strlen(str1));
      USARTx_Send(USART2,(u8 *)hex_END,3); 
      sprintf(str1,"t1.txt=\"�Զ�ģʽ\"");  
      USARTx_Send(USART2,(u8 *)str1,strlen(str1));
      USARTx_Send(USART2,(u8 *)hex_END,3);  
      
      auto_manual_LED = 0;               
     
     
       if(temperature/10.0  < expect_min)  //С����������¶ȣ��رշ���
         OUT_PWM = 1;
       else
       {        
          OUT_PWM = 1000 + (temperature/10.0-expect_min)*7000.0/(expect_max-expect_min);   //�����Զ�ģʽ�����PWM
       }
       if(OUT_PWM >= 8000)    //������PWM
         OUT_PWM = 7999;
       switch_flag = 1; 
   }
   
   if(switch_flag == 1)  //������ʾΪ1�ŵ���һ�����PWM
   {
      sprintf(str1,"page0.h0.val=%d",OUT_PWM/100);
      USARTx_Send(USART2,(u8 *)str1,strlen(str1));
      USARTx_Send(USART2,(u8 *)hex_END,3);
      TIM_SetCompare1(TIM3,OUT_PWM-1); 
     
      switch_flag = 0;
   }
   if(timing_flag == 2 || OUT_PWM == 1)        //��ʱ���������PWMΪ1ʱ���ر��������ʼ������
   {
     OUT_PWM = 1;
     timing_flag = 0;
     main_disp_init();
     TIM_SetCompare1(TIM3,OUT_PWM-1); 
   }
}





