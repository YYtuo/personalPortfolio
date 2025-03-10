#include "init.h"

//#pragma arm section code = "RAMCODE"
SD_Error SD_flag = SD_ERROR;	//SD����־λ��0Ϊ��ʼ������������ֵΪ����
void board_init(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	LED_Init();				//��ʼ��LED�˿�
	KEY_Init();
	beep_Init();
	USARTInit(USART1,US1_Rx, US1_Tx, 115200,1,1,uart1_sendBuff, uart1_recBuff,Length_USART1_RX_Buff);
	oledPlayer_Init();
//	menuInit();
	
	communicate_init();
	
	#ifdef MALLOC
	memory_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	memory_init(SRAMCCM);		//��ʼ��CCM�ڴ��
	#endif
	
	//���SD��,��SD���򱨾��������˸
	SD_flag = SD_Init();
	if(SD_flag){
		LED_R = 0;
		for(int i=0; i<3; ++i){
			beep_on(40,0,4000);
			delay_ms(100);
			beep_stop();
			delay_ms(100);
			if(!SD_Init()){
				SD_flag = SD_OK;
                printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//��ʾ����
				break;
			};
		}
		LED_R = 1;
	}
	
	// ��SD�����ܣ���������
	if(!SD_flag){
		printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//��ʾ����
		
		#ifdef FATFS
		uint32_t sd_total,sd_free;
        exfuns_init();							//Ϊfatfs��ر��������ڴ�				 
  	    u8 res = f_mount(fs[0],"0:",1); 					//����SD�� 
		if(res){
			printf("f_mount error : %d\r\n",res);
		}
		
        //�õ�SD������������ʣ������
	    while(exf_getfree("0",&sd_total,&sd_free)){
			printf("SD Card Fatfs Error!");
        }
		printf("SD Card Fatfs: total:%d MB, free:%d MB\r\n",sd_total>>10,sd_free>>10);
		#endif
	}
	
	// ��ʼ��ң����
	#ifdef REMOTE
	USBH_Init(&USB_OTG_Core_dev,USB_OTG_FS_CORE_ID,&USB_Host,&HID_cb,&USR_Callbacks);  
	#endif

	TIM3_Int_Init(100-1,840-1);	//��ʱ��ʱ��84M����Ƶϵ��840������84M/840=100Khz�ļ���Ƶ�ʣ�����100��Ϊ1ms
}
//#pragma arm section
