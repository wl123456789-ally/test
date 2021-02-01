/**************************************
程序功能：
1、发送数据到TJC4024T032屏幕显示波形数据，和四个变量的数据
2、接收TJC4024T032下发的两个按键命令到key[2]

***********************************************/

#include "main.h"




int start_time = 0;   //定时时间    1个数表示半小时 最多到16，最大定时时间8小时
int timing_flag = 0;  //定时标志
int switch_flag = 0;  //切换风速标志




char key[key_number]={0,0,0,0,0,0,0};   //用于装按键标志位0：未按下，1：按下      key[4]确定是工作在自动还是手动档位
                                    //0~2装档位数据，3装关闭数据

char hex_END[]={0xff,0xff,0xff};   //发送到串口屏的结束串

int OUT_PWM = 1;       //装输出PWM的值，后面是减一输出的，这里必须是从1开始，否则减一，见到65535了



/*****************************************
函数功能:串口发送数据函数

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
函数功能：
发送三个波形到屏幕，不用则参数给0即可
ch0：粉色
ch1：黄色
ch2：蓝色

注：此程序无使用
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
函数功能：
发送四个不同数据到屏幕显示

*******************************/
void send_data(float C,float F)
{
  char str1[50]; 
  static int state = 0;
  switch(state)              //处理一个数据太长，分成2次处理
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
函数功能：查找str1中是否有str2的字符串，返回第一个相同字符的地址


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
函数功能:初始化屏幕显示参数


************************************************/
void main_disp_init(void)
{
  char str1[50];
  sprintf(str1,"b0.pic=7");                   //全部讲按键切换为关闭                //同步屏幕显示图标
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
  sprintf(str1,"page0.h0.val=%d",0);               //设置进度条值
  USARTx_Send(USART2,(u8 *)str1,strlen(str1));
  USARTx_Send(USART2,(u8 *)hex_END,3);
  
  sprintf(str1,"page0.t7.pco=31735");
  USARTx_Send(USART2,(u8 *)str1,strlen(str1));
  USARTx_Send(USART2,(u8 *)hex_END,3);
  
  sprintf(str1,"t7.txt=\"0:0:0\"");
  USARTx_Send(USART2,(u8 *)str1,strlen(str1));
  USARTx_Send(USART2,(u8 *)hex_END,3);
  
  sprintf(str1,"t1.txt=\"手动模式\"");      //初始化显示手动模式
  USARTx_Send(USART2,(u8 *)str1,strlen(str1));
  USARTx_Send(USART2,(u8 *)hex_END,3);
  
  auto_manual_LED = 1;    //自动手动指示灯关闭表示工作在手动模式下
  
}
/********************
函数功能：整个系统控制数据的处理


注：上位机发送的是KEYx*p
其中x为按键号，对应存入key[x]的数组，“*p”为发送的数据的针尾
占时只写了两个
注：jz：0~2表示1~3挡，3表示关闭
    4表示自动手动模式
    5、6表示加减定时时间
进度条发送格式：valxxxKEY*p 

**********************/
void get_key(void)
{
  // int i = 0;
   int flag = 0,flag1 = 0;
   char str1[50];
   char jz = 50;     //初始值50表示无效键值，必须是50，否则会影响其他地方
   static u8 last_remote_jz = 0;
   deal_remote_val();      //获取红外遥控数据 

  
   if(USART_RX_STA2&0x8000)            //处理屏幕接受的数据
   {
     flag = find_str(USART_RX_BUF2,"KEY");   //查找是否收到按键的帧头
     flag1 = find_str(USART_RX_BUF2,"val");   //查找是否收到进度条的帧头
     if(flag1 != -1)
     {
        OUT_PWM = 100*USART_RX_BUF2[flag1+3];
        if(OUT_PWM == 0) OUT_PWM = 1;
          switch_flag = 1;               //调整输出PWM标志
     }
     if(flag != -1)                  //将按键键值存入key中，手动/自动按键单独处理
     {
        key[0] = key[1] = key[2] = key[3] = 0; 
        jz = USART_RX_BUF2[flag+3] - 0x30;
        if(jz == 4)              //自动/手动功能键按下
        {
          key[4] = !key[4];  
          OUT_PWM=1; 
          switch_flag=1;
        }
        else if(jz < key_number)    
          key[jz] = 1;      
     }
     USART_RX_STA2 = 0;                           //清楚缓冲，为下次做准备
     memset(USART_RX_BUF2,0,strlen(USART_RX_BUF2));   
   }
   
   if(USART_RX_STA&0x8000)      //处理手机接收到的数据
   {
      flag = find_str(USART_RX_BUF,"KEY");
      key[0] = key[1] = key[2] = key[3] = 0; 
      jz = USART_RX_BUF[flag+3] - 0x30;
      if(jz < key_number)    
        key[jz] = 1;
                
      USART_RX_STA = 0;
      memset(USART_RX_BUF,0,strlen(USART_RX_BUF));   
   }   
   
   if( (remote_jz == 1 || remote_jz == 2 || remote_jz == 3 || remote_jz == 11 || remote_jz == 10))        //处理红外接受数据，处理函数在上面
   {
     key[0] = key[1] = key[2] = key[3] = 0; 
     switch(remote_jz)
     {
       case 1:key[0] = 1;jz = 0;break;
       case 2:key[1] = 1;jz = 1;break;
       case 3:key[2] = 1;jz = 2;break;
       case 11:key[3] = 1;jz = 3;break;    //'#'表示关闭按键
       case 10:key[4] = !key[4];OUT_PWM=1;switch_flag=1; break;  //'*'表示切换手动自动模式
     }
     last_remote_jz = remote_jz;
     remote_jz = 0;
   }
   
   
//以上获取上位机信息、以下处理上位机信息   

   if(key[4] == 0)   //手动模式下
   {
      sprintf(str1,"b4.pic=3");                   //相关指示标志
      USARTx_Send(USART2,(u8 *)str1,strlen(str1));
      USARTx_Send(USART2,(u8 *)hex_END,3);

      sprintf(str1,"t1.txt=\"手动模式\"");  
      USARTx_Send(USART2,(u8 *)str1,strlen(str1));
      USARTx_Send(USART2,(u8 *)hex_END,3); 

      auto_manual_LED = 1;    
      if(key[0] == 1 || key[1] == 1 || key[2] == 1 || key[3] == 1)    //任何一个调试按键按下都执行一次
      {
        sprintf(str1,"b0.pic=7");                   //全部讲按键切换为关闭                //同步屏幕显示图标
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
           
        sprintf(str1,"b%d.pic=8",jz);         //按下的按键切换图标    
        USARTx_Send(USART2,(u8 *)str1,strlen(str1));
        USARTx_Send(USART2,(u8 *)hex_END,3); 
        
        switch_flag = 1;                                     
      } 
       if(jz != 50)   //50无效键值
       {    
                   
          switch(jz)       
          {
            case 0:OUT_PWM = 2000;break;             //确定不同档位输出PWM值
            case 1:OUT_PWM = 5000;break;
            case 2:OUT_PWM = 8000;break;
            case 3:OUT_PWM = 1;break;     //关闭
            case 5:
                   if(OUT_PWM != 1)                 //电机停止状态不可以设置定时时间
                   {
                       timing_flag = 1;
                       start_time--;
                      if(start_time == -1)    //为0和为1时均减无效
                        start_time = timing_max;
                    }
                    break;
            case 6:
                   if(OUT_PWM != 1)
                   {                    
                      timing_flag = 1; 
                      start_time++;
                      if(start_time>timing_max)     //最多加10次
                        start_time = 0;
                   }
                   break;
          }  
          
          if(timing_flag == 1 && (jz == 5 || jz == 6))   //
          {              
            if(start_time == 0)                        //未定时状态，将字体设置为灰色
            {
              sprintf(str1,"page0.t7.pco=31735");           
              USARTx_Send(USART2,(u8 *)str1,strlen(str1));
              USARTx_Send(USART2,(u8 *)hex_END,3);
            }
            else
            {
              sprintf(str1,"page0.t7.pco=65535");     //未定时状态，将字体设置为白色
              USARTx_Send(USART2,(u8 *)str1,strlen(str1));
              USARTx_Send(USART2,(u8 *)hex_END,3);
            }
            
            sprintf(str1,"t7.txt=\"%d:%d:0\"",(start_time*timing)/60,(start_time*timing)%60);    //发送定时时间在屏幕上显示
            USARTx_Send(USART2,(u8 *)str1,strlen(str1));
            USARTx_Send(USART2,(u8 *)hex_END,3);
          }
          key[0] = key[1] = key[2] = key[3] = key[5]= key[6] = 0; 
          jz = 50;
        }
        
   }  
   else   //自动模式下
   {
      sprintf(str1,"b4.pic=4");                       //相关指示标志
      USARTx_Send(USART2,(u8 *)str1,strlen(str1));
      USARTx_Send(USART2,(u8 *)hex_END,3); 
      sprintf(str1,"t1.txt=\"自动模式\"");  
      USARTx_Send(USART2,(u8 *)str1,strlen(str1));
      USARTx_Send(USART2,(u8 *)hex_END,3);  
      
      auto_manual_LED = 0;               
     
     
       if(temperature/10.0  < expect_min)  //小于设置最低温度，关闭风扇
         OUT_PWM = 1;
       else
       {        
          OUT_PWM = 1000 + (temperature/10.0-expect_min)*7000.0/(expect_max-expect_min);   //计算自动模式下输出PWM
       }
       if(OUT_PWM >= 8000)    //最大输出PWM
         OUT_PWM = 7999;
       switch_flag = 1; 
   }
   
   if(switch_flag == 1)  //调整表示为1才调整一次输出PWM
   {
      sprintf(str1,"page0.h0.val=%d",OUT_PWM/100);
      USARTx_Send(USART2,(u8 *)str1,strlen(str1));
      USARTx_Send(USART2,(u8 *)hex_END,3);
      TIM_SetCompare1(TIM3,OUT_PWM-1); 
     
      switch_flag = 0;
   }
   if(timing_flag == 2 || OUT_PWM == 1)        //定时到或者输出PWM为1时，关闭输出，初始化参数
   {
     OUT_PWM = 1;
     timing_flag = 0;
     main_disp_init();
     TIM_SetCompare1(TIM3,OUT_PWM-1); 
   }
}





