#include "bsp_key.h"
#include "delay.h" 

key_parameter_TypeDef fun_key_parameter;
key_parameter_TypeDef up_key_parameter;
key_parameter_TypeDef down_key_parameter;

uint8_t key_press_flag;//任意按键按下标志,在外部中断中进行赋值
uint8_t keyScan_delay_En;//延迟扫描使能标志位
uint16_t GetKeyValue_delayTime_ms;//按键扫描周期计时器

//按键初始化函数
void KEY_Init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//GPIOE时钟

	/*按键引脚初始化*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4; //KEY_UP KEY_DOWN对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE3,4

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//WK_UP对应引脚PE2
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2
	/* end of 按键引脚初始化 */

	/*外部中断配置初始化*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 连接到中断线2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);//PE3 连接到中断线3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);//PE4 连接到中断线4

	/* 配置EXTI_Line2,3,4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置

	EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置

	// 三个按键的中断优先级相等，
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//外部中断3
	NVIC_Init(&NVIC_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//外部中断4
	NVIC_Init(&NVIC_InitStructure);//配置
	/*end of 外部中断配置初始化*/

	/*按键参数结构体初始化*/
	fun_key_parameter.keyGPIO 	= GPIOE;
	up_key_parameter.keyGPIO 	= GPIOE;
	down_key_parameter.keyGPIO 	= GPIOE;
	
	fun_key_parameter.keyGPIO_Pin 	= GPIO_Pin_2;
	up_key_parameter.keyGPIO_Pin 	= GPIO_Pin_3;
	down_key_parameter.keyGPIO_Pin 	= GPIO_Pin_4;

	fun_key_parameter.keytype = P_KEY;
	up_key_parameter.keytype = N_KEY;
	down_key_parameter.keytype = N_KEY;
	/*end of 按键参数结构体初始化*/

} 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，WKUP按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP!!
//u8 KEY_Scan(u8 mode){ 
// 	static u8 key_up=1;//按键按松开标志
// 	if(mode)key_up=1;  //支持连按		  
// 	if(key_up && (KEY_UP==0||KEY_DOWN==0||KEY_FUN==1))
// 	{
// 		// delay_ms(10);//去抖动 
// 		key_up=0;
// 		if(KEY_UP==0)return KEY_UP_PRES;
// 		else if(KEY_DOWN==0)return KEY_DOWN_PRES;
// 		else if(KEY_FUN==1)return KEY_FUN_PRES;

// 	}else if(KEY_UP==1 && KEY_DOWN==1&&KEY_FUN==0)key_up=1; 	    
//  	return 0;// 无按键按下
//}

/********************************************
 * 函	数	名：judge_KeyValue
 * 功		能：按键键值和状态判断函数
 * 输		入：按键参数结构体
 * 输		出：无
 * 备		注：使用此函数，需要先说明各个按键的GPIO,pin
 * 				按键按下时间和键值：
 * 				2S：				进入连按模式
 * 				1500ms：				长按
 * 				1200ms内多次按下： 	多击
********************************************/
void judge_KeyValue(key_parameter_TypeDef * keyValue){
	// 判断按键类型
	uint8_t KEY_PRESSED,KEY_UNPRESSED;
	KEY_PRESSED = (keyValue->keytype == N_KEY) ? N_KEY_PRESSED : P_KEY_PRESSED;
	KEY_UNPRESSED = (keyValue->keytype == N_KEY) ? N_KEY_UNPRESSED : P_KEY_UNPRESSED;
	
	//键处理
	/*判断是否在长按*/
	//如果按键处于按下状态
	if(GPIO_ReadInputDataBit(keyValue->keyGPIO, keyValue->keyGPIO_Pin) == KEY_PRESSED){
		if(keyValue->key_event._if_keyPress == 0){
			keyValue->key_event._if_keyPress = 1;
			keyValue->press_time_ms = 0;		//长按计时开始
		}
		//按下1s，进入连按模式
		else if(keyValue->key_event._if_keyPress == 1 && keyValue->press_time_ms >= 2000){
			keyValue->key_event.isLongPressing = 1;	
		}
	}
	
	/*如果按键处于弹起状态状态并且之前有被按下，则将根据按下时间确定键值*/
	else if( (GPIO_ReadInputDataBit(keyValue->keyGPIO, keyValue->keyGPIO_Pin)==KEY_UNPRESSED) && 
				keyValue->key_event._if_keyPress == 1)
	{
		keyValue->key_event._if_keyPress = 0;
		keyValue->key_event.isLongPressing = 0;	
		if(keyValue->press_time_ms >= 1500){
			keyValue->key_event.longPressed = 1;
			keyValue->key_event.shortPressed = 0;
			keyValue->key_event.doublePressed = 0;
		}
		
		/*若已经发生一次短按事件，再次短按，发生双击事件*/
		else if(keyValue->press_count == 1 && keyValue->next_press_time_ms <= 1200){
			keyValue->press_count = 0;
			keyValue->next_press_timeEn = 0;	//停止 按键再次按下的计时
			
			keyValue->key_event.longPressed = 0;
			keyValue->key_event.shortPressed = 0;
			keyValue->key_event.doublePressed = 1;
		}
		else{
			/*清空下一次按键按下间隔计时器，开始计时*/
			/*若超过250ms没有按键按下，在定时器中断中判定为单按，并停止next_press_time_ms的计时*/
			keyValue->next_press_time_ms = 0;
			keyValue->next_press_timeEn = 1;

			keyValue->press_count++;
		}
		key_press_flag = 0;	
//		myKey_ValueChangedFlag = 1;		//键值改变标志位
	}
}

/********************************************
 * 函	数	名：set_singlePress
 * 功		能：判定按键为单击
 * 输		入：按键参数结构体
 * 输		出：无
 * 备		注：在定时器中断中调用
********************************************/
void set_singlePress(key_parameter_TypeDef* key_parameter){
	if(key_parameter->next_press_timeEn){
		key_parameter->next_press_time_ms++;
		//超过250ms没有按键再次按下，停止计时
		if(key_parameter->next_press_time_ms >= 250){
			key_parameter->next_press_timeEn = 0;
			//判定为单击
			key_parameter->press_count = 0;

			key_parameter->key_event.longPressed = 0;
			key_parameter->key_event.shortPressed = 1;
			key_parameter->key_event.doublePressed = 0;
		}
	}
}
/********************************************
*函	数	名：GetKeyValue
*功		能：获取按下按键键值和状态
*输		入：按键扫描周期（单位ms，若不需要延迟，则传入NULL）
*输		出：无
*备		注：处理完后，记得对按键参数标志位进行复位
********************************************/
void GetKeyValue(uint16_t GetKeyValue_delayTime){
	//标志位为1时，开始延迟扫描计数
	if(GetKeyValue_delayTime){
		 keyScan_delay_En = 1;
	}
	else {
		keyScan_delay_En = 0;
		GetKeyValue_delayTime_ms = 0;
	}
	
	//判断如果有按键被按下了 ，并且按键扫描周期到达相应时间了则开始扫描按键
	if(key_press_flag == 1 && GetKeyValue_delayTime_ms >= GetKeyValue_delayTime){
		/*此处增加你的按键*/
		judge_KeyValue(&fun_key_parameter);
		judge_KeyValue(&up_key_parameter);
		judge_KeyValue(&down_key_parameter);
		
		GetKeyValue_delayTime_ms = 0;//清除按键扫描周期计数	
	}
}





















