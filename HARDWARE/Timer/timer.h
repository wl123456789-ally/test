#ifndef _timer_H_
#define _timer_H_

#include "stm32f10x.h"
#include "stm32f10x.h"
#include "led.h"
#include "sys.h"


void TIMER_Init(u16 arr,u16 pros);

void TIM2_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 pros);
void TIM5_Int_Init(u16 arr,u16 psc);

#endif
