#include "timer.h"
#include "bsp_led.h"
#include "tmc2209.h"

uint8_t PWM;//pwm总数

uint8_t pwm_R,pwm_G,pwm_B;//灯pwm预分频

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM6_Int_Init(u16 arr,u16 psc){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6|RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM3时钟

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);//初始化TIM3
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM6,ENABLE); //使能定时器3
	
	
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM2
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器2更新中断
	TIM_Cmd(TIM2,ENABLE); //使能定时器2
}

void pwm_output_init(void){
	//X轴定时器组初始化
	TIM1_Config(168, 100, 50, 0);		//168分频，得到1MHz，1M/100 = 10KHz 	50占空比
	TIM2_Config(0);
	//Y轴定时器组初始化
	TIM4_GPIO_Config( 84, 100, 50, 0);	//84分频，得到1MHz，1M/100 = 10KHz		50占空比
	TIM8_GPIO_Config(0);
	//Z轴定时器组初始化
	TIM3_Config( 84, 2000, 50, 0);		//84分频，得到1MHz，1M/100 = 10KHz		50占空比
	TIM5_Config(0);
}

//void disable_output(uint8_t motor_num){
//	switch(motor_num){
//		case _x:
//			TIM_CtrlPWMOutputs( TIM1, DISABLE);		// 高级定时器 TIM1 关闭MOE
//			TIM_Cmd( TIM1, DISABLE);			// 关闭定时器1
//			TIM_Cmd( TIM2, DISABLE);			// 关闭定时器2	
//			TIM_ITConfig( TIM2, TIM_IT_Update, DISABLE);
//			busy_reg &= 0xFE;//清除x电机繁忙状态
//		break;
//		
//		case _y:
//			TIM_Cmd( TIM4, DISABLE);			// 关闭定时器4
//			TIM_Cmd( TIM8, DISABLE);			// 关闭定时器8
//			TIM_ITConfig( TIM8, TIM_IT_Update, DISABLE);
//			busy_reg &= 0xFD;//清除y电机繁忙状态
//		break;
//		
//		case _z:
//			TIM_Cmd( TIM3, DISABLE);			// 关闭定时器3
//			TIM_Cmd( TIM5, DISABLE);			// 关闭定时器5
//			TIM_ITConfig( TIM5, TIM_IT_Update, DISABLE);
//			busy_reg &= 0xFB;//清除z电机繁忙状态
//		break;
//	}
//}
/*********** X ***********/
// 定时器1主模式
void TIM1_Config(u16 TIM1_Prescaler, u16 TIM1_Period, u16 CCR_A, u16 DIR_A){
	GPIO_InitTypeDef	GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOE, ENABLE);		
	
	GPIO_PinAFConfig( GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	GPIO_PinAFConfig( GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;		// TIM1_CH1 - PE13， CH4 - PE14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// 复用
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// 上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = TIM1_Period - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM1_Prescaler - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			// 重复计数，一定要 = 0
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	// 设置工作模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;							// PWM1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	// 比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;			// 输出极性
	// PWM通道，TIM1 - 通道1设置函数，50/100
	TIM_OCInitStructure.TIM_Pulse = CCR_A;								// 设置待装入捕获寄存器的脉冲值
	TIM_OC3Init( TIM1, &TIM_OCInitStructure);
	TIM_SelectMasterSlaveMode( TIM1, TIM_MasterSlaveMode_Enable);
	TIM_SelectOutputTrigger( TIM1, TIM_TRGOSource_Update);
	TIM_OC3PreloadConfig( TIM1, TIM_OCPreload_Enable);
	// PWM通道，TIM1 - 通道4设置函数，100/100 or 0/100
	TIM_OCInitStructure.TIM_Pulse = DIR_A;								// 初始化 TIM1-OC4
	TIM_OC4Init( TIM1, &TIM_OCInitStructure);						// CH4预装载使能，修改
	TIM_OC4PreloadConfig( TIM1, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1, ENABLE);
}

//定时器2从模式
void TIM2_Config(u32 PulseNum_A){
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = PulseNum_A;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM2, &TIM_TimeBaseStructure);
	
	TIM_SelectInputTrigger( TIM2, TIM_TS_ITR0);			// TIM1-主，TIM2-从
	TIM_SelectSlaveMode( TIM2, TIM_SlaveMode_External1);
	TIM_ITConfig( TIM2, TIM_IT_Update, DISABLE);
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure);
}

/*
***************************************************
函数名：PWM_Output_X
功能：设置X轴电机脉冲参数
入口参数：	Cycle_A 脉冲频率
返回值：成功返回1
应用范围：外部调用
备注：
***************************************************
*/
void PWM_Output_X(u16 Cycle_A, u32 PulseNum_A){
//	TIM2_Config(PulseNum_A);
	TIM2 -> ARR = PulseNum_A - 1;
	TIM_Cmd( TIM2, ENABLE);
	TIM_ClearITPendingBit( TIM2, TIM_IT_Update);
	TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE);
	
	TIM1 -> ARR = Cycle_A - 1;
	TIM_Cmd( TIM1, ENABLE);
	TIM_CtrlPWMOutputs( TIM1, ENABLE);	// 高级定时器 TIM1 使能 MOE
}
/*********** Group B ***********/
// 定时器4主模式
void TIM4_GPIO_Config(u16 TIM4_Prescaler, u16 TIM4_Period, u16 CCR_B, u16 DIR_B)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	// TIM3通道1\2 - PWM Z轴步进电机脉冲信号
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE);			
	
	GPIO_PinAFConfig( GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);		
	GPIO_PinAFConfig( GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;			// TIM3_CH1 PA6, CH2 - PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);		
	
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	// 时钟频率设置
	TIM_TimeBaseStructure.TIM_Period = TIM4_Period - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM4_Prescaler - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM4, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	// 设置工作模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			// 设置工作模式是PWM，且为PWM1工作模式，TIMx_CNT<TIMx_CCR1时为高电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		// 也就是使能PWM输出到端口					
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;			// 输出极性
	// PWM通道，TIM3 - 通道1设置函数，50/100
	TIM_OCInitStructure.TIM_Pulse = CCR_B;					// 设置待装入捕获寄存器的脉冲值
	TIM_OC1Init( TIM4, &TIM_OCInitStructure);						// 初始化 TIM3-OC1
	TIM_SelectMasterSlaveMode( TIM4, TIM_MasterSlaveMode_Enable);		// 定时器主从模式使能
	TIM_SelectOutputTrigger( TIM4, TIM_TRGOSource_Update);						// 选择触发方式：使用更新事件作为触发输出
	TIM_OC1PreloadConfig( TIM4, TIM_OCPreload_Enable);		// CH1预装载使能，修改				
	// PWM通道，TIM3 - 通道2设置函数，100/100 or 0/100
	TIM_OCInitStructure.TIM_Pulse = DIR_B;									// 初始化 TIM3-OC2
	TIM_OC2Init( TIM4, &TIM_OCInitStructure);						// CH2预装载使能，修改
	TIM_OC2PreloadConfig( TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig( TIM4, ENABLE);					// 使能ARR预装载寄存器
}

//定时器8从模式
void TIM8_GPIO_Config(u32 PulseNum_B)
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM8, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = PulseNum_B;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM8, &TIM_TimeBaseStructure);
	
	TIM_SelectInputTrigger( TIM8, TIM_TS_ITR2);			// TIM2-主，TIM4-从
	TIM_SelectSlaveMode( TIM8, TIM_SlaveMode_External1);
	TIM_ITConfig( TIM8, TIM_IT_Update, DISABLE);
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure);
}

void PWM_Output_Y(u16 Cycle_B, u32 PulseNum_B)			// TIM2-主，TIM4-从
{
//	TIM8_GPIO_Config(PulseNum_B);
	TIM8 -> ARR = PulseNum_B - 1;
	TIM_Cmd( TIM8, ENABLE);
	TIM_ClearITPendingBit( TIM8, TIM_IT_Update);
	TIM_ITConfig( TIM8, TIM_IT_Update, ENABLE);
//	TIM4_GPIO_Config( 84, Cycle_B, Cycle_B / 2, DIR_B);		//F407:通用定时器是 84MHz, 故84MHz / 84 = 1MHz
	TIM4 -> ARR = Cycle_B - 1;
	TIM_Cmd( TIM4, ENABLE);
}

/*********** Z ***********/
// 定时器3主模式
void TIM3_Config(u16 TIM3_Prescaler, u16 TIM3_Period, u16 CCR_C, u16 DIR_C){
	GPIO_InitTypeDef	GPIO_InitStructure;
	// TIM3通道1\2 - PWM Z轴步进电机脉冲信号
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);			
	
	GPIO_PinAFConfig( GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);		
	GPIO_PinAFConfig( GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;			// TIM3_CH1 PA6, CH2 - PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
	
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	// 时钟频率设置
	TIM_TimeBaseStructure.TIM_Period = TIM3_Period - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM3_Prescaler - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	// 设置工作模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			// 设置工作模式是PWM，且为PWM1工作模式，TIMx_CNT<TIMx_CCR1时为高电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		// 也就是使能PWM输出到端口					
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;			// 输出极性
	// PWM通道，TIM3 - 通道1设置函数，50/100
	TIM_OCInitStructure.TIM_Pulse = CCR_C;					// 设置待装入捕获寄存器的脉冲值
	TIM_OC3Init( TIM3, &TIM_OCInitStructure);						// 初始化 TIM3-OC1
	TIM_SelectMasterSlaveMode( TIM3, TIM_MasterSlaveMode_Enable);		// 定时器主从模式使能
	TIM_SelectOutputTrigger( TIM3, TIM_TRGOSource_Update);						// 选择触发方式：使用更新事件作为触发输出
	TIM_OC3PreloadConfig( TIM3, TIM_OCPreload_Enable);		// CH1预装载使能，修改				
//	// PWM通道，TIM3 - 通道2设置函数，100/100 or 0/100
//	TIM_OCInitStructure.TIM_Pulse = DIR_C;									// 初始化 TIM3-OC2
//	TIM_OC4Init( TIM3, &TIM_OCInitStructure);						// CH2预装载使能，修改
//	TIM_OC4PreloadConfig( TIM3, TIM_OCPreload_Enable);
//	TIM_ARRPreloadConfig( TIM3, ENABLE);					// 使能ARR预装载寄存器
}

// 定时器5从模式
void TIM5_Config(u32 PulseNum_Z){
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = PulseNum_Z;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM5, &TIM_TimeBaseStructure);
	
	TIM_SelectInputTrigger( TIM5, TIM_TS_ITR1);
	TIM_SelectSlaveMode( TIM5,TIM_SlaveMode_External1 );		// 等同下一句 TIM5->SMCR |= 0x07
	TIM_ITConfig( TIM5, TIM_IT_Update, DISABLE);
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void PWM_Output_Z(u16 Cycle_C, u32 PulseNum_C){
//	TIM5_Config(PulseNum_C);
	TIM5 -> ARR = PulseNum_C - 1;
	TIM_Cmd( TIM5, ENABLE);
	TIM_ClearITPendingBit( TIM5, TIM_IT_Update);
	TIM_ITConfig( TIM5, TIM_IT_Update, ENABLE);
//	TIM3_Config( 84, Cycle_C, Cycle_C / 2, DIR_C);		//84MHz / 56 = 1.5MHz
	TIM3 -> ARR = Cycle_C - 1;
	TIM_Cmd( TIM3, ENABLE);
}

/*
***************************************************
函数名：disable_output
功能：停止输出pwm，停止计数
入口参数：	motor_num 电机号
返回值：无
应用范围：外部调用
备注：
***************************************************
*/
void disable_output(uint8_t motor_num){
	switch(motor_num){
		case _x:
			TIM_CtrlPWMOutputs( TIM1, DISABLE);		// 高级定时器 TIM1 关闭MOE
			TIM_Cmd( TIM1, DISABLE);			// 关闭定时器1
			TIM_Cmd( TIM2, DISABLE);			// 关闭定时器2	
			TIM_ITConfig( TIM2, TIM_IT_Update, DISABLE);
		break;
		
		case _y:
			TIM_Cmd( TIM4, DISABLE);			// 关闭定时器4
			TIM_Cmd( TIM8, DISABLE);			// 关闭定时器8
			TIM_ITConfig( TIM8, TIM_IT_Update, DISABLE);
		break;
		
		case _z:
			TIM_Cmd( TIM3, DISABLE);			// 关闭定时器3
			TIM_Cmd( TIM5, DISABLE);			// 关闭定时器5
			TIM_ITConfig( TIM5, TIM_IT_Update, DISABLE);
		break;
	}
}




