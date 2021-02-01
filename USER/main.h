#ifndef _main_H_
#define _main_H_

#include "stm32f10x.h"
#include "led.h"
#include "sys.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "string.h"
#include "math.h"
#include "send_uart.h"
#include "dht11.h"
#include "ds18b20.h"
#include "remote.h"
#include "timer.h"
#include "pid.h"



#define timing_max 16   //最大定时时间
#define timing 20    //按键按下一次，增加或者减少定时分钟数
#define key_number 7  //屏幕按键数量
 
#define expect_max  20   //自动模式下温度自动调整范围   
#define expect_min  10



extern char key[key_number];  //装按键键值

extern u8  USART_RX_BUF2[200]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA2;         		//接收状态标记	

extern u8  USART_RX_BUF[200]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
extern float temperature;      //装环境温度

extern int start_time;         //记录定时按下次数
extern int timing_flag;        //定时标志位




#endif
