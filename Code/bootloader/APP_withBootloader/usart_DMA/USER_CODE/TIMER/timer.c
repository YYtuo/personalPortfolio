#include "timer.h"
#include "bsp_led.h"
#include "tmc2209.h"

uint8_t PWM;//pwm����

uint8_t pwm_R,pwm_G,pwm_B;//��pwmԤ��Ƶ

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM6_Int_Init(u16 arr,u16 psc){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6|RCC_APB1Periph_TIM2,ENABLE);  ///ʹ��TIM3ʱ��

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM6,ENABLE); //ʹ�ܶ�ʱ��3
	
	
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //��ʱ��2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM2
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //����ʱ��2�����ж�
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��2
}

void pwm_output_init(void){
	//X�ᶨʱ�����ʼ��
	TIM1_Config(168, 100, 50, 0);		//168��Ƶ���õ�1MHz��1M/100 = 10KHz 	50ռ�ձ�
	TIM2_Config(0);
	//Y�ᶨʱ�����ʼ��
	TIM4_GPIO_Config( 84, 100, 50, 0);	//84��Ƶ���õ�1MHz��1M/100 = 10KHz		50ռ�ձ�
	TIM8_GPIO_Config(0);
	//Z�ᶨʱ�����ʼ��
	TIM3_Config( 84, 2000, 50, 0);		//84��Ƶ���õ�1MHz��1M/100 = 10KHz		50ռ�ձ�
	TIM5_Config(0);
}

//void disable_output(uint8_t motor_num){
//	switch(motor_num){
//		case _x:
//			TIM_CtrlPWMOutputs( TIM1, DISABLE);		// �߼���ʱ�� TIM1 �ر�MOE
//			TIM_Cmd( TIM1, DISABLE);			// �رն�ʱ��1
//			TIM_Cmd( TIM2, DISABLE);			// �رն�ʱ��2	
//			TIM_ITConfig( TIM2, TIM_IT_Update, DISABLE);
//			busy_reg &= 0xFE;//���x�����æ״̬
//		break;
//		
//		case _y:
//			TIM_Cmd( TIM4, DISABLE);			// �رն�ʱ��4
//			TIM_Cmd( TIM8, DISABLE);			// �رն�ʱ��8
//			TIM_ITConfig( TIM8, TIM_IT_Update, DISABLE);
//			busy_reg &= 0xFD;//���y�����æ״̬
//		break;
//		
//		case _z:
//			TIM_Cmd( TIM3, DISABLE);			// �رն�ʱ��3
//			TIM_Cmd( TIM5, DISABLE);			// �رն�ʱ��5
//			TIM_ITConfig( TIM5, TIM_IT_Update, DISABLE);
//			busy_reg &= 0xFB;//���z�����æ״̬
//		break;
//	}
//}
/*********** X ***********/
// ��ʱ��1��ģʽ
void TIM1_Config(u16 TIM1_Prescaler, u16 TIM1_Period, u16 CCR_A, u16 DIR_A){
	GPIO_InitTypeDef	GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOE, ENABLE);		
	
	GPIO_PinAFConfig( GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	GPIO_PinAFConfig( GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;		// TIM1_CH1 - PE13�� CH4 - PE14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// ����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		// ���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// ����
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = TIM1_Period - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM1_Prescaler - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			// �ظ�������һ��Ҫ = 0
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	// ���ù���ģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;							// PWM1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	// �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;			// �������
	// PWMͨ����TIM1 - ͨ��1���ú�����50/100
	TIM_OCInitStructure.TIM_Pulse = CCR_A;								// ���ô�װ�벶��Ĵ���������ֵ
	TIM_OC3Init( TIM1, &TIM_OCInitStructure);
	TIM_SelectMasterSlaveMode( TIM1, TIM_MasterSlaveMode_Enable);
	TIM_SelectOutputTrigger( TIM1, TIM_TRGOSource_Update);
	TIM_OC3PreloadConfig( TIM1, TIM_OCPreload_Enable);
	// PWMͨ����TIM1 - ͨ��4���ú�����100/100 or 0/100
	TIM_OCInitStructure.TIM_Pulse = DIR_A;								// ��ʼ�� TIM1-OC4
	TIM_OC4Init( TIM1, &TIM_OCInitStructure);						// CH4Ԥװ��ʹ�ܣ��޸�
	TIM_OC4PreloadConfig( TIM1, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1, ENABLE);
}

//��ʱ��2��ģʽ
void TIM2_Config(u32 PulseNum_A){
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = PulseNum_A;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM2, &TIM_TimeBaseStructure);
	
	TIM_SelectInputTrigger( TIM2, TIM_TS_ITR0);			// TIM1-����TIM2-��
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
��������PWM_Output_X
���ܣ�����X�����������
��ڲ�����	Cycle_A ����Ƶ��
����ֵ���ɹ�����1
Ӧ�÷�Χ���ⲿ����
��ע��
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
	TIM_CtrlPWMOutputs( TIM1, ENABLE);	// �߼���ʱ�� TIM1 ʹ�� MOE
}
/*********** Group B ***********/
// ��ʱ��4��ģʽ
void TIM4_GPIO_Config(u16 TIM4_Prescaler, u16 TIM4_Period, u16 CCR_B, u16 DIR_B)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	// TIM3ͨ��1\2 - PWM Z�Ჽ����������ź�
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE);			
	
	GPIO_PinAFConfig( GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);		
	GPIO_PinAFConfig( GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;			// TIM3_CH1 PA6, CH2 - PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);		
	
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	// ʱ��Ƶ������
	TIM_TimeBaseStructure.TIM_Period = TIM4_Period - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM4_Prescaler - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM4, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	// ���ù���ģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			// ���ù���ģʽ��PWM����ΪPWM1����ģʽ��TIMx_CNT<TIMx_CCR1ʱΪ�ߵ�ƽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		// Ҳ����ʹ��PWM������˿�					
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;			// �������
	// PWMͨ����TIM3 - ͨ��1���ú�����50/100
	TIM_OCInitStructure.TIM_Pulse = CCR_B;					// ���ô�װ�벶��Ĵ���������ֵ
	TIM_OC1Init( TIM4, &TIM_OCInitStructure);						// ��ʼ�� TIM3-OC1
	TIM_SelectMasterSlaveMode( TIM4, TIM_MasterSlaveMode_Enable);		// ��ʱ������ģʽʹ��
	TIM_SelectOutputTrigger( TIM4, TIM_TRGOSource_Update);						// ѡ�񴥷���ʽ��ʹ�ø����¼���Ϊ�������
	TIM_OC1PreloadConfig( TIM4, TIM_OCPreload_Enable);		// CH1Ԥװ��ʹ�ܣ��޸�				
	// PWMͨ����TIM3 - ͨ��2���ú�����100/100 or 0/100
	TIM_OCInitStructure.TIM_Pulse = DIR_B;									// ��ʼ�� TIM3-OC2
	TIM_OC2Init( TIM4, &TIM_OCInitStructure);						// CH2Ԥװ��ʹ�ܣ��޸�
	TIM_OC2PreloadConfig( TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig( TIM4, ENABLE);					// ʹ��ARRԤװ�ؼĴ���
}

//��ʱ��8��ģʽ
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
	
	TIM_SelectInputTrigger( TIM8, TIM_TS_ITR2);			// TIM2-����TIM4-��
	TIM_SelectSlaveMode( TIM8, TIM_SlaveMode_External1);
	TIM_ITConfig( TIM8, TIM_IT_Update, DISABLE);
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure);
}

void PWM_Output_Y(u16 Cycle_B, u32 PulseNum_B)			// TIM2-����TIM4-��
{
//	TIM8_GPIO_Config(PulseNum_B);
	TIM8 -> ARR = PulseNum_B - 1;
	TIM_Cmd( TIM8, ENABLE);
	TIM_ClearITPendingBit( TIM8, TIM_IT_Update);
	TIM_ITConfig( TIM8, TIM_IT_Update, ENABLE);
//	TIM4_GPIO_Config( 84, Cycle_B, Cycle_B / 2, DIR_B);		//F407:ͨ�ö�ʱ���� 84MHz, ��84MHz / 84 = 1MHz
	TIM4 -> ARR = Cycle_B - 1;
	TIM_Cmd( TIM4, ENABLE);
}

/*********** Z ***********/
// ��ʱ��3��ģʽ
void TIM3_Config(u16 TIM3_Prescaler, u16 TIM3_Period, u16 CCR_C, u16 DIR_C){
	GPIO_InitTypeDef	GPIO_InitStructure;
	// TIM3ͨ��1\2 - PWM Z�Ჽ����������ź�
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);			
	
	GPIO_PinAFConfig( GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);		
	GPIO_PinAFConfig( GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;			// TIM3_CH1 PA6, CH2 - PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
	
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	// ʱ��Ƶ������
	TIM_TimeBaseStructure.TIM_Period = TIM3_Period - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM3_Prescaler - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	// ���ù���ģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			// ���ù���ģʽ��PWM����ΪPWM1����ģʽ��TIMx_CNT<TIMx_CCR1ʱΪ�ߵ�ƽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		// Ҳ����ʹ��PWM������˿�					
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;			// �������
	// PWMͨ����TIM3 - ͨ��1���ú�����50/100
	TIM_OCInitStructure.TIM_Pulse = CCR_C;					// ���ô�װ�벶��Ĵ���������ֵ
	TIM_OC3Init( TIM3, &TIM_OCInitStructure);						// ��ʼ�� TIM3-OC1
	TIM_SelectMasterSlaveMode( TIM3, TIM_MasterSlaveMode_Enable);		// ��ʱ������ģʽʹ��
	TIM_SelectOutputTrigger( TIM3, TIM_TRGOSource_Update);						// ѡ�񴥷���ʽ��ʹ�ø����¼���Ϊ�������
	TIM_OC3PreloadConfig( TIM3, TIM_OCPreload_Enable);		// CH1Ԥװ��ʹ�ܣ��޸�				
//	// PWMͨ����TIM3 - ͨ��2���ú�����100/100 or 0/100
//	TIM_OCInitStructure.TIM_Pulse = DIR_C;									// ��ʼ�� TIM3-OC2
//	TIM_OC4Init( TIM3, &TIM_OCInitStructure);						// CH2Ԥװ��ʹ�ܣ��޸�
//	TIM_OC4PreloadConfig( TIM3, TIM_OCPreload_Enable);
//	TIM_ARRPreloadConfig( TIM3, ENABLE);					// ʹ��ARRԤװ�ؼĴ���
}

// ��ʱ��5��ģʽ
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
	TIM_SelectSlaveMode( TIM5,TIM_SlaveMode_External1 );		// ��ͬ��һ�� TIM5->SMCR |= 0x07
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
��������disable_output
���ܣ�ֹͣ���pwm��ֹͣ����
��ڲ�����	motor_num �����
����ֵ����
Ӧ�÷�Χ���ⲿ����
��ע��
***************************************************
*/
void disable_output(uint8_t motor_num){
	switch(motor_num){
		case _x:
			TIM_CtrlPWMOutputs( TIM1, DISABLE);		// �߼���ʱ�� TIM1 �ر�MOE
			TIM_Cmd( TIM1, DISABLE);			// �رն�ʱ��1
			TIM_Cmd( TIM2, DISABLE);			// �رն�ʱ��2	
			TIM_ITConfig( TIM2, TIM_IT_Update, DISABLE);
		break;
		
		case _y:
			TIM_Cmd( TIM4, DISABLE);			// �رն�ʱ��4
			TIM_Cmd( TIM8, DISABLE);			// �رն�ʱ��8
			TIM_ITConfig( TIM8, TIM_IT_Update, DISABLE);
		break;
		
		case _z:
			TIM_Cmd( TIM3, DISABLE);			// �رն�ʱ��3
			TIM_Cmd( TIM5, DISABLE);			// �رն�ʱ��5
			TIM_ITConfig( TIM5, TIM_IT_Update, DISABLE);
		break;
	}
}




