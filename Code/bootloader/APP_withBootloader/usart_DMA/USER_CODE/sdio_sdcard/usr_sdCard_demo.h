#ifndef __USR_SDCARD_DEMO_H
#define __USR_SDCARD_DEMO_H		
#include "stdio.h" 
#include <string.h>
#include <stdlib.h>

#include "stm32f4xx.h" 		

#include "sdio_sdcard.h"
#include "ff.h"	
#include "exfuns.h"
#include "malloc.h"


void usr_scan_files(uint8_t* path);
void usr_f_read(const uint8_t* path, uint16_t len);
void usr_f_write(const uint8_t* path, uint8_t* buf, uint16_t len);

#endif
