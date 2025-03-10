#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H

#include "BSP_GPIO.h"
#include "BSP_PWM.h"

#define IC_FRE			84000000UL
#define TIM_PSC			84
#define TIM_FRE			IC_FRE/TIM_PSC		//蜂鸣器控制定时器分频后时钟频率
#define LOUDNESS_PAR	40					//蜂鸣器响度

#define BEEP_TIMER		TIM12				//蜂鸣器控制定时器
#define BEEP_GPIO		BSP_GPIOB14			//PWM输出引脚

#define BEEP_TONE		BEEP_TIMER->ARR		//BEEP_TONE = IC_FRE/待设置频率 - 1 
#define BEEP_LOUDNESS  	BEEP_TIMER->CCR1	


enum {
	low=1,
	mid,
	high
};

static const uint16_t C_L[] = {1,3816,3401,3030,2865,2551,2272,2024};
static const uint16_t C_M[] = {1,1912,1703,1517,1432,1275,1136,1012};
static const uint16_t C_H[] = {1,955,851,758,721,637,568,508};

void beep_Init(void);
void beep_on(uint8_t loundness, uint16_t tone, uint16_t fre);
void beep_stop(void);


void beep_remoteEn(void);
void beep_remoteDis(void);
void play_music(uint8_t section, uint8_t spectrum);


#endif

