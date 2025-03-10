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

// uint8_t remote_en = 0;	// start��:��¼ң����Ȩ�ޣ�0��ң�����¿أ�1��ң�����Ͽ�

/**********************************
 * ��������remote_getPermission
 * ���ܣ���ȡ��ǰң��Ȩ��
 * ���룺ң�����ṹ��
 * �����0��ң�����¿أ�1��ң�����Ͽ�
 * ��ע����
**********************************/
uint8_t remote_getPermission(Remote_state_TypeDef* Remote_data){
	return Remote_data->remote_en;
}

/**********************************
 * ��������remote_getMode
 * ���ܣ���ȡ��ǰң�ص�ģʽ
 * ���룺ң�����ṹ��
 * �����ң����ģʽ
 * ��ע����
**********************************/
Remote_Mode remote_getMode(Remote_state_TypeDef* Remote_data){
	return Remote_data->MODE;
}

/**********************************
 * ��������get_Remote_key
 * ���ܣ���ȡ��ǰң�ذ��µİ���
 * ���룺ң�����ṹ��
 * �����������ֵ
 * ��ע����
**********************************/
// uint8_t remote_getKey(Remote_state_TypeDef* Remote_data){
// 	return 0;
// }

/**********************************
 * ������:remote_getRockerVal
 * ����:��ȡ��ǰң��ҡ�˵�adֵ
 * ����:Remote_data:ң�����ṹ��
 *		rocker_NO_:ҡ�����
 * �����ҡ��adֵ
 * ��ע����
**********************************/
uint8_t remote_getRockerVal(Remote_state_TypeDef* Remote_data, Remote_Rocker_NO_ rocker_NO_){
	uint8_t val = 0;
	switch (rocker_NO_){
		case 0:return 0;
		// �����һ��ҡ��adֵ
		case left_rocker_lr:
			val = Remote_data->rockerValReg.left_rocker_lr;
			break;

		// ����ڶ���ҡ��adֵ
		case left_rocker_ud:
			val = Remote_data->rockerValReg.left_rocker_ud;
			break;
			
		// ���������ҡ��adֵ
		case right_rocker_lr:
			val = Remote_data->rockerValReg.right_rocker_lr;
			break;

		// ������ĸ�ҡ��adֵ
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
 * ��������remote_getPressureVal
 * ���ܣ���ȡ��ǰң�ذ������µ�ѹ��ֵ
 * ���룺Remote_data:ң�����ṹ��
 * 		key_NO_:�������
 * ���������ѹ��ֵ
 * ��ע����
**********************************/
uint8_t remote_getPressureVal(Remote_state_TypeDef* Remote_data,Remote_Key_NO_ key_NO_){
	uint8_t val = 0;
	switch (key_NO_){
		case 0:return 0;

		// ��� �� ����ֵ
		case right_key:
			val = Remote_data->keyPressure.right_key;
			break;

		// ��� �� ����ֵ
		case left_key:
			val = Remote_data->keyPressure.left_key;
			break;
			
		// ��� �� ����ֵ
		case up_key:
			val = Remote_data->keyPressure.up_key;
			break;

		// ��� �� ����ֵ
		case down_key:
			val = Remote_data->keyPressure.down_key;
			break;

		// ��� ������ ����ֵ
		case triangle:
			val = Remote_data->keyPressure.triangle;
			break;

		// ��� Բ�� ����ֵ
		case circular:
			val = Remote_data->keyPressure.circular;
			break;

		// ��� ���� ����ֵ
		case fork:
			val = Remote_data->keyPressure.fork;
			break;

		// ��� ������ ����ֵ
		case square:
			val = Remote_data->keyPressure.square;
			break;

		// ��� L1 ����ֵ
		case L1:
			val = Remote_data->keyPressure.L1;
			break;

		// ��� R1 ����ֵ
		case R1:
			val = Remote_data->keyPressure.R1;
			break;

		// ��� L2 ����ֵ
		case L2:
			val = Remote_data->keyPressure.L2;
			break;

		// ��� R2 ����ֵ
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
 * ��������remote_getPressureTimes
 * ���ܣ���ȡ��ǰң�ذ������µĴ���
 * ���룺Remote_data:ң�����ṹ��
 * 		key_NO_:�������
 * ������������´���
 * ��ע����
**********************************/
uint8_t remote_getPressureTimes(Remote_state_TypeDef* Remote_data,Remote_Key_NO_ key_NO_){
	uint8_t times = 0;
	switch (key_NO_){
		case 0:return 0;

		case right_key:
			times = Remote_data->keyPressedTimes.right_key;
			break;

		// ��� �� �����´���
		case left_key:
			times = Remote_data->keyPressedTimes.left_key;
			break;
			
		// ��� �� �����´���
		case up_key:
			times = Remote_data->keyPressedTimes.up_key;
			break;

		// ��� �� �����´���
		case down_key:
			times = Remote_data->keyPressedTimes.down_key;
			break;

		// ��� ������ �����´���
		case triangle:
			times = Remote_data->keyPressedTimes.triangle;
			break;

		// ��� Բ�� �����´���
		case circular:
			times = Remote_data->keyPressedTimes.circular;
			break;

		// ��� ���� �����´���
		case fork:
			times = Remote_data->keyPressedTimes.fork;
			break;

		// ��� ������ �����´���
		case square:
			times = Remote_data->keyPressedTimes.square;
			break;

		// ��� L1 �����´���
		case L1:
			times = Remote_data->keyPressedTimes.L1;
			break;

		// ��� R1 �����´���
		case R1:
			times = Remote_data->keyPressedTimes.R1;
			break;

		// ��� L2 �����´���
		case L2:
			times = Remote_data->keyPressedTimes.L2;
			break;

		// ��� R2 �����´���
		case R2:
			times = Remote_data->keyPressedTimes.R2;
			break;

		// ��� select �����´���
		case select:
			times = Remote_data->keyPressedTimes.select;
			break;

		// ��� start �����´���
		case start:
			times = Remote_data->keyPressedTimes.start;
			break;

		// ��� left_rocker �����´���
		case left_rocker:
			times = Remote_data->keyPressedTimes.left_rocker;
			break;

		// ��� right_rocker �����´���
		case right_rocker:
			times = Remote_data->keyPressedTimes.right_rocker;
			break;

		// ��� mode �����´���
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
��ʼ������
û���ر�ĳ�ʼ�����̣�����ֻ��ӡ��Ϣ
*/
static void  REMOTE_Init (void){
	/* Call User Init*/
	USR_REMOTE_Init();
	HID_Remote.judge_mode_flag = 1;
}

/*
���뺯��
*pbuf:USB���ջ�����
*/
static void REMOTE_Decode(uint8_t *pbuf){
	//��ʼ����
	uint8_t *temp_point = pbuf;

	//ʶ��ң����ģʽ,��ʼ��ʱ��ֻ����һ��
	if(HID_Remote.judge_mode_flag){
		if( pbuf[4] == 0x7f && pbuf[6] == 0x7f ){
			//���ģʽ��ҡ�����ģ����
			HID_Remote.MODE = HID_Remote.preMODE = red_mode;	
		}
		else if( pbuf[4] == 0x80 && pbuf[6] == 0x80 ){
			//�̵�ģʽ��ҡ�����������
			HID_Remote.MODE = HID_Remote.preMODE = green_mode;
		}
		else {
			HID_Remote.MODE = remote_mode_error;
			HID_Remote.judge_mode_flag = 1;
			return;
		}
		HID_Remote.judge_mode_flag = 0;
	}
	
	/*end of ʶ��ң����ģʽ*/

	/*ʶ��ң��������*/
	HID_Remote.primaryKeyReg = *((Remote_primaryKeyReg_TypeDef*)pbuf);
	HID_Remote.funtionKeyReg = *((Remote_funtionKeyReg_TypeDef*)(pbuf+1));
//	temp_point = pbuf;

	//����ģʽ�£����������������
	if(HID_Remote.MODE == red_mode){
		//���ģʽ�½���
		HID_Remote.DirectionKey = pbuf[2];
	}
	else{
		//�̵�ģʽ��DirectionKey��Remote_rockerValReg�ڴ�ϲ�
		;
	}
	/*end of ʶ��ң��������*/

	/*ʶ��ң����ҡ��*/

	HID_Remote.rockerValReg = *((Remote_rockerValReg_TypeDef*)(pbuf+3));

	/*end of ʶ��ң����ҡ��*/
	
	/*ʶ��ң��������ѹ��*/
	HID_Remote.keyPressure = *((Remote_KeyPressureVal_TypeDef*)(temp_point+7));
	/*end of ʶ��ң��������ѹ��*/
	
	USR_REMOTE_ProcessData(&HID_Remote);//�û����ݴ�����
}

/**********************************
 * ��������_if_key_pressed
 * ���ܣ��ж�ĳһ�������Ƿ񱻰���
 * ���룺���������ź�
 * �����0��û�б����£�1��������
 * ��ע����ʶ��������������һ��
**********************************/
uint8_t  _if_key_pressed(uint8_t key_signal){
	return 0;
}

/**********************************
 * ��������remote_setPermission
 * ���ܣ�����ң����״̬
 * ���룺RemoteData��ң�������ݽṹ��
 * �����0���¿أ�1���Ͽ�
 * ��ע����
**********************************/
uint8_t remote_setPermission(Remote_state_TypeDef *Remote_data){
	static uint8_t key_press = 0;	// ���ڼ�ⰴ���Ƿ���

	// ����ң��Ȩ�ޣ�������������ʶ��
	if(Remote_data->funtionKeyReg.start == 1 && key_press == 0){
		key_press = 1; // ��⵽��������

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

	// ��⵽�����ɿ�
	else if (Remote_data->funtionKeyReg.start == 0){
		key_press = 0;
	}

	return Remote_data->remote_en;
}

/**********************************
 * ��������remote_changeMode
 * ���ܣ���ȡ��ǰң�ص�ģʽ
 * ���룺ң�����ṹ��
 * �����ң����ģʽ
 * ��ע����
**********************************/
Remote_Mode remote_changeMode(Remote_state_TypeDef* Remote_data){
	static uint8_t key_press = 0;	// ���ڼ�ⰴ���Ƿ���

	if(Remote_data->funtionKeyReg.mode == 1 && key_press == 0){
		key_press = 1; // ��⵽��������
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

	// ��⵽�����ɿ�
	else if (Remote_data->funtionKeyReg.mode == 0){
		key_press = 0;
	}

	return Remote_data->MODE;
}

/**********************************
 * ��������record_keyPressedTimes
 * ���ܣ���¼ң�ذ������µĴ���
 * ���룺RemoteData:ң�����ṹ��
 * 		key_NO_:�������
 * �������
 * ��ע����֧�����������º��ɿ�����Ϊһ��
**********************************/
void record_keyPressedTimes(Remote_state_TypeDef *RemoteData, Remote_Key_NO_ key_NO_){
	static uint8_t key_press = 0;	// ���ڼ�ⰴ���Ƿ���
	uint8_t key_index = 0;			//ȡ������
	uint8_t* record_index = 0;		//��¼���µİ����Ĵ���
	
	uint8_t _if_directionKey = 0;
	// ����ѡ���ʼ��
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
		// ���������������
		// �������ʱ�����м���
	}

	// ������������
	else{
		// start������
		if(key_index == 1 && key_press == 0){
			key_press = 1; // ��⵽��������
		}

		// ��ʱ����һֱ���ţ�û���ɿ������ﲻ���κδ���
		else if(key_index == 1 && key_press == 1){
			
		}
		
		// ��ʱ�����ո��ɿ���������+1
		else if(key_index == 0 && key_press == 1){
			(*record_index) ++ ;
			key_press = 0;
		}

		// ��⵽�����ɿ�
		else if (key_index == 0){
			key_press = 0;
		}
	}
	
}












