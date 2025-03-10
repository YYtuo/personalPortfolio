#ifndef __USART_HMI_H__
#define __USART_HMI_H__

#ifdef STM32F40_41xxx
#include "stm32f4xx.h"
#else
#include "stm32f10x.h"
#endif



#define ORG_PPG		1
#define FILTER_PPG	2
#define PLUSE		4

#define WAVE_ON		0x80


#define END() 	(printf("\xff"),printf("\xff"),printf("\xff"))



typedef struct{
	uint8_t flag;
	uint8_t wave_state;
	
	
}UART_HMI_Str;


void UART_HMI_update(void* ctrlDat, UART_HMI_Str* uart_hmi_Dat);

#endif

