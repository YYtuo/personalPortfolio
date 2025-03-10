#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

extern uint8_t PWM;//pwm总数
extern uint8_t pwm_R,pwm_G,pwm_B;//灯pwm占空比

void TIM3_Int_Init(u16 arr,u16 psc);

#endif
