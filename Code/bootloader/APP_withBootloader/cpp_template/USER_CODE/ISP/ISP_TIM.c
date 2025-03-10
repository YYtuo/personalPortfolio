#include "timer.h"
#include "bsp_key.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

volatile int32_t TIM3_ms_counter = 0;
volatile int32_t TIM3_s_counter = 0;
uint8_t keyVal = 0;
/*
***************************************************
函数名：TIM2_IRQHandler
功能：定时器2中断
备注：
***************************************************
*/
void TIM2_IRQHandler(void)
{
	/*************定时中断****************/
	if (TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET) 
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		 
		/*********以下是自定义部分**********/
		GetKeyValue(0);
	}
}

/*
***************************************************
函数名：TIM3_IRQHandler
功能：定时器3中断
备注：1ms进一次中断
***************************************************
*/
#include "usbh_usr.h"
void TIM3_IRQHandler(void){
	/*************定时中断****************/
	if (TIM_GetITStatus(TIM3,TIM_IT_Update)!= RESET) 
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		 
		/*********以下是自定义部分**********/
//		TIM3_ms_counter++;
//		if(TIM3_ms_counter % 1000 ==0){
//			TIM3_s_counter++;
//		}

//		if(keyScan_delay_En) GetKeyValue_delayTime_ms++;//按键扫描延迟计时器计时更新

//		if(fun_key_parameter.key_event._if_keyPress == 1) fun_key_parameter.press_time_ms++;//按键按下计时器计时更新
//		if(up_key_parameter.key_event._if_keyPress == 1) up_key_parameter.press_time_ms++;
//		if(down_key_parameter.key_event._if_keyPress == 1) down_key_parameter.press_time_ms++;
//		
//		set_singlePress(&fun_key_parameter);
//		set_singlePress(&up_key_parameter);
//		set_singlePress(&down_key_parameter);
//		
////		#ifdef REMOTE
////		USBH_Process(&USB_OTG_Core_dev, &USB_Host);

////		#endif
		
	}
}

/*
***************************************************
函数名：TIM4_IRQHandler
功能：定时器4中断
备注：
***************************************************
*/
void TIM4_IRQHandler(void)
{
	/*************定时中断****************/
	if (TIM_GetITStatus(TIM4,TIM_IT_Update)!= RESET) 
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
		 
		/*********以下是自定义部分**********/
		
	}
}

/*
***************************************************
函数名：TIM5_IRQHandler
功能：定时器5中断
备注：
***************************************************
*/
//void TIM5_IRQHandler(void)
//{
//	/*************定时中断****************/
//	if (TIM_GetITStatus(TIM5,TIM_IT_Update)!= RESET) 
//	{
//		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
//		TIM_ClearFlag(TIM5, TIM_FLAG_Update);
//		/*********以下是自定义部分**********/	

//	}
//}

/*
***************************************************
函数名：TIM6_DAC_IRQHandler
功能：定时器6中断
备注：
***************************************************
*/

#include "oled.h"
void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
		TIM_ClearFlag(TIM6, TIM_FLAG_Update);
		
		/*********以下是自定义部分**********/
		TIM3_ms_counter++;
		if(TIM3_ms_counter % 1000 ==0){
			TIM3_s_counter++;
		}
//		oled_ShowStr(0,0,"abc",small);
		if(keyScan_delay_En) GetKeyValue_delayTime_ms++;//按键扫描延迟计时器计时更新

		if(fun_key_parameter.key_event._if_keyPress == 1) fun_key_parameter.press_time_ms++;//按键按下计时器计时更新
		if(up_key_parameter.key_event._if_keyPress == 1) up_key_parameter.press_time_ms++;
		if(down_key_parameter.key_event._if_keyPress == 1) down_key_parameter.press_time_ms++;
		
		set_singlePress(&fun_key_parameter);
		set_singlePress(&up_key_parameter);
		set_singlePress(&down_key_parameter);
		
//		#ifdef REMOTE
//		USBH_Process(&USB_OTG_Core_dev, &USB_Host);

//		#endif

	}
}

/*
***************************************************
函数名：TIM7_IRQHandler
功能：定时器7中断0.1ms进一次
备注：
***************************************************
*/
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
		TIM_ClearFlag(TIM7, TIM_FLAG_Update);
		/*********以下是自定义部分**********/

	}
}

/*
***************************************************
函数名：TIM11_IRQHandler
功能：定时器11中断
备注：
***************************************************
*/
void TIM11_IRQHandler(void)
{
	/*************定时中断****************/
	if (TIM_GetITStatus(TIM11,TIM_IT_Update)!= RESET) 
	{
		TIM_ClearITPendingBit(TIM11,TIM_IT_Update);
		TIM_ClearFlag(TIM11, TIM_FLAG_Update);
		/*********以下是自定义部分**********/
		
	}         
}

void TIM1_CC_IRQHandler(void)
{
	
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
	
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		TIM_ClearFlag(TIM1, TIM_FLAG_Update);	

	}
}

void TIM8_BRK_TIM12_IRQHandler(void)
{

}

//void TIM8_UP_TIM13_IRQHandler(void)
//{
//	
//}

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

