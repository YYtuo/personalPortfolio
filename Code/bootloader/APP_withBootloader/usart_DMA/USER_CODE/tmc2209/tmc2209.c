#include "tmc2209.h"
#include "stdio.h"
uint8_t limit_state = 0;
uint8_t limit_last_state = 0;

void limit_init(void){
	BSP_GPIO_Init(BSP_GPIOC2,GPIO_Mode_IPU);
	BSP_GPIO_Init(BSP_GPIOC3,GPIO_Mode_IPU);
	
	BSP_GPIO_Init(BSP_GPIOA0,GPIO_Mode_IPU);
	BSP_GPIO_Init(BSP_GPIOA1,GPIO_Mode_IPU);
	
	BSP_GPIO_Init(BSP_GPIOB12,GPIO_Mode_IPU);
	BSP_GPIO_Init(BSP_GPIOB13,GPIO_Mode_IPU);
}

void contorl_GPIO_config(void){
	limit_init();
	BSP_GPIO_Init(BSP_GPIOC0,GPIO_Mode_Out_PP);
	BSP_GPIO_Init(BSP_GPIOC1,GPIO_Mode_Out_PP);
	
	BSP_GPIO_Init(BSP_GPIOA2,GPIO_Mode_Out_PP);
	BSP_GPIO_Init(BSP_GPIOA3,GPIO_Mode_Out_PP);
	
	BSP_GPIO_Init(BSP_GPIOA4,GPIO_Mode_Out_PP);
	BSP_GPIO_Init(BSP_GPIOA5,GPIO_Mode_Out_PP);

	ENA_x = 0; DIR_x = 0; 
	ENA_y = 0; DIR_y = 0; 
	ENA_z = 0; DIR_z = 0;
}

u8 limit_Scan(u8 mode){
	static u8 key_up=1;//�������ɿ���־
//	static uint16_t pretime = 0, last_time = 0;
	
	if(mode)key_up=1;  //֧������		
	
	if( key_up && (X_L==0 || X_R==0 || Y_U==0 || Y_D == 0 || Z_H == 0 || Z_L == 0) )
	{
		delay_ms(4);//ȥ���� 
		key_up=0;
		if(X_L == 0){
			limit_state |= ACHIEVE_X_L;
			disable_output(_x);//ǿ��ֹͣ��ǰ�˶�
			ENA_x = ENABLE;
			move_distance( _x, 1, right, NULL );	
		}
		if(X_R == 0){
			limit_state |= ACHIEVE_X_R;
			disable_output(_x);//ǿ��ֹͣ��ǰ�˶�
			ENA_x = ENABLE;
			move_distance( _x, 1, left, NULL );;
		}
		if(Y_U == 0){
			limit_state |= ACHIEVE_Y_U;
			disable_output(_y);//ǿ��ֹͣ��ǰ�˶�
			ENA_y = ENABLE;
			move_distance( _y, 1, down, NULL );
		}
		if(Y_D == 0){
			limit_state |= ACHIEVE_Y_D;
			disable_output(_y);//ǿ��ֹͣ��ǰ�˶�
			ENA_y = ENABLE;
			move_distance( _y, 1, up, NULL );
		}
		if(Z_H == 0){
			limit_state |= ACHIEVE_Z_H;
			move_distance( _z, 1, motor_low, NULL );
		}
		if(Z_L == 0){
			limit_state |= ACHIEVE_Z_L;
			move_distance( _z, 1, motor_high, NULL );
		}
	
		return 1;//��������һ���߷���1
	}
	
	else if(X_L == 1 && X_R == 1 && Y_U==1 && Y_D==1 && Z_H == 1 && Z_L == 1){
		limit_state = 0;
		key_up=1;
	}
	
 	return 0;// �ް�������
}



/*************
�ض��㸴λ
*************/
//�����λ�����½ǣ���x��y�������(0,0)��
void motorInit_L_D(void){
	TIM1_Config(168, 100, 100 / 2, 0);	//�߼���ʱ���� 168MHz, ��168MHz / 84 = 2MHz
	TIM_Cmd( TIM1, ENABLE);
	TIM_CtrlPWMOutputs( TIM1, ENABLE);	// �߼���ʱ�� TIM1 ʹ�� MOE
	
	TIM4_GPIO_Config( 84, 100, 100 / 2, 0);		//F407:ͨ�ö�ʱ���� 84MHz, ��84MHz / 84 = 1MHz
	TIM_Cmd( TIM4, ENABLE);
	
	while( !(((limit_state & ACHIEVE_X_L) != 0) && ((limit_state & ACHIEVE_Y_D) != 0)) ){
		DIR_x = left;
		DIR_y = down;
	
		if( (limit_state & ACHIEVE_X_L)  == 1 )
			ENA_x = DISABLE;
		else 
			ENA_x = ENABLE;
		if((limit_state & ACHIEVE_Y_D)  == ACHIEVE_Y_D)
			ENA_y = DISABLE;
		else 
			ENA_y = ENABLE;
	}
	
//	ENA_x = DISABLE;
//	ENA_y = DISABLE;
	
	TIM_Cmd( TIM1, DISABLE);			// �رն�ʱ��1
	TIM_Cmd( TIM4, DISABLE);			// �رն�ʱ��4
//	TIM_Cmd( TIM3, DISABLE);			// �رն�ʱ��3
	
	
	limit_state = 0;
}

void motorInit_R_U(void){
	TIM1_Config(168, x_fre, x_fre / 2, 0);	//�߼���ʱ���� 168MHz, ��168MHz / 84 = 2MHz
	TIM_Cmd( TIM1, ENABLE);
	TIM_CtrlPWMOutputs( TIM1, ENABLE);	// �߼���ʱ�� TIM1 ʹ�� MOE
	
	TIM4_GPIO_Config( 84, y_fre, y_fre / 2, 0);		//F407:ͨ�ö�ʱ���� 84MHz, ��84MHz / 84 = 1MHz
	TIM_Cmd( TIM4, ENABLE);
	
	while( !(((limit_state & ACHIEVE_X_R) != 0) && ((limit_state & ACHIEVE_Y_U) != 0)) ){
		DIR_x = right;
		DIR_y = up;
	
		if( (limit_state & ACHIEVE_X_R)  == ACHIEVE_X_R )
			ENA_x = DISABLE;
		else 
			ENA_x = ENABLE;
		if( (limit_state & ACHIEVE_Y_U)  == ACHIEVE_Y_U )
			ENA_y = DISABLE;
		else 
			ENA_y = ENABLE;
	}
		
//	ENA_x = DISABLE;
//	ENA_y = DISABLE;
	TIM_Cmd( TIM1, DISABLE);			// �رն�ʱ��1
	TIM_Cmd( TIM4, DISABLE);			// �رն�ʱ��4
	
	limit_state = 0;
}

/*
***************************************************
��������z_motorInit_top
���ܣ����������λ
��ڲ�������
����ֵ���ɹ�����1
Ӧ�÷�Χ���ⲿ����

��ע��
***************************************************
*/
uint8_t z_motorInit_top(void){
	TIM3_Config( 84, 2000, 1000, 0);
	TIM_Cmd( TIM3, ENABLE);
	
	while(limit_state >> 4 != 1){
		DIR_z = motor_high;
//	
//		if(limit_state >> 4 == 1)ENA_z = DISABLE;
//		else 
			ENA_z = ENABLE;

	}
	
//	ENA_z = DISABLE;
	TIM_Cmd( TIM3, DISABLE);			// �رն�ʱ��3
	limit_state = 0;
	return 1;
}

/*
***************************************************
��������z_motorInit_bottom
���ܣ�����ײ���λ
��ڲ�������
����ֵ���ɹ�����1
Ӧ�÷�Χ���ⲿ����

��ע��
***************************************************
*/
uint8_t z_motorInit_bottom(void){
	TIM3_Config( 84, 2000, 1000, 0);
	TIM_Cmd( TIM3, ENABLE);
	
	while(limit_state >> 5 != 1){
		DIR_z = motor_low;
	
//		if(limit_state >> 5 == 1)ENA_z = DISABLE;
//		else 
			ENA_z = ENABLE;

	}
	
//	ENA_z = DISABLE;
	TIM_Cmd( TIM3, DISABLE);			// �رն�ʱ��3
	limit_state = 0;
	return 1;
}


/*
***************************************************
��������motor_move
���ܣ����巢�����ձ�
��ڲ�����	motor_num�������
			step���������
			direction������
����ֵ����
Ӧ�÷�Χ���ⲿ����

��ע���޿���ʹ��λ�����ڶ����߼��п��ǣ���æ��־λ�ڶ�ʱ���ж������
***************************************************
*/
void motor_move(uint8_t motor_num, u32 step, uint8_t direction, uint16_t motor_speed){
	uint16_t mspeed_x, mspeed_y, mspeed_z;
	
	//�ٶ��޲�����ʹ��Ĭ��ֵ
	if(motor_speed != NULL){
		mspeed_x = mspeed_y = mspeed_z = motor_speed;
	}
	
	else{
		mspeed_x = x_fre;
		mspeed_y = y_fre;
		mspeed_z = z_fre;
	}
	switch (motor_num){
		case _x:
			DIR_x = direction;
//			busy_reg |= 0x01;//x�����æ
			PWM_Output_X(mspeed_x, step);
		break;
		
		case _y:
			DIR_y = direction;
//			busy_reg |= 0x02;//y�����æ
			PWM_Output_Y(mspeed_y, step);
		break;
		
		case _z:
			DIR_z = direction;
//			busy_reg |= 0x04;//z�����æ
			PWM_Output_Z(mspeed_z, step);
		break;
	}
}


/*
***************************************************
��������move_distance
���ܣ�����ת������������ƶ�
��ڲ�����	motor_num�������
			distance������
			direction������
����ֵ����
Ӧ�÷�Χ���ⲿ����

��ע�������߼���Ҫ�ӱ�־λ���жϵ���Ƿ������һ�ε��˶�
***************************************************
*/
void move_distance(uint8_t motor_num, float distance, uint8_t direction, uint16_t motor_speed){
	double step = 0.0;
	if(distance == 0)return;
	
	if(motor_num != _z)
		step = distance / (meter_per_cycle / step_per_cycle);
	else
		step = distance / (94.2f / step_per_cycle);//ÿһ��0.006mm
	step += 0.5;											//����ǿ��ת��ʱ��������
	motor_move(motor_num, (u32)step, direction,	motor_speed);
}

/*
***************************************************
��������all_motor_busy
���ܣ��жϵ���Ƿ�æ
��ڲ�����	��
����ֵ����
Ӧ�÷�Χ���ⲿ����
��ע�����е�����з���0
***************************************************
*/
uint8_t all_motor_busy(void){
//	return busy_reg & 0xff;
	return 0;
}

