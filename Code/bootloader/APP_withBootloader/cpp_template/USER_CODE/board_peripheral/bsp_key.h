#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#ifndef __BSP_KEY_H
#define __BSP_KEY_H	 

#include "BSP_GPIO.h"

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
/*
#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0
*/

/*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/

#define KEY_UP 		PEin(3)   	//PE3
#define KEY_DOWN	PEin(4)		//PE4 
#define KEY_FUN 	    PEin(2)		//PE2

#define KEY_DOWN_PRES	1
#define KEY_UP_PRES 	2
#define KEY_FUN_PRES   3

// �������ͣ�N_KEY����������͵�ƽ��P_KEY����������ߵ�ƽ
typedef enum {
    N_KEY = 0,
    P_KEY = 1
}KeyType;

typedef enum {
	N_KEY_PRESSED = 0,
	N_KEY_UNPRESSED = 1,

	P_KEY_PRESSED = 1,
	P_KEY_UNPRESSED = 0
}KeyState;

typedef struct keyEvent{
    uint8_t _if_keyPress:1;     //��¼��������״̬
	uint8_t shortPressed:1;
	uint8_t doublePressed:1;
	uint8_t longPressed:1;
	uint8_t isLongPressing:1;
    uint8_t :0;
}keyEvent_TypeDef;

typedef struct key_parameter{
    /*��Ҫ�û���ʼ���Ĳ���*/
	GPIO_TypeDef * keyGPIO;		//����GPIO
	uint16_t keyGPIO_Pin;		//����GPIO_pin
    KeyType keytype:1;          //��¼��������
    /*end of ��Ҫ�û���ʼ���Ĳ���*/

    keyEvent_TypeDef key_event;
    // uint8_t longPressed:1;		//������־λ
	// uint8_t shortPressed:1;		//�̰���־λ
	// uint8_t doublePressed:1;		//˫����־λ
	// uint8_t isLongPressing:1;		//����������־λ
    // uint8_t :0;
	// KeyState keyState;		//��¼��������״̬
	uint8_t next_press_timeEn;	//�����ٴΰ��¼�ʱʹ�ܣ��ɽ���Ϊbit����
	uint8_t press_count;		//��¼�������´���
	uint16_t press_time_ms;		//��¼��һ�ΰ�������ʱ��
	uint16_t next_press_time_ms;//��¼�ٴΰ�������ʱ��	
//	keyEvent_enum key_event;			//�����¼��Ĵ�����7-4�������� 3�����������¼���2�������¼��� 1��˫���¼���0�������¼�

}key_parameter_TypeDef;

extern key_parameter_TypeDef fun_key_parameter;
extern key_parameter_TypeDef up_key_parameter;
extern key_parameter_TypeDef down_key_parameter;

extern uint8_t key_press_flag;//���ⰴ�����±�־
extern uint8_t keyScan_delay_En;//�ӳ�ɨ��ʹ�ܱ�־λ
extern uint16_t GetKeyValue_delayTime_ms;//����ɨ�����ڼ�ʱ��

void KEY_Init(void);	//IO��ʼ��
void set_singlePress(key_parameter_TypeDef* key_parameter);
void GetKeyValue(uint16_t GetKeyValue_delayTime);

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */



