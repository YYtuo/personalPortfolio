#include "bsp_beep.h"
#include "delay.h"

/*蜂鸣器连接在PB14引脚*/
// 共振频率4000Hz
void beep_Init(void){
	BSP_TIM_PWM_Init(BEEP_TIMER, 1, 84-1, BEEP_GPIO, NULL, NULL, NULL);
	BEEP_LOUDNESS = 0;
}
/*
***************************************************
函数名：beep_on
功能：蜂鸣器开启
入口参数：loundness：响度
			fre：音调（Hz）
			tone：谱号，如果要输入频率，这个传0
返回值：无
应用范围：外部调用
备注：无
***************************************************
*/
void beep_on(uint8_t loundness, uint16_t tone, uint16_t fre){
	BEEP_LOUDNESS = loundness;
	if(!tone){
		BEEP_TONE = TIM_FRE / fre;
	}
	else{
		BEEP_TONE = tone;
	}
}

/*
***************************************************
函数名：beep_stop
功能：蜂鸣器停止
入口参数：无
返回值：无
应用范围：外部调用
备注：无
***************************************************
*/
void beep_stop(void){
	BEEP_LOUDNESS = 0;
	BEEP_TONE = 1;
}

/**********************************
 * 函数名：beep_workState
 * 功能：不同状态下的蜂鸣器状态指示
 * 输入：状态列表
 * 输出：无
 * 备注：无
**********************************/
void beep_workState(uint8_t workState){
	switch(workState){
		case 0:;break;
		default:break;
	}
}

void beep_remoteEn(void){
	beep_on(20,C_H[1],0);
	delay_ms(300);

	beep_on(30,C_H[2],0);
	delay_ms(300);
	
	beep_on(40,C_H[3],0);
	delay_ms(300);
	
	beep_stop();
}

void beep_remoteDis(void){
	beep_on(40,C_H[3],0);
	delay_ms(300);

	beep_on(30,C_H[2],0);
	delay_ms(300);
	
	beep_on(20,C_H[1],0);
	delay_ms(300);
	
	beep_stop();
}

// /*
// ***************************************************
// 函数名：play_music
// 功能：蜂鸣器唱谱
// 入口参数：section:区间; 
// 			spectrum:谱;
// 返回值：无
// 应用范围：外部调用
// 备注：无
// ***************************************************
// */
// void play_music(uint8_t section, uint8_t spectrum){
// 	BEEP_LOUDNESS = 40;
// 	switch(section){
// 		case low:{
// 			BEEP_TONE = C_L[spectrum];
// 		}break;
		
// 		case mid:{
// 			BEEP_TONE = C_M[spectrum];
// 		}break;
		
// 		case high:{
// 			BEEP_TONE = C_H[spectrum];
// 		}break;
// 	}
// }










