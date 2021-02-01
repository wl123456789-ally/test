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



#define timing_max 16   //���ʱʱ��
#define timing 20    //��������һ�Σ����ӻ��߼��ٶ�ʱ������
#define key_number 7  //��Ļ��������
 
#define expect_max  20   //�Զ�ģʽ���¶��Զ�������Χ   
#define expect_min  10



extern char key[key_number];  //װ������ֵ

extern u8  USART_RX_BUF2[200]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA2;         		//����״̬���	

extern u8  USART_RX_BUF[200]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern float temperature;      //װ�����¶�

extern int start_time;         //��¼��ʱ���´���
extern int timing_flag;        //��ʱ��־λ




#endif
