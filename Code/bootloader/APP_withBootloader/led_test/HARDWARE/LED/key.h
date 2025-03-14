#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
 

/*下面的方式是通过直接操作库函数方式读取IO*/
/*
#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0
*/

/*下面方式是通过位带操作方式读取IO*/

#define KEY_UP 		PEin(3)   	//PE3
#define KEY_DOWN	PEin(4)		//PE4 
#define WK_UP 	    PEin(2)		//PE2


#define KEY_UP_PRES 	1
#define KEY_DOWN_PRES	2
#define WKUP_PRES   3

void KEY_Init(void);	//IO初始化
u8 KEY_Scan(u8);  		//按键扫描函数	

#endif
