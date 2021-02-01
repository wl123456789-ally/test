/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"

#define DHT11_IO_IN()  DHT11_INMode()
#define DHT11_IO_OUT() DHT11_OUTMode()

#define DHT11_IO_H()   GPIO_SetBits(GPIOC, GPIO_Pin_4 );  	       
#define DHT11_IO_L() 	 GPIO_ResetBits(GPIOC, GPIO_Pin_4 ); 	   

////IO��������											   
#define	DHT11_DQ_OUT PGout(11) //���ݶ˿�	PG11 
//#define	DHT11_DQ_IN  PGin(11)  //���ݶ˿�	PG11
#define DHT11_DQ_IN 			GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)/////////////////


#define u8 unsigned char


u8 DHT11_Init(void);//��ʼ��DHT11
u8 DHT11_Read_Data(float *temp,float *humi);//��ȡ��ʪ��
u8 DHT11_Read_Byte(void);//����һ���ֽ�
u8 DHT11_Read_Bit(void);//����һ��λ
u8 DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11    

void DHT11_OUTMode(void);
void DHT11_INMode(void);     
	 
	 
	 
#endif

