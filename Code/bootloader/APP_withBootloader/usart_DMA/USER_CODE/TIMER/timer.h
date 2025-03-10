#ifndef _TIMER_H
#define _TIMER_H

#include "BSP_GPIO.h"

extern uint8_t PWM;//pwm总数
extern uint8_t pwm_R,pwm_G,pwm_B;//灯pwm占空比

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM6_Int_Init(u16 arr,u16 psc);


void TIM_Init(void);
void pwm_output_init(void);
void disable_output(uint8_t motor_num);



void TIM1_Config(u16 TIM1_Prescaler, u16 TIM1_Period, u16 CCR_A, u16 DIR_A);
void TIM2_Config(u32 PulseNum_A);
void PWM_Output_X(u16 Cycle_A, u32 PulseNum_A);
//void TIM2_IRQHandler(void);

// Group B
void TIM4_GPIO_Config(u16 TIM4_Prescaler, u16 TIM4_Period, u16 CCR_B, u16 DIR_B);
void TIM8_GPIO_Config(u32 PulseNum_B);
void PWM_Output_Y(u16 Cycle_B, u32 PulseNum_B);
//void TIM8_UP_TIM13_IRQHandler(void);

// Group C
void TIM3_Config(u16 TIM3_Prescaler, u16 TIM3_Period, u16 CCR_C, u16 DIR_C);
void TIM5_Config(u32 PulseNum_C);
void PWM_Output_Z(u16 Cycle_C, u32 PulseNum_C);
//void TIM5_IRQHandler(void);

#endif
