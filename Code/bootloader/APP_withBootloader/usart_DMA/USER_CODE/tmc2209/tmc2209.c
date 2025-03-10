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
	static u8 key_up=1;//按键按松开标志
//	static uint16_t pretime = 0, last_time = 0;
	
	if(mode)key_up=1;  //支持连按		
	
	if( key_up && (X_L==0 || X_R==0 || Y_U==0 || Y_D == 0 || Z_H == 0 || Z_L == 0) )
	{
		delay_ms(4);//去抖动 
		key_up=0;
		if(X_L == 0){
			limit_state |= ACHIEVE_X_L;
			disable_output(_x);//强制停止当前运动
			ENA_x = ENABLE;
			move_distance( _x, 1, right, NULL );	
		}
		if(X_R == 0){
			limit_state |= ACHIEVE_X_R;
			disable_output(_x);//强制停止当前运动
			ENA_x = ENABLE;
			move_distance( _x, 1, left, NULL );;
		}
		if(Y_U == 0){
			limit_state |= ACHIEVE_Y_U;
			disable_output(_y);//强制停止当前运动
			ENA_y = ENABLE;
			move_distance( _y, 1, down, NULL );
		}
		if(Y_D == 0){
			limit_state |= ACHIEVE_Y_D;
			disable_output(_y);//强制停止当前运动
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
	
		return 1;//碰到任意一个边返回1
	}
	
	else if(X_L == 1 && X_R == 1 && Y_U==1 && Y_D==1 && Z_H == 1 && Z_L == 1){
		limit_state = 0;
		key_up=1;
	}
	
 	return 0;// 无按键按下
}



/*************
特定点复位
*************/
//电机复位至左下角，即x，y步进电机(0,0)处
void motorInit_L_D(void){
	TIM1_Config(168, 100, 100 / 2, 0);	//高级定时器是 168MHz, 故168MHz / 84 = 2MHz
	TIM_Cmd( TIM1, ENABLE);
	TIM_CtrlPWMOutputs( TIM1, ENABLE);	// 高级定时器 TIM1 使能 MOE
	
	TIM4_GPIO_Config( 84, 100, 100 / 2, 0);		//F407:通用定时器是 84MHz, 故84MHz / 84 = 1MHz
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
	
	TIM_Cmd( TIM1, DISABLE);			// 关闭定时器1
	TIM_Cmd( TIM4, DISABLE);			// 关闭定时器4
//	TIM_Cmd( TIM3, DISABLE);			// 关闭定时器3
	
	
	limit_state = 0;
}

void motorInit_R_U(void){
	TIM1_Config(168, x_fre, x_fre / 2, 0);	//高级定时器是 168MHz, 故168MHz / 84 = 2MHz
	TIM_Cmd( TIM1, ENABLE);
	TIM_CtrlPWMOutputs( TIM1, ENABLE);	// 高级定时器 TIM1 使能 MOE
	
	TIM4_GPIO_Config( 84, y_fre, y_fre / 2, 0);		//F407:通用定时器是 84MHz, 故84MHz / 84 = 1MHz
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
	TIM_Cmd( TIM1, DISABLE);			// 关闭定时器1
	TIM_Cmd( TIM4, DISABLE);			// 关闭定时器4
	
	limit_state = 0;
}

/*
***************************************************
函数名：z_motorInit_top
功能：电机顶部复位
入口参数：无
返回值：成功返回1
应用范围：外部调用

备注：
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
	TIM_Cmd( TIM3, DISABLE);			// 关闭定时器3
	limit_state = 0;
	return 1;
}

/*
***************************************************
函数名：z_motorInit_bottom
功能：电机底部复位
入口参数：无
返回值：成功返回1
应用范围：外部调用

备注：
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
	TIM_Cmd( TIM3, DISABLE);			// 关闭定时器3
	limit_state = 0;
	return 1;
}


/*
***************************************************
函数名：motor_move
功能：脉冲发生对照表
入口参数：	motor_num：电机号
			step：脉冲个数
			direction：方向
返回值：无
应用范围：外部调用

备注：无控制使能位，需在顶层逻辑中考虑，繁忙标志位在定时器中断中清除
***************************************************
*/
void motor_move(uint8_t motor_num, u32 step, uint8_t direction, uint16_t motor_speed){
	uint16_t mspeed_x, mspeed_y, mspeed_z;
	
	//速度无参数则使用默认值
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
//			busy_reg |= 0x01;//x电机繁忙
			PWM_Output_X(mspeed_x, step);
		break;
		
		case _y:
			DIR_y = direction;
//			busy_reg |= 0x02;//y电机繁忙
			PWM_Output_Y(mspeed_y, step);
		break;
		
		case _z:
			DIR_z = direction;
//			busy_reg |= 0x04;//z电机繁忙
			PWM_Output_Z(mspeed_z, step);
		break;
	}
}


/*
***************************************************
函数名：move_distance
功能：距离转换步数，电机移动
入口参数：	motor_num：电机号
			distance：距离
			direction：方向
返回值：无
应用范围：外部调用

备注：顶层逻辑需要加标志位，判断电机是否完成上一次的运动
***************************************************
*/
void move_distance(uint8_t motor_num, float distance, uint8_t direction, uint16_t motor_speed){
	double step = 0.0;
	if(distance == 0)return;
	
	if(motor_num != _z)
		step = distance / (meter_per_cycle / step_per_cycle);
	else
		step = distance / (94.2f / step_per_cycle);//每一步0.006mm
	step += 0.5;											//后续强制转换时四舍五入
	motor_move(motor_num, (u32)step, direction,	motor_speed);
}

/*
***************************************************
函数名：all_motor_busy
功能：判断电机是否繁忙
入口参数：	无
返回值：无
应用范围：外部调用
备注：所有电机空闲返回0
***************************************************
*/
uint8_t all_motor_busy(void){
//	return busy_reg & 0xff;
	return 0;
}

