#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#ifndef __IAP_H__
#define __IAP_H__


#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "stm32f4xx.h"

#include "delay.h"
#include "flash.h"
#include "exfuns.h"
#include "ff.h"	
#include "malloc.h"

typedef  void (*iapfun)(void);				//定义一个函数类型的参数.   

#define BOOTLOADER_BIN_ADDR         ( "0:/APP.bin" ) //BootLoader的SD卡bin文件路径
#define FLASH_APP1_ADDR		0x08010000  	//第一个应用程序起始地址(存放在FLASH)
											//保留0X08000000~0X0800FFFF的空间为Bootloader使用(共64KB)

#define BOOT_WAITING		4000				// boot加载延迟，单位ms
#define FileList_NUM		6				//	列表文件数				
#define FileList_SIZE		20				// 文件的长度（取最大值）
											
#define STM_PAGE_SIZE		2048			//注意：STM32F103ZET6的FLASH页大小为2K

/*开启此宏定义，更新完成后删除更新文件*/
//#define UPDATE_AND_DEL

void iap_load_app(u32 appxaddr);			//跳转到APP程序执行
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//在指定地址开始,写入bin
void FirmwareUpdate(const char* file_name);
uint8_t  _ifBinFile(uint8_t (*fileList)[FileList_SIZE]);
#endif

#ifdef __cplusplus
 }
#endif /* __cplusplus */



