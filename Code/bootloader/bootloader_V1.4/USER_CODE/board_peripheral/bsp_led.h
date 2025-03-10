#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "BSP_GPIO.h"

///////////////////////////////////////////////////////////////////// 	


//LED端口定义
#define LED_R PCout(15)	
#define LED_G PCout(14)
#define LED_B PCout(13)






void LED_Init(void);//初始化		 				    
#endif
