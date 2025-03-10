#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#ifndef __BSP_KEY_H
#define __BSP_KEY_H	 

#include "BSP_GPIO.h"

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
#define KEY_FUN 	    PEin(2)		//PE2

#define KEY_DOWN_PRES	1
#define KEY_UP_PRES 	2
#define KEY_FUN_PRES   3

// 按键类型，N_KEY：按下输入低电平，P_KEY：按下输入高电平
typedef enum {
    N_KEY = 0,
    P_KEY = 1
}KeyType;

typedef enum {
	N_KEY_PRESSED = 0,
	N_KEY_UNPRESSED = 1,

	P_KEY_PRESSED = 1,
	P_KEY_UNPRESSED = 0
}KeyState;

typedef struct keyEvent{
    uint8_t _if_keyPress:1;     //记录本按键的状态
	uint8_t shortPressed:1;
	uint8_t doublePressed:1;
	uint8_t longPressed:1;
	uint8_t isLongPressing:1;
    uint8_t :0;
}keyEvent_TypeDef;

typedef struct key_parameter{
    /*需要用户初始化的参数*/
	GPIO_TypeDef * keyGPIO;		//按键GPIO
	uint16_t keyGPIO_Pin;		//按键GPIO_pin
    KeyType keytype:1;          //记录按键类型
    /*end of 需要用户初始化的参数*/

    keyEvent_TypeDef key_event;
    // uint8_t longPressed:1;		//长按标志位
	// uint8_t shortPressed:1;		//短按标志位
	// uint8_t doublePressed:1;		//双击标志位
	// uint8_t isLongPressing:1;		//持续长按标志位
    // uint8_t :0;
	// KeyState keyState;		//记录本按键的状态
	uint8_t next_press_timeEn;	//按键再次按下计时使能，可降级为bit类型
	uint8_t press_count;		//记录按键按下次数
	uint16_t press_time_ms;		//记录第一次按键按下时间
	uint16_t next_press_time_ms;//记录再次按键按下时间	
//	keyEvent_enum key_event;			//按键事件寄存器，7-4：保留； 3：持续长按事件；2：长按事件； 1；双击事件；0：单击事件

}key_parameter_TypeDef;

extern key_parameter_TypeDef fun_key_parameter;
extern key_parameter_TypeDef up_key_parameter;
extern key_parameter_TypeDef down_key_parameter;

extern uint8_t key_press_flag;//任意按键按下标志
extern uint8_t keyScan_delay_En;//延迟扫描使能标志位
extern uint16_t GetKeyValue_delayTime_ms;//按键扫描周期计时器

void KEY_Init(void);	//IO初始化
void set_singlePress(key_parameter_TypeDef* key_parameter);
void GetKeyValue(uint16_t GetKeyValue_delayTime);

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */



