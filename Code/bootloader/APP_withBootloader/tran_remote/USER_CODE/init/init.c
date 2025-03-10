#include "init.h"

//#pragma arm section code = "RAMCODE"
SD_Error SD_flag = SD_ERROR;	//SD卡标志位，0为初始化正常，其他值为错误
void board_init(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	LED_Init();				//初始化LED端口
	KEY_Init();
	beep_Init();
	USARTInit(USART1,US1_Rx, US1_Tx, 115200,1,1,uart1_sendBuff, uart1_recBuff,Length_USART1_RX_Buff);
	oledPlayer_Init();
//	menuInit();
	
	communicate_init();
	
	#ifdef MALLOC
	memory_init(SRAMIN);		//初始化内部内存池 
	memory_init(SRAMCCM);		//初始化CCM内存池
	#endif
	
	//检测SD卡,无SD卡则报警，红灯闪烁
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
                printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//显示容量
				break;
			};
		}
		LED_R = 1;
	}
	
	// 有SD卡则跑，无则跳过
	if(!SD_flag){
		printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//显示容量
		
		#ifdef FATFS
		uint32_t sd_total,sd_free;
        exfuns_init();							//为fatfs相关变量申请内存				 
  	    u8 res = f_mount(fs[0],"0:",1); 					//挂载SD卡 
		if(res){
			printf("f_mount error : %d\r\n",res);
		}
		
        //得到SD卡的总容量和剩余容量
	    while(exf_getfree("0",&sd_total,&sd_free)){
			printf("SD Card Fatfs Error!");
        }
		printf("SD Card Fatfs: total:%d MB, free:%d MB\r\n",sd_total>>10,sd_free>>10);
		#endif
	}
	
	// 初始化遥控器
	#ifdef REMOTE
	USBH_Init(&USB_OTG_Core_dev,USB_OTG_FS_CORE_ID,&USB_Host,&HID_cb,&USR_Callbacks);  
	#endif

	TIM3_Int_Init(100-1,840-1);	//定时器时钟84M，分频系数840，所以84M/840=100Khz的计数频率，计数100次为1ms
}
//#pragma arm section
