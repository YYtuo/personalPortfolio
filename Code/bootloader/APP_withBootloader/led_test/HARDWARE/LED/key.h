#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
 

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
/*
#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0
*/

/*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/

#define KEY_UP 		PEin(3)   	//PE3
#define KEY_DOWN	PEin(4)		//PE4 
#define WK_UP 	    PEin(2)		//PE2


#define KEY_UP_PRES 	1
#define KEY_DOWN_PRES	2
#define WKUP_PRES   3

void KEY_Init(void);	//IO��ʼ��
u8 KEY_Scan(u8);  		//����ɨ�躯��	

#endif
