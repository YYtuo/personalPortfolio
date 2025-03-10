#ifndef __INIT_H__
#define __INIT_H__

#include "delay.h"

#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_beep.h"
#include "bsp_usart.h"
#include "protocol.h"


#include "timer.h"
//#include "communicate.h"
#include "oled.h"
#include "malloc.h"
#include "sdio_sdcard.h"
#include "exfuns.h"
#include "ff.h"	
//#include "usbh_usr.h" 
//#include "usbd_usr.h"
//#include "usbd_msc_core.h"
//#include "usbd_desc.h"
#include "menu.h"



#include "IAP.h" 

//#include "usr_sdCard_demo.h"

#include "adc.h"

#define FATFS
#define MALLOC
//#define REMOTE
//#define BOOT_USB_MSC		// 初始化读卡器，直接把程序更新文件拖进SD卡


void board_init(void);

#endif // __INIT_H__
