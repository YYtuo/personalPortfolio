#ifndef __MENU_H
#define __MENU_H

#include "oled.h"
#include "bsp_key.h"
#include "IAP.h"
#include "LinkQueue.h"

typedef struct windowLim{
	void* windowUpper;// 显示窗口上限,当文件数大于单屏显示数（4）时用到
	void* windowLower;// 显示窗口下限,当文件数大于单屏显示数（4）时用到
}windowLim_TypeDef;


typedef struct{
	// uint8_t Interface_Mark;     	//界面状态
	// uint8_t Menu_Interface_Flag;

	uint8_t main_PreMeunPosition;	// 上一次主菜单光标位置
	uint8_t main_MeunPosition;		// 主菜单光标位置

	uint8_t fileList_PreIndex;		// 上一次文件索引,保存着文件上一次位置
	uint8_t fileList_Index;			// 文件索引,保存着当前实际文件位置

	windowLim_TypeDef windowLimite;			// 窗口对象，当文件数大于单屏显示数（4）时用到
	Circle_LinkQueue_TypeDef* showQueue;	// 文件显示队列，当文件数大于单屏显示数（4）时用到
	
}Menu_Str;

enum{
	Main_Interface	=  0x10, /****主界面*****/
	
	Menu_Interface_1	=  	0x20, /****菜单界面1***/
	Fail_Interface		=  	0x30, /****下载失败界面***/
};

extern Menu_Str gui;

void menuInit(void);
void fileListInterFaceInit(uint8_t (*fileList)[FileList_SIZE], uint8_t file_num);
uint8_t meunTaskRun(uint8_t (*fileList)[FileList_SIZE]);

#endif
