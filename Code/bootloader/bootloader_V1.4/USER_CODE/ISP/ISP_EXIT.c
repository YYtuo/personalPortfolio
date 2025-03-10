#include "stm32f4xx_conf.h"
#include "BSP_GPIO.h"
#include "bsp_key.h" 
#include "bsp_led.h"
#include "delay.h"

void EXTI0_IRQHandler(void){   
	if(EXTI_GetITStatus(EXTI_Line0) != RESET){    
		EXTI_ClearFlag(EXTI_Line0);          
		EXTI_ClearITPendingBit(EXTI_Line0);
		
		/*********�������Զ��岿��**********/
	}
}

void EXTI1_IRQHandler(void){   
	if(EXTI_GetITStatus(EXTI_Line1) != RESET){    
		EXTI_ClearFlag(EXTI_Line1);          
		EXTI_ClearITPendingBit(EXTI_Line1);
		
		/*********�������Զ��岿��**********/
	}
}

void EXTI2_IRQHandler(void){   
	if(EXTI_GetITStatus(EXTI_Line2) != RESET){    
		EXTI_ClearFlag(EXTI_Line2);          
		EXTI_ClearITPendingBit(EXTI_Line2);
		
		/*********�������Զ��岿��**********/
		//������������ʶ�������������
		// delay_ms(5);
		if(KEY_FUN == 1){
			key_press_flag = 1;//�������±�־λ��1��ֻ��֪���а������£���֪�����ĸ�����Ҫ��һ���жϣ�	
//			LED_R = ~LED_R;
		}
		
	}
}

void EXTI3_IRQHandler(void){   
	if(EXTI_GetITStatus(EXTI_Line3) != RESET){    
		EXTI_ClearFlag(EXTI_Line3);          
		EXTI_ClearITPendingBit(EXTI_Line3);
		
		/*********�������Զ��岿��**********/
		// delay_ms(5);
		if(KEY_UP == 0){
			key_press_flag = 1;//�������±�־λ��1��ֻ��֪���а������£���֪�����ĸ�����Ҫ��һ���жϣ�	
//			LED_G = ~LED_G;
		}
	}
}

void EXTI4_IRQHandler(void){   
	if(EXTI_GetITStatus(EXTI_Line4) != RESET){    
		EXTI_ClearFlag(EXTI_Line4);          
		EXTI_ClearITPendingBit(EXTI_Line4);
		/*********�������Զ��岿��**********/
		// delay_ms(5);
		if(KEY_DOWN == 0){
			key_press_flag = 1;//�������±�־λ��1��ֻ��֪���а������£���֪�����ĸ�����Ҫ��һ���жϣ�	
//			LED_B = ~LED_B;
		}
	}
}

void EXTI9_5_IRQHandler(void) {
	//����ʵ��ʹ���޸�
	if(EXTI_GetITStatus(EXTI_Line7) != RESET){
		EXTI_ClearFlag(EXTI_Line7); 
		EXTI_ClearITPendingBit(EXTI_Line7);
		
		/*********�������Զ��岿��**********/
  }
}

void EXTI15_10_IRQHandler(void) {
	//����ʵ��ʹ���޸�
	if(EXTI_GetITStatus(EXTI_Line15) != RESET){
		EXTI_ClearFlag(EXTI_Line15); 
		EXTI_ClearITPendingBit(EXTI_Line15);
		
		/*********�������Զ��岿��**********/
	}
}

