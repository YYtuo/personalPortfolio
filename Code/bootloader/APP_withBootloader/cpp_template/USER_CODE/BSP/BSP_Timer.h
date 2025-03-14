#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

#include "stm32f4xx.h"
#include "stm32f4xx.h"
#include "stdlib.h"
#include "stdio.h"

#include "BSP_GPIO.h"

/*************************************************************
TIM1, TIM8->TIM11的时钟频率为APB2时钟的2倍，即168MHz
TIM2->TIM7, TIM12->TIM14的时钟频率为APB1时钟的2倍，即84MHz
*************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

typedef struct
{
	BSP_GPIOSource_TypeDef *ICAP_GPIO;	//输入捕获
	uint16_t TIM_ICPolarity;						//输入捕获极性：TIM_ICPolarity_Rising；TIM_ICPolarity_Falling；TIM_ICPolarity_BothEdge
}BSP_TIM_ICAP_TypeDef;

void BSP_TIM_RCC_Init(TIM_TypeDef* BSP_TIMx);
void BSP_TIM_Init(TIM_TypeDef* BSP_TIMx,uint32_t Period,uint16_t Prescaler);
void BSP_TIM_INT_Init(TIM_TypeDef* BSP_TIMx,uint32_t Period,uint16_t Prescaler,uint8_t PreemptionPriority,uint8_t SubPriority);
uint8_t	TIM_TO_GPIO_AF(TIM_TypeDef* BSP_TIMx);
void BSP_TIM_ICAP_Init(TIM_TypeDef* BSP_TIMx,uint32_t Period,uint16_t Prescaler,
											BSP_TIM_ICAP_TypeDef* ICAP1,
											BSP_TIM_ICAP_TypeDef* ICAP2,
											BSP_TIM_ICAP_TypeDef* ICAP3,
											BSP_TIM_ICAP_TypeDef* ICAP4,
											uint8_t PreemptionPriority, uint8_t SubPriority);
#ifdef __cplusplus
 }
#endif /* __cplusplus */
#endif
