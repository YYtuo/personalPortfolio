#ifndef __TOP_H__
#define __TOP_H__


#include "stdint.h"

typedef struct{
//	float last_x;
//	float last_y;
//	float last_radius;
	float	x;
	float 	y;
	float 	radius;
}point_dataStr;

typedef struct {
	uint8_t last_time;		//上一时刻；
	uint8_t current_time;	//当前时刻；
	uint8_t receive_flag;   //数据接收标志位；
	
	uint8_t Vertex_coordinates[4][2];
	
	uint8_t center_point[30][2];
	uint8_t radius[30];
	
	uint8_t coordinate_Y[256];
	uint8_t coordinate_Xmin[256];
	uint8_t coordinate_Xmax[256];
	
}receive_data_Str;

typedef struct{
	uint16_t point_num;
	
	float coordinate_X[256];
	float coordinate_Ymin[256];
	float coordinate_Ymax[256];
	
	/*坐标记录*/
	//上一点
	float pre_coordinate_X;
	float pre_coordinate_Y;

	//当前点
	float last_coordinate_X;
	float last_coordinate_Y;

	//下一点
	float next_coordinate_X;
	float next_coordinate_Y;
	
	uint8_t scanf_flag;

}control_dataStr;


#endif

