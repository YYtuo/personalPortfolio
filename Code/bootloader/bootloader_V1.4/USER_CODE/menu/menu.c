#include "menu.h"

Menu_Str gui;

extern uint8_t file_num;
extern char* file_name;
// extern uint8_t bin_fileList[][FileList_SIZE];

//��ʼ����ĳ�ʼ��
void mainInterFaceInit(void){
	oled_Cls();
	oled_DrawBMP(0,0,128,8,(unsigned char *)tuo);//����BMPλͼ��ʾ
}

// ��ʼ��oled�˵���ʾ�ļ��б���ʼ��ʱֻ��ʾǰ�ĸ�
void fileListInterFaceInit(uint8_t (*fileList)[FileList_SIZE], uint8_t file_num){
	// spring()
	oled_ShowStr(0,0,fileList[0],small);
	oled_ShowStr(0,2,fileList[1],small);
	oled_ShowStr(0,4,fileList[2],small);
	oled_ShowStr(0,6,fileList[3],small);
	drowPiont_8x8(15,1);		// ��ʾ��һ����

	// �ļ��������ģ���ʼ����ʾ����
	if(file_num > 4){
		// ����һ������
		gui.showQueue = Create_Circle_LinkQueue();
		// �������ļ���ѹ�뻷�ζ�����
		for(int i=0; i<file_num; ++i){
			Ins_CircleQueue(gui.showQueue,fileList[i]);
		}
		
		// �����ڵ�����ָ���һ���ļ�������ָ����ĸ��ļ�
		gui.windowLimite.windowUpper = gui.showQueue->Rear->next;				// ��������ָ����еĵ�һ��
		gui.windowLimite.windowLower = gui.showQueue->Rear->next->next->next->next;	// ��������ָ����еĵ�����
	}
	
}

/**********************************
 * ��������cursorMovement
 * ���ܣ�������
 * ���룺��
 * �������
 * ��ע����
**********************************/
void cursorMovement(void){
	uint8_t local = (2*gui.main_MeunPosition-1)%8;
	uint8_t prelocal = (2*gui.main_PreMeunPosition-1)%8;

	cleanPiont_8x8(15,prelocal);
	drowPiont_8x8(15,local);
}

/**********************************
 * ��������windowMovement_up
 * ���ܣ������Ͻ�����һλ
 * ���룺qu����������ʾ���ݵĻ��ζ��У�window_x�����ڽ���
 * �������
 * ��ע��windowMovement�ڲ�����
**********************************/
static void windowMovement_up(Circle_LinkQueue_TypeDef* qu, windowLim_TypeDef* window_x){
	uint8_t cnt = (qu->queue_nodeNum - 4);
	Node_TypeDef* temp_node = (Node_TypeDef*)window_x->windowLower;

	for(int i=0; i<cnt; ++i){
		temp_node = temp_node->next;
	}
	
	window_x->windowUpper = temp_node;
	window_x->windowLower = temp_node->next->next->next;
}

/**********************************
 * ��������windowMovement_down
 * ���ܣ������Ͻ�����һλ
 * ���룺window_x�����ڽ���
 * �������
 * ��ע��windowMovement�ڲ�����
**********************************/
static void windowMovement_down(windowLim_TypeDef* window_x){
	Node_TypeDef* temp_node = (Node_TypeDef*)window_x->windowUpper;
	window_x->windowUpper = temp_node->next;
	window_x->windowLower = temp_node->next->next->next->next;
}
/**********************************
 * ��������windowMovement
 * ���ܣ����ڹ���
 * ���룺qu����������ʾ���ݵĻ��ζ��У�window_x�����ڽ���
 * �������
 * ��ע����
**********************************/
void windowMovement(Circle_LinkQueue_TypeDef* qu, windowLim_TypeDef* window_x){
	// �ж�����ǰ�ƶ������������ƶ�
	// λ��++����С���
	if(gui.fileList_PreIndex < gui.fileList_Index ){
		// �ж��Ƿ�Ϊλ��--�Ľ���㣬�����������
		if(gui.fileList_PreIndex == 1 && gui.fileList_Index == qu->queue_nodeNum){
			windowMovement_up(qu, window_x);
		}
		// ����λ��--����㣬����
		else{
			windowMovement_down(window_x);		
		}
	}
	
	// λ��--���ɴ��С
	else {
		// λ��++����㣬����
		if(gui.fileList_PreIndex == qu->queue_nodeNum && gui.fileList_Index == 1){
			windowMovement_down(window_x);
		}
		// ��λ��++����㣬����
		else{
			windowMovement_up(qu, window_x);
		}
	}

	// ���ô��ڵĽ��޺󣬿�ʼ��ʾ����
	Node_TypeDef* showstr = (Node_TypeDef*)window_x->windowUpper;

	for(int i = 0; i<4; ++i){
		oled_ShowStr(0,2*i,showstr->fileName,small);
		showstr = showstr->next;
	}

}




//���˵���ʾ
void meunInterFace_1(uint8_t (*fileList)[FileList_SIZE]){
	// oled_Cls();
	
	// �ļ���С��4�����÷�ҳ��������
	if(gui.showQueue == NULL){
		cursorMovement();
	}
	// �ļ�������4����ҳ��ʾ
	else{
		if( gui.main_MeunPosition != gui.main_PreMeunPosition ){
			// ������
			cursorMovement();
		}
		else {
			// ���ݹ���
			windowMovement( gui.showQueue, &(gui.windowLimite) );
		}

	}
	
}

//�˵����ܳ�ʼ��
void menuInit(void){
	//�˵���ҳ��ʾ
	// gui.Interface_Mark = Menu_Interface_1;
	
//	gui.Menu_Interface_Flag = 0;//Ĭ�Ͽ���������
	gui.fileList_PreIndex = 1;//1�Ų˵�ָ���ʼ��
	gui.fileList_Index = 1;//1�Ų˵�ָ���ʼ��
	
	gui.main_PreMeunPosition = 1;//1�Ų˵�ָ���ʼ��
	gui.main_MeunPosition = 1;//1�Ų˵�ָ���ʼ��
	
	// gui.fileList_showIndex = 0;
	
	gui.windowLimite.windowLower = gui.windowLimite.windowLower = NULL;
	gui.showQueue = NULL;
	
	
	mainInterFaceInit();
}

uint8_t meunTaskRun(uint8_t (*fileList)[FileList_SIZE]){
	//����������߼�
	
	//���˵�_1 �����߼�
	// if(gui.Interface_Mark == Menu_Interface_1){
		//�ϼ�����:ѡ����Ҫ���صĳ���
		if(up_key_parameter.key_event.shortPressed){
			// ��������¼���־λ
			up_key_parameter.key_event.shortPressed = 0;

			gui.fileList_Index--;
			gui.main_MeunPosition--;
			
			if(gui.fileList_Index < 1) gui.fileList_Index = file_num;
			if(gui.main_MeunPosition < 1){
				if(file_num > 4){
					gui.main_MeunPosition = 1;
				}
				else{
					gui.main_MeunPosition = file_num;
				}
			}
		}

		//�¼�����:ѡ����Ҫ���صĳ���
		if(down_key_parameter.key_event.shortPressed){
			// ��������¼���־λ
			down_key_parameter.key_event.shortPressed = 0;
			
			gui.fileList_Index++;
			gui.main_MeunPosition++;
			if(gui.fileList_Index > file_num) gui.fileList_Index = 1;

			if (file_num > 4){
				if(gui.main_MeunPosition > 4)
					gui.main_MeunPosition = 4;
			}
			else{
				if(gui.main_MeunPosition > file_num)
					gui.main_MeunPosition = 1;
			}
		}

		if(fun_key_parameter.key_event.shortPressed){
			// ȷ�ϳ����Ҽ���
			// load_file = (char*)fileList[gui.fileList_Index];
			fun_key_parameter.key_event.shortPressed = 0;

			oled_Cls();
			// gui.Interface_Mark = Fail_Interface;
			return gui.fileList_Index;
		}
		if(gui.fileList_PreIndex != gui.fileList_Index){
			meunInterFace_1(fileList);
			gui.fileList_PreIndex = gui.fileList_Index;
			gui.main_PreMeunPosition = gui.main_MeunPosition;
			
		}
	// }

	// if(gui.Interface_Mark == Fail_Interface){
	// 	oled_ShowStr(0,2,"app error!",big);
	// 	return 0;
	// }


	return 0;

}

