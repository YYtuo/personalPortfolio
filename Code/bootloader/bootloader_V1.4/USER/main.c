#include "init.h"
#define BOOT_OLED_LOG

typedef enum {
	INIT,
	FIND_BIN_FILE,
	SD_CARD_BUSY,	// 读卡器 初始化
	BIN_CHOOSE,		// 进入程序选择界面
	LOADING,		// 程序加载
	LOAD_FAILED
}Boot_State;

// bin文件列表，从这个列表中加载bin文件
uint8_t bin_fileList[FileList_NUM][FileList_SIZE]={0};
int32_t currentTime = 0;
Boot_State ctl_state = LOADING;
uint8_t file_num = 0;
SD_Error SD_flag = SD_ERROR;	//SD卡标志位，0为初始化正常，其他值为错误


uint8_t USBInitFlag_bit = 1;	// USB初始化标志位


extern int32_t TIM3_ms_counter;
extern uint8_t keyVal;
extern vu8 USB_STATUS_REG;

void loadFailed(void){
	// 红灯闪烁，程序跳转失败
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
	USART_Cmd(USART1, DISABLE);  //失能串口1 ，防止跳转到App程序时死机
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, DISABLE);//失能USB OTG时钟
	
	// 关闭定时器
	TIM_Cmd(TIM2,DISABLE); //使能定时器2
	TIM_Cmd(TIM3,DISABLE); //使能定时器3
	TIM_Cmd(TIM12,DISABLE); //使能定时器12
	
	iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码	
	
	// 跳转失败
	USART_Cmd(USART1, ENABLE);  //使能串口1
	printf("===Original BIN Error!!!===\r\n");	
}

int main(void){
//	uint8_t readbuf[512] = {0};
	board_init();

	// 如果有SD卡，则获取SD卡的bin文件数，否则直接加载旧程序
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
		
		// 识别有无bin文件
		if(ctl_state == FIND_BIN_FILE){
			// 没有.bin文件的提示窗口
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
			
			// 超时自动加载旧程序
			if(TIM3_ms_counter-currentTime >= BOOT_WAITING || TIM3_ms_counter-currentTime < 0){
				oled_Cls();
				ctl_state = LOADING;
				// 更新时间节点
				currentTime = TIM3_ms_counter;
			}
			
			// 期间如果按下确认键，初始化读卡器
			if(fun_key_parameter.key_event.shortPressed){
				fun_key_parameter.key_event.shortPressed = 0;

				if(USBInitFlag_bit){
					USBInitFlag_bit = 0;
					oled_Cls();
					// 读卡器 初始化
					#ifdef BOOT_USB_MSC
					USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
					#endif
					oled_ShowStr(0,0,"================",small);
					oled_ShowStr(0,1,"SD-Card is busy",small);
					oled_ShowStr(0,2,"================",small);
					ctl_state = SD_CARD_BUSY;
				}
			}

			// 期间如果按下其他按键,且sd卡非空
			if(up_key_parameter.key_event.shortPressed || down_key_parameter.key_event.shortPressed){
				up_key_parameter.key_event.shortPressed = up_key_parameter.key_event.shortPressed?0:0;
				down_key_parameter.key_event.shortPressed = down_key_parameter.key_event.shortPressed?0:0;

				if(file_num){
					oled_Cls();
					// 初始化oled菜单显示文件列表
					fileListInterFaceInit(bin_fileList,file_num);

					// 进入程序选择界面
					ctl_state = BIN_CHOOSE;
				}
			}
		}
		/*end of FIND_BIN_FILE*/ 

		// PC正在操作sd卡
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
					
					// bin文件无效或者跳转失败
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
			// 跳转失败
			ctl_state = LOAD_FAILED;
		}
		/*end of LOADING*/ 
		
		if(ctl_state == LOAD_FAILED){
			loadFailed();
		}
	}

}



