#include "bsp_beep.h"
#include "delay.h"

/*������������PB14����*/
// ����Ƶ��4000Hz
void beep_Init(void){
	BSP_TIM_PWM_Init(BEEP_TIMER, 1, 84-1, BEEP_GPIO, NULL, NULL, NULL);
	BEEP_LOUDNESS = 0;
}
/*
***************************************************
��������beep_on
���ܣ�����������
��ڲ�����loundness�����
			fre��������Hz��
			tone���׺ţ����Ҫ����Ƶ�ʣ������0
����ֵ����
Ӧ�÷�Χ���ⲿ����
��ע����
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
��������beep_stop
���ܣ�������ֹͣ
��ڲ�������
����ֵ����
Ӧ�÷�Χ���ⲿ����
��ע����
***************************************************
*/
void beep_stop(void){
	BEEP_LOUDNESS = 0;
	BEEP_TONE = 1;
}

/**********************************
 * ��������beep_workState
 * ���ܣ���ͬ״̬�µķ�����״ָ̬ʾ
 * ���룺״̬�б�
 * �������
 * ��ע����
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
// ��������play_music
// ���ܣ�����������
// ��ڲ�����section:����; 
// 			spectrum:��;
// ����ֵ����
// Ӧ�÷�Χ���ⲿ����
// ��ע����
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










