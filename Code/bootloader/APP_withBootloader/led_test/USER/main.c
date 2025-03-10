#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "key.h"

int main(void){ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	LED_Init();				//初始化LED端口
	KEY_Init();
	
 	TIM3_Int_Init(100-1,840-1);	//定时器时钟84M，分频系数840，所以84M/840=100Khz的计数频率，计数100次为1ms

	while(1){
		switch(KEY_Scan(1)){//不支持连按
			case 0:LED_B=1,LED_R=1,LED_G=1;break;
			case 1:LED_B=!LED_B;break;
			case 2:LED_R=!LED_R;break;
			case 3:LED_G=!LED_G;break;
		}
	}
}
