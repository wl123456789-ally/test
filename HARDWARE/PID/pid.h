#ifndef _PID_H_
#define _PID_H_

#include "stm32f10x.h"
#include "stm32f10x.h"
#include "led.h"
#include "sys.h"



void PID_Init(void);
float PID_Realize(float speed);

#endif
