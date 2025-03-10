#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#ifndef __DELAY_H
#define __DELAY_H 			   

#include "stm32f4xx.h"

void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */






























