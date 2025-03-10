#include "menu.h"

Menu_Str gui;

extern uint8_t file_num;
extern char* file_name;
// extern uint8_t bin_fileList[][FileList_SIZE];

//开始界面的初始化
void mainInterFaceInit(void){
	oled_Cls();
	oled_DrawBMP(0,0,128,8,(unsigned char *)tuo);//测试BMP位图显示
}

// 初始化oled菜单显示文件列表，初始化时只显示前四个
void fileListInterFaceInit(uint8_t (*fileList)[FileList_SIZE], uint8_t file_num){
	// spring()
	oled_ShowStr(0,0,fileList[0],small);
	oled_ShowStr(0,2,fileList[1],small);
	oled_ShowStr(0,4,fileList[2],small);
	oled_ShowStr(0,6,fileList[3],small);
	drowPiont_8x8(15,1);		// 显示第一个点

	// 文件数大于四，初始化显示队列
	if(file_num > 4){
		// 创建一个队列
		gui.showQueue = Create_Circle_LinkQueue();
		// 将所有文件名压入环形队列中
		for(int i=0; i<file_num; ++i){
			Ins_CircleQueue(gui.showQueue,fileList[i]);
		}
		
		// 将窗口的上限指向第一个文件，下限指向第四个文件
		gui.windowLimite.windowUpper = gui.showQueue->Rear->next;				// 窗口上限指向队列的第一个
		gui.windowLimite.windowLower = gui.showQueue->Rear->next->next->next->next;	// 窗口下限指向队列的第三个
	}
	
}

/**********************************
 * 函数名：cursorMovement
 * 功能：光标滚动
 * 输入：无
 * 输出：无
 * 备注：无
**********************************/
void cursorMovement(void){
	uint8_t local = (2*gui.main_MeunPosition-1)%8;
	uint8_t prelocal = (2*gui.main_PreMeunPosition-1)%8;

	cleanPiont_8x8(15,prelocal);
	drowPiont_8x8(15,local);
}

/**********************************
 * 函数名：windowMovement_up
 * 功能：窗口上届上移一位
 * 输入：qu：保存有显示数据的环形队列；window_x：窗口界限
 * 输出：无
 * 备注：windowMovement内部调用
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
 * 函数名：windowMovement_down
 * 功能：窗口上届下移一位
 * 输入：window_x：窗口界限
 * 输出：无
 * 备注：windowMovement内部调用
**********************************/
static void windowMovement_down(windowLim_TypeDef* window_x){
	Node_TypeDef* temp_node = (Node_TypeDef*)window_x->windowUpper;
	window_x->windowUpper = temp_node->next;
	window_x->windowLower = temp_node->next->next->next->next;
}
/**********************************
 * 函数名：windowMovement
 * 功能：窗口滚动
 * 输入：qu：保存有显示数据的环形队列；window_x：窗口界限
 * 输出：无
 * 备注：无
**********************************/
void windowMovement(Circle_LinkQueue_TypeDef* qu, windowLim_TypeDef* window_x){
	// 判断是往前移动给还是往后移动
	// 位置++，由小变大
	if(gui.fileList_PreIndex < gui.fileList_Index ){
		// 判断是否为位置--的交界点，交界点则上移
		if(gui.fileList_PreIndex == 1 && gui.fileList_Index == qu->queue_nodeNum){
			windowMovement_up(qu, window_x);
		}
		// 不是位置--交界点，下移
		else{
			windowMovement_down(window_x);		
		}
	}
	
	// 位置--，由大变小
	else {
		// 位置++交界点，下移
		if(gui.fileList_PreIndex == qu->queue_nodeNum && gui.fileList_Index == 1){
			windowMovement_down(window_x);
		}
		// 非位置++交界点，上移
		else{
			windowMovement_up(qu, window_x);
		}
	}

	// 定好窗口的界限后，开始显示窗口
	Node_TypeDef* showstr = (Node_TypeDef*)window_x->windowUpper;

	for(int i = 0; i<4; ++i){
		oled_ShowStr(0,2*i,showstr->fileName,small);
		showstr = showstr->next;
	}

}




//主菜单显示
void meunInterFace_1(uint8_t (*fileList)[FileList_SIZE]){
	// oled_Cls();
	
	// 文件数小于4，不用翻页，光标滚动
	if(gui.showQueue == NULL){
		cursorMovement();
	}
	// 文件数大于4，翻页显示
	else{
		if( gui.main_MeunPosition != gui.main_PreMeunPosition ){
			// 光标滚动
			cursorMovement();
		}
		else {
			// 内容滚动
			windowMovement( gui.showQueue, &(gui.windowLimite) );
		}

	}
	
}

//菜单功能初始化
void menuInit(void){
	//菜单首页显示
	// gui.Interface_Mark = Menu_Interface_1;
	
//	gui.Menu_Interface_Flag = 0;//默认开机主界面
	gui.fileList_PreIndex = 1;//1号菜单指针初始化
	gui.fileList_Index = 1;//1号菜单指针初始化
	
	gui.main_PreMeunPosition = 1;//1号菜单指针初始化
	gui.main_MeunPosition = 1;//1号菜单指针初始化
	
	// gui.fileList_showIndex = 0;
	
	gui.windowLimite.windowLower = gui.windowLimite.windowLower = NULL;
	gui.showQueue = NULL;
	
	
	mainInterFaceInit();
}

uint8_t meunTaskRun(uint8_t (*fileList)[FileList_SIZE]){
	//主界面操作逻辑
	
	//主菜单_1 操作逻辑
	// if(gui.Interface_Mark == Menu_Interface_1){
		//上键功能:选择需要加载的程序
		if(up_key_parameter.key_event.shortPressed){
			// 清除按键事件标志位
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

		//下键功能:选择需要加载的程序
		if(down_key_parameter.key_event.shortPressed){
			// 清除按键事件标志位
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
			// 确认程序并且加载
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

