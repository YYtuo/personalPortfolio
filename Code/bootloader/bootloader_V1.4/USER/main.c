#include "init.h"
#define BOOT_OLED_LOG

typedef enum {
	INIT,
	FIND_BIN_FILE,
	SD_CARD_BUSY,	// ������ ��ʼ��
	BIN_CHOOSE,		// �������ѡ�����
	LOADING,		// �������
	LOAD_FAILED
}Boot_State;

// bin�ļ��б�������б��м���bin�ļ�
uint8_t bin_fileList[FileList_NUM][FileList_SIZE]={0};
int32_t currentTime = 0;
Boot_State ctl_state = LOADING;
uint8_t file_num = 0;
SD_Error SD_flag = SD_ERROR;	//SD����־λ��0Ϊ��ʼ������������ֵΪ����


uint8_t USBInitFlag_bit = 1;	// USB��ʼ����־λ


extern int32_t TIM3_ms_counter;
extern uint8_t keyVal;
extern vu8 USB_STATUS_REG;

void loadFailed(void){
	// �����˸��������תʧ��
	LED_R = 0;
//	beep_on(40,0,4000);
	delay_ms(500);
	
	LED_R = 1;
//	beep_stop();
	delay_ms(500);
}


void test_jump(void){
	
	beep_on(10,0,4000);
	delay_ms(500);
	beep_stop();
	printf("===Loading Original BIN===\r\n");
	USART_Cmd(USART1, DISABLE);  //ʧ�ܴ���1 ����ֹ��ת��App����ʱ����
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, DISABLE);//ʧ��USB OTGʱ��
	
	// �رն�ʱ��
	TIM_Cmd(TIM2,DISABLE); //ʹ�ܶ�ʱ��2
	TIM_Cmd(TIM3,DISABLE); //ʹ�ܶ�ʱ��3
	TIM_Cmd(TIM12,DISABLE); //ʹ�ܶ�ʱ��12
	
	iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����	
	
	// ��תʧ��
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1
	printf("===Original BIN Error!!!===\r\n");	
}

int main(void){
//	uint8_t readbuf[512] = {0};
	board_init();

	// �����SD�������ȡSD����bin�ļ���������ֱ�Ӽ��ؾɳ���
	if(SD_flag == SD_OK){
		ctl_state = FIND_BIN_FILE;
		file_num = _ifBinFile( bin_fileList );
	}
	currentTime = TIM3_ms_counter;
	delay_ms(500);
	oled_Cls();
	while(1){
		#ifdef REMOTE
		USBH_Process(&USB_OTG_Core_dev, &USB_Host);
		#endif
		
		// ʶ������bin�ļ�
		if(ctl_state == FIND_BIN_FILE){
			// û��.bin�ļ�����ʾ����
			#ifdef BOOT_OLED_LOG
			if(!file_num){
				oled_ShowStr(0,0,"======================",small);
				oled_ShowStr(0,1,"No bin file in SD-Card",small);
				oled_ShowStr(0,2,"Press left key to init",small);
				oled_ShowStr(0,3,"    SD-Card Reader    ",small);
				oled_ShowStr(0,4,"======================",small);
			}

			else{
				uint8_t log[22] = {"0"};
				sprintf((char*)log," =%d= bin file(s)  ",file_num);
				oled_ShowStr(0,0,"=====================",small);
				oled_ShowStr(0,1,log,small);
				oled_ShowStr(0,2," Press mid/right key ",small);
				oled_ShowStr(0,3,"    to choose one    ",small);
				oled_ShowStr(0,4,"=====================",small);
			}
			#endif
			
			
			oled_ShowNum(8*7,6,((BOOT_WAITING - (TIM3_ms_counter-currentTime)) /1000),2,big);
			
			// ��ʱ�Զ����ؾɳ���
			if(TIM3_ms_counter-currentTime >= BOOT_WAITING || TIM3_ms_counter-currentTime < 0){
				oled_Cls();
				ctl_state = LOADING;
				// ����ʱ��ڵ�
				currentTime = TIM3_ms_counter;
			}
			
			// �ڼ��������ȷ�ϼ�����ʼ��������
			if(fun_key_parameter.key_event.shortPressed){
				fun_key_parameter.key_event.shortPressed = 0;

				if(USBInitFlag_bit){
					USBInitFlag_bit = 0;
					oled_Cls();
					// ������ ��ʼ��
					#ifdef BOOT_USB_MSC
					USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
					#endif
					oled_ShowStr(0,0,"================",small);
					oled_ShowStr(0,1,"SD-Card is busy",small);
					oled_ShowStr(0,2,"================",small);
					ctl_state = SD_CARD_BUSY;
				}
			}

			// �ڼ����������������,��sd���ǿ�
			if(up_key_parameter.key_event.shortPressed || down_key_parameter.key_event.shortPressed){
				up_key_parameter.key_event.shortPressed = up_key_parameter.key_event.shortPressed?0:0;
				down_key_parameter.key_event.shortPressed = down_key_parameter.key_event.shortPressed?0:0;

				if(file_num){
					oled_Cls();
					// ��ʼ��oled�˵���ʾ�ļ��б�
					fileListInterFaceInit(bin_fileList,file_num);

					// �������ѡ�����
					ctl_state = BIN_CHOOSE;
				}
			}
		}
		/*end of FIND_BIN_FILE*/ 

		// PC���ڲ���sd��
		if(ctl_state == SD_CARD_BUSY){
			while(1){
				LED_B = 0;
				delay_ms(500);
				LED_B = 1;
				delay_ms(500);
			}
		}
		/*end of SD_CARD_BUSY*/ 

		if(ctl_state == BIN_CHOOSE){
			while(1){
				uint8_t file_index = meunTaskRun(bin_fileList);
				
				if(file_index){
					oled_ShowStr(0,0,"================",small);
					oled_ShowStr(0,1,"APP Updating ...",small);
					oled_ShowStr(0,2,"================",small);
					FirmwareUpdate((const char*)bin_fileList[file_index-1]);
					oled_Cls();
					
					oled_ShowStr(0,2,"================",small);
					oled_ShowStr(5*6,3,bin_fileList[file_index-1],small);
					oled_ShowStr(0,4," * is runing ...",small);
					oled_ShowStr(0,5,"================",small);
					
					test_jump();
					
					// bin�ļ���Ч������תʧ��
					ctl_state = LOAD_FAILED;
					break;
				}
			}
		}
		/*end of BIN_CHOOSE*/ 

		if(ctl_state == LOADING){
			oled_ShowStr(0,0,"===============",small);
			oled_ShowStr(0,1,"Auto Loading...",small);
			oled_ShowStr(0,1,"Default Progran",small);
			oled_ShowStr(0,2,"===============",small);
			test_jump();
			// ��תʧ��
			ctl_state = LOAD_FAILED;
		}
		/*end of LOADING*/ 
		
		if(ctl_state == LOAD_FAILED){
			loadFailed();
		}
	}

}



