#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

extern uint8_t PWM;//pwm����
extern uint8_t pwm_R,pwm_G,pwm_B;//��pwmռ�ձ�

void TIM3_Int_Init(u16 arr,u16 psc);

#endif
