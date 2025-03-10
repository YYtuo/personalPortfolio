#ifndef __MENU_H
#define __MENU_H

#include "oled.h"
#include "bsp_key.h"
#include "IAP.h"
#include "LinkQueue.h"

typedef struct windowLim{
	void* windowUpper;// ��ʾ��������,���ļ������ڵ�����ʾ����4��ʱ�õ�
	void* windowLower;// ��ʾ��������,���ļ������ڵ�����ʾ����4��ʱ�õ�
}windowLim_TypeDef;


typedef struct{
	// uint8_t Interface_Mark;     	//����״̬
	// uint8_t Menu_Interface_Flag;

	uint8_t main_PreMeunPosition;	// ��һ�����˵����λ��
	uint8_t main_MeunPosition;		// ���˵����λ��

	uint8_t fileList_PreIndex;		// ��һ���ļ�����,�������ļ���һ��λ��
	uint8_t fileList_Index;			// �ļ�����,�����ŵ�ǰʵ���ļ�λ��

	windowLim_TypeDef windowLimite;			// ���ڶ��󣬵��ļ������ڵ�����ʾ����4��ʱ�õ�
	Circle_LinkQueue_TypeDef* showQueue;	// �ļ���ʾ���У����ļ������ڵ�����ʾ����4��ʱ�õ�
	
}Menu_Str;

enum{
	Main_Interface	=  0x10, /****������*****/
	
	Menu_Interface_1	=  	0x20, /****�˵�����1***/
	Fail_Interface		=  	0x30, /****����ʧ�ܽ���***/
};

extern Menu_Str gui;

void menuInit(void);
void fileListInterFaceInit(uint8_t (*fileList)[FileList_SIZE], uint8_t file_num);
uint8_t meunTaskRun(uint8_t (*fileList)[FileList_SIZE]);

#endif
