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

typedef  void (*iapfun)(void);				//����һ���������͵Ĳ���.   

#define BOOTLOADER_BIN_ADDR         ( "0:/APP.bin" ) //BootLoader��SD��bin�ļ�·��
#define FLASH_APP1_ADDR		0x08010000  	//��һ��Ӧ�ó�����ʼ��ַ(�����FLASH)
											//����0X08000000~0X0800FFFF�Ŀռ�ΪBootloaderʹ��(��64KB)

#define BOOT_WAITING		4000				// boot�����ӳ٣���λms
#define FileList_NUM		6				//	�б��ļ���				
#define FileList_SIZE		20				// �ļ��ĳ��ȣ�ȡ���ֵ��
											
#define STM_PAGE_SIZE		2048			//ע�⣺STM32F103ZET6��FLASHҳ��СΪ2K

/*�����˺궨�壬������ɺ�ɾ�������ļ�*/
//#define UPDATE_AND_DEL

void iap_load_app(u32 appxaddr);			//��ת��APP����ִ��
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//��ָ����ַ��ʼ,д��bin
void FirmwareUpdate(const char* file_name);
uint8_t  _ifBinFile(uint8_t (*fileList)[FileList_SIZE]);
#endif

#ifdef __cplusplus
 }
#endif /* __cplusplus */



