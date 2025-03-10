#include "bsp_key.h"
#include "delay.h" 

key_parameter_TypeDef fun_key_parameter;
key_parameter_TypeDef up_key_parameter;
key_parameter_TypeDef down_key_parameter;

uint8_t key_press_flag;//���ⰴ�����±�־,���ⲿ�ж��н��и�ֵ
uint8_t keyScan_delay_En;//�ӳ�ɨ��ʹ�ܱ�־λ
uint16_t GetKeyValue_delayTime_ms;//����ɨ�����ڼ�ʱ��

//������ʼ������
void KEY_Init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//GPIOEʱ��

	/*�������ų�ʼ��*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4; //KEY_UP KEY_DOWN��Ӧ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE3,4

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//WK_UP��Ӧ����PE2
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2
	/* end of �������ų�ʼ�� */

	/*�ⲿ�ж����ó�ʼ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 ���ӵ��ж���2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);//PE3 ���ӵ��ж���3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);//PE4 ���ӵ��ж���4

	/* ����EXTI_Line2,3,4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure);//����

	EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure);//����

	// �����������ж����ȼ���ȣ�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//�ⲿ�ж�2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//�ⲿ�ж�3
	NVIC_Init(&NVIC_InitStructure);//����

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//�ⲿ�ж�4
	NVIC_Init(&NVIC_InitStructure);//����
	/*end of �ⲿ�ж����ó�ʼ��*/

	/*���������ṹ���ʼ��*/
	fun_key_parameter.keyGPIO 	= GPIOE;
	up_key_parameter.keyGPIO 	= GPIOE;
	down_key_parameter.keyGPIO 	= GPIOE;
	
	fun_key_parameter.keyGPIO_Pin 	= GPIO_Pin_2;
	up_key_parameter.keyGPIO_Pin 	= GPIO_Pin_3;
	down_key_parameter.keyGPIO_Pin 	= GPIO_Pin_4;

	fun_key_parameter.keytype = P_KEY;
	up_key_parameter.keytype = N_KEY;
	down_key_parameter.keytype = N_KEY;
	/*end of ���������ṹ���ʼ��*/

} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��WKUP���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP!!
//u8 KEY_Scan(u8 mode){ 
// 	static u8 key_up=1;//�������ɿ���־
// 	if(mode)key_up=1;  //֧������		  
// 	if(key_up && (KEY_UP==0||KEY_DOWN==0||KEY_FUN==1))
// 	{
// 		// delay_ms(10);//ȥ���� 
// 		key_up=0;
// 		if(KEY_UP==0)return KEY_UP_PRES;
// 		else if(KEY_DOWN==0)return KEY_DOWN_PRES;
// 		else if(KEY_FUN==1)return KEY_FUN_PRES;

// 	}else if(KEY_UP==1 && KEY_DOWN==1&&KEY_FUN==0)key_up=1; 	    
//  	return 0;// �ް�������
//}

/********************************************
 * ��	��	����judge_KeyValue
 * ��		�ܣ�������ֵ��״̬�жϺ���
 * ��		�룺���������ṹ��
 * ��		������
 * ��		ע��ʹ�ô˺�������Ҫ��˵������������GPIO,pin
 * 				��������ʱ��ͼ�ֵ��
 * 				2S��				��������ģʽ
 * 				1500ms��				����
 * 				1200ms�ڶ�ΰ��£� 	���
********************************************/
void judge_KeyValue(key_parameter_TypeDef * keyValue){
	// �жϰ�������
	uint8_t KEY_PRESSED,KEY_UNPRESSED;
	KEY_PRESSED = (keyValue->keytype == N_KEY) ? N_KEY_PRESSED : P_KEY_PRESSED;
	KEY_UNPRESSED = (keyValue->keytype == N_KEY) ? N_KEY_UNPRESSED : P_KEY_UNPRESSED;
	
	//������
	/*�ж��Ƿ��ڳ���*/
	//����������ڰ���״̬
	if(GPIO_ReadInputDataBit(keyValue->keyGPIO, keyValue->keyGPIO_Pin) == KEY_PRESSED){
		if(keyValue->key_event._if_keyPress == 0){
			keyValue->key_event._if_keyPress = 1;
			keyValue->press_time_ms = 0;		//������ʱ��ʼ
		}
		//����1s����������ģʽ
		else if(keyValue->key_event._if_keyPress == 1 && keyValue->press_time_ms >= 2000){
			keyValue->key_event.isLongPressing = 1;	
		}
	}
	
	/*����������ڵ���״̬״̬����֮ǰ�б����£��򽫸��ݰ���ʱ��ȷ����ֵ*/
	else if( (GPIO_ReadInputDataBit(keyValue->keyGPIO, keyValue->keyGPIO_Pin)==KEY_UNPRESSED) && 
				keyValue->key_event._if_keyPress == 1)
	{
		keyValue->key_event._if_keyPress = 0;
		keyValue->key_event.isLongPressing = 0;	
		if(keyValue->press_time_ms >= 1500){
			keyValue->key_event.longPressed = 1;
			keyValue->key_event.shortPressed = 0;
			keyValue->key_event.doublePressed = 0;
		}
		
		/*���Ѿ�����һ�ζ̰��¼����ٴζ̰�������˫���¼�*/
		else if(keyValue->press_count == 1 && keyValue->next_press_time_ms <= 1200){
			keyValue->press_count = 0;
			keyValue->next_press_timeEn = 0;	//ֹͣ �����ٴΰ��µļ�ʱ
			
			keyValue->key_event.longPressed = 0;
			keyValue->key_event.shortPressed = 0;
			keyValue->key_event.doublePressed = 1;
		}
		else{
			/*�����һ�ΰ������¼����ʱ������ʼ��ʱ*/
			/*������250msû�а������£��ڶ�ʱ���ж����ж�Ϊ��������ֹͣnext_press_time_ms�ļ�ʱ*/
			keyValue->next_press_time_ms = 0;
			keyValue->next_press_timeEn = 1;

			keyValue->press_count++;
		}
		key_press_flag = 0;	
//		myKey_ValueChangedFlag = 1;		//��ֵ�ı��־λ
	}
}

/********************************************
 * ��	��	����set_singlePress
 * ��		�ܣ��ж�����Ϊ����
 * ��		�룺���������ṹ��
 * ��		������
 * ��		ע���ڶ�ʱ���ж��е���
********************************************/
void set_singlePress(key_parameter_TypeDef* key_parameter){
	if(key_parameter->next_press_timeEn){
		key_parameter->next_press_time_ms++;
		//����250msû�а����ٴΰ��£�ֹͣ��ʱ
		if(key_parameter->next_press_time_ms >= 250){
			key_parameter->next_press_timeEn = 0;
			//�ж�Ϊ����
			key_parameter->press_count = 0;

			key_parameter->key_event.longPressed = 0;
			key_parameter->key_event.shortPressed = 1;
			key_parameter->key_event.doublePressed = 0;
		}
	}
}
/********************************************
*��	��	����GetKeyValue
*��		�ܣ���ȡ���°�����ֵ��״̬
*��		�룺����ɨ�����ڣ���λms��������Ҫ�ӳ٣�����NULL��
*��		������
*��		ע��������󣬼ǵö԰���������־λ���и�λ
********************************************/
void GetKeyValue(uint16_t GetKeyValue_delayTime){
	//��־λΪ1ʱ����ʼ�ӳ�ɨ�����
	if(GetKeyValue_delayTime){
		 keyScan_delay_En = 1;
	}
	else {
		keyScan_delay_En = 0;
		GetKeyValue_delayTime_ms = 0;
	}
	
	//�ж�����а����������� �����Ұ���ɨ�����ڵ�����Ӧʱ������ʼɨ�谴��
	if(key_press_flag == 1 && GetKeyValue_delayTime_ms >= GetKeyValue_delayTime){
		/*�˴�������İ���*/
		judge_KeyValue(&fun_key_parameter);
		judge_KeyValue(&up_key_parameter);
		judge_KeyValue(&down_key_parameter);
		
		GetKeyValue_delayTime_ms = 0;//�������ɨ�����ڼ���	
	}
}





















