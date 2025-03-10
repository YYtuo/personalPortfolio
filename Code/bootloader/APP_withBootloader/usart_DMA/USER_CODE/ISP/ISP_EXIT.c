#include "stm32f4xx_conf.h"
#include "BSP_GPIO.h"
#include "bsp_key.h" 
#include "bsp_led.h"
#include "delay.h"

void EXTI0_IRQHandler(void){   
	if(EXTI_GetITStatus(EXTI_Line0) != RESET){    
		EXTI_ClearFlag(EXTI_Line0);          
		EXTI_ClearITPendingBit(EXTI_Line0);
		
		/*********以下是自定义部分**********/
	}
}

void EXTI1_IRQHandler(void){   
	if(EXTI_GetITStatus(EXTI_Line1) != RESET){    
		EXTI_ClearFlag(EXTI_Line1);          
		EXTI_ClearITPendingBit(EXTI_Line1);
		
		/*********以下是自定义部分**********/
	}
}

void EXTI2_IRQHandler(void){   
	if(EXTI_GetITStatus(EXTI_Line2) != RESET){    
		EXTI_ClearFlag(EXTI_Line2);          
		EXTI_ClearITPendingBit(EXTI_Line2);
		
		/*********以下是自定义部分**********/
		//若按键出现误识别，在这里加消抖
		// delay_ms(5);
		if(KEY_FUN == 1){
			key_press_flag = 1;//按键按下标志位置1（只是知道有按键按下，不知道是哪个，需要进一步判断）	
//			LED_R = ~LED_R;
		}
		
	}
}

void EXTI3_IRQHandler(void){   
	if(EXTI_GetITStatus(EXTI_Line3) != RESET){    
		EXTI_ClearFlag(EXTI_Line3);          
		EXTI_ClearITPendingBit(EXTI_Line3);
		
		/*********以下是自定义部分**********/
		// delay_ms(5);
		if(KEY_UP == 0){
			key_press_flag = 1;//按键按下标志位置1（只是知道有按键按下，不知道是哪个，需要进一步判断）	
//			LED_G = ~LED_G;
		}
	}
}

void EXTI4_IRQHandler(void){   
	if(EXTI_GetITStatus(EXTI_Line4) != RESET){    
		EXTI_ClearFlag(EXTI_Line4);          
		EXTI_ClearITPendingBit(EXTI_Line4);
		/*********以下是自定义部分**********/
		// delay_ms(5);
		if(KEY_DOWN == 0){
			key_press_flag = 1;//按键按下标志位置1（只是知道有按键按下，不知道是哪个，需要进一步判断）	
//			LED_B = ~LED_B;
		}
	}
}

void EXTI9_5_IRQHandler(void) {
	//根据实际使用修改
	if(EXTI_GetITStatus(EXTI_Line7) != RESET){
		EXTI_ClearFlag(EXTI_Line7); 
		EXTI_ClearITPendingBit(EXTI_Line7);
		
		/*********以下是自定义部分**********/
  }
}

void EXTI15_10_IRQHandler(void) {
	//根据实际使用修改
	if(EXTI_GetITStatus(EXTI_Line15) != RESET){
		EXTI_ClearFlag(EXTI_Line15); 
		EXTI_ClearITPendingBit(EXTI_Line15);
		
		/*********以下是自定义部分**********/
	}
}

