#ifndef __DM542_H
#define __DM542_H

#include "top.h"
#include "delay.h"
#include "BSP_GPIO.h"
#include "timer.h"


#define ENA_x PCout(0)
#define DIR_x PCout(1)

#define ENA_y PAout(2)
#define DIR_y PAout(3)

#define ENA_z PAout(4)
#define DIR_z PAout(5)

#define X_L 		PCin(2)   	//X_L
#define X_R 		PCin(3)		//X_R 
#define Y_U 		PAin(0)		//Y_U
#define Y_D 		PAin(1)		//Y_D
#define Z_H			PBin(12)	//Z_H
#define Z_L			PBin(13)	//Z_L


#define ACHIEVE_X_L 	0x01//左
#define ACHIEVE_X_R		0x02//右
#define ACHIEVE_Y_U		0x04//上
#define ACHIEVE_Y_D   	0x08//下
#define ACHIEVE_Z_H		0x10//顶部
#define ACHIEVE_Z_L		0x20//底部
#define BEFORE_INIT		0x80//底部


#define ACHIEVE_X_L_Y_U		5//左上
#define ACHIEVE_X_R_Y_U		6//右上
#define ACHIEVE_X_L_Y_D		9//左下
#define ACHIEVE_X_R_Y_D		10//右下



#define meter_per_cycle  5.0f//5mm
#define step_per_cycle  5000.0f//5mm
//速度最快60
#define x_fre	200			//84M / 84 = 1MHz, 1M / 100 = 10kHz
#define y_fre	200			//84M / 84 = 1MHz, 1M / 100 = 10kHz
#define z_fre	2000			//84M / 84 = 1MHz, 1M / 100 = 10kHz

typedef struct{
	uint8_t num;
	uint16_t angle;
	uint32_t frequency;
}step_MotorStr;

extern point_dataStr point[];

enum{
	_x = 0,
	_y,
	_z
};

enum {
	reverse = 0,
	forward
	
};

enum {
	right = 0,
	left = 1,
	
	down = 0,
	up = 1,
	
	motor_high = 0,
	motor_low = 1
};

void contorl_GPIO_config(void);
uint8_t limit_Scan(uint8_t mode);

void motorInit_L_D(void);
void motorInit_R_U(void);
uint8_t z_motorInit_bottom(void);
uint8_t z_motorInit_top(void);

void motor_move(uint8_t motor_num, uint32_t step, uint8_t direction, uint16_t motor_speed);
void move_distance(uint8_t motor_num, float distance, uint8_t direction,uint16_t motor_speed);
uint8_t all_motor_busy(void);












void MotorDataInit(step_MotorStr* motorDat);
void y_motorReset(void);

void motorEnable(uint8_t motorNum,FunctionalState state);
void pulse_Init(void);
uint8_t x_motorPulse_freSet(int fre);
uint8_t y_z_motorPulse_freSet(int fre);
uint8_t pulse_countInit(void);
void OutputControl(uint8_t motorNum,FunctionalState state);
uint8_t motorMove(uint8_t motorNum,float angle,uint8_t direction);

void x_y_move_distance(uint8_t motor_num, float distance, uint8_t direction);
void z_move_distance(uint8_t motor_num, float distance, uint8_t direction);

void z_motorInit(void);
void motorInit1(void);
void motorInit2(void);
void motor_goto_R_D(void);






void injectorReset(void);
void beginWork(void);
void rectangle_move(float x1, float x2, float y1, float y2);
void point_move(void);



#endif
