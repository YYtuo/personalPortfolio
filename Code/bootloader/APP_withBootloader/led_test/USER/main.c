#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "key.h"

int main(void){ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	LED_Init();				//��ʼ��LED�˿�
	KEY_Init();
	
 	TIM3_Int_Init(100-1,840-1);	//��ʱ��ʱ��84M����Ƶϵ��840������84M/840=100Khz�ļ���Ƶ�ʣ�����100��Ϊ1ms

	while(1){
		switch(KEY_Scan(1)){//��֧������
			case 0:LED_B=1,LED_R=1,LED_G=1;break;
			case 1:LED_B=!LED_B;break;
			case 2:LED_R=!LED_R;break;
			case 3:LED_G=!LED_G;break;
		}
	}
}
