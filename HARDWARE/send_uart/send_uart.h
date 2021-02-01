#ifndef __SEND_UART_H
#define __SEND_UART_H	 
#include "sys.h"



void dfs_init(void);
void main_disp_init(void);
void TJC4_init(int baud);
void send_wave(int ch0,int ch1,int ch2);
void send_data(float F,float C);
void get_key(void);
void deal_key_val(int flag);



#endif


