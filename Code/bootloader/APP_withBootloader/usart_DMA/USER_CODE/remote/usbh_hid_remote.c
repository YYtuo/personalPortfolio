#include "usbh_hid_remote.h"
#include "bsp_beep.h"
#include "bsp_led.h"

static void  REMOTE_Init (void);
static void  REMOTE_Decode(uint8_t *data);

HID_cb_TypeDef HID_REMOTE_cb = {
	REMOTE_Init,
	REMOTE_Decode
};

Remote_state_TypeDef HID_Remote;

// uint8_t remote_en = 0;	// start键:记录遥控器权限，0：遥控器下控，1：遥控器上控

/**********************************
 * 函数名：remote_getPermission
 * 功能：获取当前遥控权限
 * 输入：遥控器结构体
 * 输出：0：遥控器下控，1：遥控器上控
 * 备注：无
**********************************/
uint8_t remote_getPermission(Remote_state_TypeDef* Remote_data){
	return Remote_data->remote_en;
}

/**********************************
 * 函数名：remote_getMode
 * 功能：获取当前遥控的模式
 * 输入：遥控器结构体
 * 输出：遥控器模式
 * 备注：无
**********************************/
Remote_Mode remote_getMode(Remote_state_TypeDef* Remote_data){
	return Remote_data->MODE;
}

/**********************************
 * 函数名：get_Remote_key
 * 功能：获取当前遥控按下的按键
 * 输入：遥控器结构体
 * 输出：按键键值
 * 备注：无
**********************************/
// uint8_t remote_getKey(Remote_state_TypeDef* Remote_data){
// 	return 0;
// }

/**********************************
 * 函数名:remote_getRockerVal
 * 功能:获取当前遥控摇杆的ad值
 * 输入:Remote_data:遥控器结构体
 *		rocker_NO_:摇杆序号
 * 输出：摇杆ad值
 * 备注：无
**********************************/
uint8_t remote_getRockerVal(Remote_state_TypeDef* Remote_data, Remote_Rocker_NO_ rocker_NO_){
	uint8_t val = 0;
	switch (rocker_NO_){
		case 0:return 0;
		// 输出第一个摇杆ad值
		case left_rocker_lr:
			val = Remote_data->rockerValReg.left_rocker_lr;
			break;

		// 输出第二个摇杆ad值
		case left_rocker_ud:
			val = Remote_data->rockerValReg.left_rocker_ud;
			break;
			
		// 输出第三个摇杆ad值
		case right_rocker_lr:
			val = Remote_data->rockerValReg.right_rocker_lr;
			break;

		// 输出第四个摇杆ad值
		case right_rocker_ud:
			val = Remote_data->rockerValReg.right_rocker_ud;
			break;

		default:
			val = 0;
			break;
	}
	return val;
}

/**********************************
 * 函数名：remote_getPressureVal
 * 功能：获取当前遥控按键按下的压力值
 * 输入：Remote_data:遥控器结构体
 * 		key_NO_:按键序号
 * 输出：按键压力值
 * 备注：无
**********************************/
uint8_t remote_getPressureVal(Remote_state_TypeDef* Remote_data,Remote_Key_NO_ key_NO_){
	uint8_t val = 0;
	switch (key_NO_){
		case 0:return 0;

		// 输出 右 按键值
		case right_key:
			val = Remote_data->keyPressure.right_key;
			break;

		// 输出 左 按键值
		case left_key:
			val = Remote_data->keyPressure.left_key;
			break;
			
		// 输出 上 按键值
		case up_key:
			val = Remote_data->keyPressure.up_key;
			break;

		// 输出 下 按键值
		case down_key:
			val = Remote_data->keyPressure.down_key;
			break;

		// 输出 三角形 按键值
		case triangle:
			val = Remote_data->keyPressure.triangle;
			break;

		// 输出 圆形 按键值
		case circular:
			val = Remote_data->keyPressure.circular;
			break;

		// 输出 叉形 按键值
		case fork:
			val = Remote_data->keyPressure.fork;
			break;

		// 输出 正方形 按键值
		case square:
			val = Remote_data->keyPressure.square;
			break;

		// 输出 L1 按键值
		case L1:
			val = Remote_data->keyPressure.L1;
			break;

		// 输出 R1 按键值
		case R1:
			val = Remote_data->keyPressure.R1;
			break;

		// 输出 L2 按键值
		case L2:
			val = Remote_data->keyPressure.L2;
			break;

		// 输出 R2 按键值
		case R2:
			val = Remote_data->keyPressure.R2;
			break;

		default:
			val = 0;
			break;
	}
	return val;
}

/**********************************
 * 函数名：remote_getPressureTimes
 * 功能：获取当前遥控按键按下的次数
 * 输入：Remote_data:遥控器结构体
 * 		key_NO_:按键序号
 * 输出：按键按下次数
 * 备注：无
**********************************/
uint8_t remote_getPressureTimes(Remote_state_TypeDef* Remote_data,Remote_Key_NO_ key_NO_){
	uint8_t times = 0;
	switch (key_NO_){
		case 0:return 0;

		case right_key:
			times = Remote_data->keyPressedTimes.right_key;
			break;

		// 输出 左 键按下次数
		case left_key:
			times = Remote_data->keyPressedTimes.left_key;
			break;
			
		// 输出 上 键按下次数
		case up_key:
			times = Remote_data->keyPressedTimes.up_key;
			break;

		// 输出 下 键按下次数
		case down_key:
			times = Remote_data->keyPressedTimes.down_key;
			break;

		// 输出 三角形 键按下次数
		case triangle:
			times = Remote_data->keyPressedTimes.triangle;
			break;

		// 输出 圆形 键按下次数
		case circular:
			times = Remote_data->keyPressedTimes.circular;
			break;

		// 输出 叉形 键按下次数
		case fork:
			times = Remote_data->keyPressedTimes.fork;
			break;

		// 输出 正方形 键按下次数
		case square:
			times = Remote_data->keyPressedTimes.square;
			break;

		// 输出 L1 键按下次数
		case L1:
			times = Remote_data->keyPressedTimes.L1;
			break;

		// 输出 R1 键按下次数
		case R1:
			times = Remote_data->keyPressedTimes.R1;
			break;

		// 输出 L2 键按下次数
		case L2:
			times = Remote_data->keyPressedTimes.L2;
			break;

		// 输出 R2 键按下次数
		case R2:
			times = Remote_data->keyPressedTimes.R2;
			break;

		// 输出 select 键按下次数
		case select:
			times = Remote_data->keyPressedTimes.select;
			break;

		// 输出 start 键按下次数
		case start:
			times = Remote_data->keyPressedTimes.start;
			break;

		// 输出 left_rocker 键按下次数
		case left_rocker:
			times = Remote_data->keyPressedTimes.left_rocker;
			break;

		// 输出 right_rocker 键按下次数
		case right_rocker:
			times = Remote_data->keyPressedTimes.right_rocker;
			break;

		// 输出 mode 键按下次数
		case mode:
			times = Remote_data->keyPressedTimes.mode;
			break;

		default:
			times = 0;
			break;
	}
	return times;
}

/*
初始化函数
没有特别的初始化流程，所以只打印信息
*/
static void  REMOTE_Init (void){
	/* Call User Init*/
	USR_REMOTE_Init();
	HID_Remote.judge_mode_flag = 1;
}

/*
解码函数
*pbuf:USB接收缓冲区
*/
static void REMOTE_Decode(uint8_t *pbuf){
	//开始解码
	uint8_t *temp_point = pbuf;

	//识别遥控器模式,初始化时，只进入一次
	if(HID_Remote.judge_mode_flag){
		if( pbuf[4] == 0x7f && pbuf[6] == 0x7f ){
			//红灯模式，摇杆输出模拟量
			HID_Remote.MODE = HID_Remote.preMODE = red_mode;	
		}
		else if( pbuf[4] == 0x80 && pbuf[6] == 0x80 ){
			//绿灯模式，摇杆输出数字量
			HID_Remote.MODE = HID_Remote.preMODE = green_mode;
		}
		else {
			HID_Remote.MODE = remote_mode_error;
			HID_Remote.judge_mode_flag = 1;
			return;
		}
		HID_Remote.judge_mode_flag = 0;
	}
	
	/*end of 识别遥控器模式*/

	/*识别遥控器按键*/
	HID_Remote.primaryKeyReg = *((Remote_primaryKeyReg_TypeDef*)pbuf);
	HID_Remote.funtionKeyReg = *((Remote_funtionKeyReg_TypeDef*)(pbuf+1));
//	temp_point = pbuf;

	//两种模式下，方向键解码有区别
	if(HID_Remote.MODE == red_mode){
		//红灯模式下解码
		HID_Remote.DirectionKey = pbuf[2];
	}
	else{
		//绿灯模式下DirectionKey与Remote_rockerValReg内存合并
		;
	}
	/*end of 识别遥控器按键*/

	/*识别遥控器摇杆*/

	HID_Remote.rockerValReg = *((Remote_rockerValReg_TypeDef*)(pbuf+3));

	/*end of 识别遥控器摇杆*/
	
	/*识别遥控器按键压力*/
	HID_Remote.keyPressure = *((Remote_KeyPressureVal_TypeDef*)(temp_point+7));
	/*end of 识别遥控器按键压力*/
	
	USR_REMOTE_ProcessData(&HID_Remote);//用户数据处理函数
}

/**********************************
 * 函数名：_if_key_pressed
 * 功能：判断某一个按键是否被按下
 * 输入：按键输入信号
 * 输出：0：没有被按下；1：被按下
 * 备注：不识别连按，长按算一次
**********************************/
uint8_t  _if_key_pressed(uint8_t key_signal){
	return 0;
}

/**********************************
 * 函数名：remote_setPermission
 * 功能：设置遥控器状态
 * 输入：RemoteData：遥控器数据结构体
 * 输出：0：下控；1：上控
 * 备注：无
**********************************/
uint8_t remote_setPermission(Remote_state_TypeDef *Remote_data){
	static uint8_t key_press = 0;	// 用于检测按键是否按下

	// 设置遥控权限，连按不会连续识别
	if(Remote_data->funtionKeyReg.start == 1 && key_press == 0){
		key_press = 1; // 检测到按键按下

		if(Remote_data->remote_en){
			Remote_data->remote_en = 0;
			
			#ifdef __BSP_LED_H
			LED_G = 1;
			#endif
			
			#ifdef __BSP_BEEP_H
			beep_remoteDis();
			#endif
			
			
		}
		else{
			Remote_data->remote_en = 1;
			
			#ifdef __BSP_LED_H
			LED_G = 0;
			#endif
			
			#ifdef __BSP_BEEP_H
			beep_remoteEn();
			#endif
			
			
		}
	}

	// 检测到按键松开
	else if (Remote_data->funtionKeyReg.start == 0){
		key_press = 0;
	}

	return Remote_data->remote_en;
}

/**********************************
 * 函数名：remote_changeMode
 * 功能：获取当前遥控的模式
 * 输入：遥控器结构体
 * 输出：遥控器模式
 * 备注：无
**********************************/
Remote_Mode remote_changeMode(Remote_state_TypeDef* Remote_data){
	static uint8_t key_press = 0;	// 用于检测按键是否按下

	if(Remote_data->funtionKeyReg.mode == 1 && key_press == 0){
		key_press = 1; // 检测到按键按下
		if(Remote_data->MODE == remote_mode_error)return remote_mode_error;

		if(Remote_data->MODE == red_mode){
			Remote_data->preMODE = Remote_data->MODE;
			Remote_data->MODE = green_mode;
		}
		else if(Remote_data->MODE == green_mode){
			Remote_data->preMODE = Remote_data->MODE;
			Remote_data->MODE = red_mode;
		}
	}

	// 检测到按键松开
	else if (Remote_data->funtionKeyReg.mode == 0){
		key_press = 0;
	}

	return Remote_data->MODE;
}

/**********************************
 * 函数名：record_keyPressedTimes
 * 功能：记录遥控按键按下的次数
 * 输入：RemoteData:遥控器结构体
 * 		key_NO_:按键序号
 * 输出：无
 * 备注：不支持连按，按下后松开，记为一次
**********************************/
void record_keyPressedTimes(Remote_state_TypeDef *RemoteData, Remote_Key_NO_ key_NO_){
	static uint8_t key_press = 0;	// 用于检测按键是否按下
	uint8_t key_index = 0;			//取出按键
	uint8_t* record_index = 0;		//记录按下的按键的次数
	
	uint8_t _if_directionKey = 0;
	// 按键选择初始化
	switch(key_NO_){
		case right_key:
			_if_directionKey = 1;
			key_index = RemoteData->DirectionKey;
			record_index = &RemoteData->keyPressedTimes.right_key;
		break;
		
 		case left_key:
			_if_directionKey = 1;
			key_index = RemoteData->DirectionKey;
			record_index = &RemoteData->keyPressedTimes.left_key;
		break;
		
 		case up_key:
			_if_directionKey = 1;
			key_index = RemoteData->DirectionKey;
			record_index = &RemoteData->keyPressedTimes.up_key;
		break;
		
 		case down_key:
			_if_directionKey = 1;
			key_index = RemoteData->DirectionKey;
			record_index = &RemoteData->keyPressedTimes.down_key;
		break;
		
 		case triangle:
			key_index = RemoteData->primaryKeyReg.triangle;
			record_index = &RemoteData->keyPressedTimes.triangle;
		break;
		
 		case circular:
			key_index = RemoteData->primaryKeyReg.circular;
			record_index = &RemoteData->keyPressedTimes.circular;
		break;
		
 		case fork:
			key_index = RemoteData->primaryKeyReg.fork;
			record_index = &RemoteData->keyPressedTimes.fork;
		break;
		
 		case square:
			key_index = RemoteData->primaryKeyReg.square;
			record_index = &RemoteData->keyPressedTimes.square;
		break;
		
 		case L1:
			key_index = RemoteData->primaryKeyReg.L1;
			record_index = &RemoteData->keyPressedTimes.L1;
		break;
		
 		case R1:
			key_index = RemoteData->primaryKeyReg.R1;
			record_index = &RemoteData->keyPressedTimes.R1;
		break;
		
 		case L2:
			key_index = RemoteData->primaryKeyReg.L2;
			record_index = &RemoteData->keyPressedTimes.L2;
		break;
		
 		case R2:
			key_index = RemoteData->primaryKeyReg.R2;
			record_index = &RemoteData->keyPressedTimes.R2;
		break;
		
 		case select:
			key_index = RemoteData->funtionKeyReg.select;
			record_index = &RemoteData->keyPressedTimes.select;
		break;
		
 		case start:
			key_index = RemoteData->funtionKeyReg.start;
			record_index = &RemoteData->keyPressedTimes.start;
		break;
		
 		case left_rocker:
			key_index = RemoteData->funtionKeyReg.left_rocker;
			record_index = &RemoteData->keyPressedTimes.left_rocker;
		break;
		
 		case right_rocker:
			key_index = RemoteData->funtionKeyReg.right_rocker;
			record_index = &RemoteData->keyPressedTimes.right_rocker;
		break;
		
 		case mode:
			key_index = RemoteData->funtionKeyReg.mode;
			record_index = &RemoteData->keyPressedTimes.mode;
		break;
		
	}

	if(_if_directionKey){
		// 方向键解析并计数
		// 方向键暂时不进行计数
	}

	// 其他按键计数
	else{
		// start键计数
		if(key_index == 1 && key_press == 0){
			key_press = 1; // 检测到按键按下
		}

		// 此时按键一直按着，没有松开，这里不作任何处理
		else if(key_index == 1 && key_press == 1){
			
		}
		
		// 此时按键刚刚松开，计数器+1
		else if(key_index == 0 && key_press == 1){
			(*record_index) ++ ;
			key_press = 0;
		}

		// 检测到按键松开
		else if (key_index == 0){
			key_press = 0;
		}
	}
	
}












