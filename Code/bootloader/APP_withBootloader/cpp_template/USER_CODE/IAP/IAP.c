#include "IAP.h" 



iapfun jump2app; 
u32 iapbuf[512]; 	//2K字节缓存  
u8 Receive_dat_buffer[2048];			  	//数据接收缓存数组


//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}

/************************************************
*函数名:	iap_write_appbin
*功能：	写入bin文件
*输入：	appxaddr:应用程序的起始地址
*		appbuf:应用程序CODE
*		appsize:应用程序大小(字节).
*输出：	无
*备注：	无
************************************************/
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize){
	u32 t;
	u16 i=0;
	u32 temp;
	u32 fwaddr=appxaddr;//当前写入的地址
	u8 *dfu=appbuf;
	for(t=0;t<appsize;t+=4)
	{						   
		temp=(u32)dfu[3]<<24;   
		temp|=(u32)dfu[2]<<16;    
		temp|=(u32)dfu[1]<<8;
		temp|=(u32)dfu[0];	  
		dfu+=4;//偏移4个字节
		iapbuf[i++]=temp;	    
		if(i==512)
		{
			i=0; 
			BSP_FLASH_Write(fwaddr,iapbuf,512);
			fwaddr+=2048;//偏移2048  512*4=2048
		}
	} 
	if(i)BSP_FLASH_Write(fwaddr,iapbuf,i);//将最后的一些内容字节写进去.  
}

/************************************************
*函数名:	iap_load_app
*功能：	跳转到应用程序段
*输入：	appxaddr:应用程序的起始地址
*输出：	无
*备注：	无
************************************************/
void iap_load_app(u32 appxaddr){
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();									//跳转到APP.
	}
}		 


/******************************
函数名  ：FirmwareUpdate
功    能：固件升级函数
入口参数：无
出口参数：无
返回值  ：无
*******************************/
void FirmwareUpdate(const char* file_name){
    u8 i=0;
    u8 res;
    u16 br;
    u16 readlen;
    u32 addrx;
    u32 Receive_data=0; //计算接收的总数据数
    u32 file_size=0;    //文件size
    
	/*查找是否存在要升级的BIN文件*/
    res = f_open(file, file_name, FA_OPEN_EXISTING | FA_READ);
    //res = f_open(&file_bin, "TEST.bin", FA_OPEN_EXISTING | FA_READ);
    file_size=f_size(file);    //读取的文件大小Byte
//    printf("读取的文件大小为:%dByte\r\n",file_size);

	if(res!=FR_OK) {
		printf("No Updatable Firmware \r\n");
		delay_ms(1000);
		return;
	}
    addrx=FLASH_APP1_ADDR;
    
	/*执行主要的IAP功能*/
	printf("Firmware Updating...\r\n");
	printf("Size of APP: %d\r\n",file_size);
	while(1){
		/*每次读取2K的数据到内存缓冲区buffer*/
	    res = f_read(file, Receive_dat_buffer, STM_PAGE_SIZE, (UINT*)&br);
        i++;
        readlen=br;
        Receive_data+=br;   //读取的总字节数
        if (res || br == 0){
            break; 
		}
        
        iap_write_appbin(addrx,Receive_dat_buffer,readlen);//将读取的数据写入Flash中
        addrx+=STM_PAGE_SIZE;//偏移2048  512*4=2048
    }
    delay_ms(500);
	
#ifdef UPDATE_AND_DEL
	f_unlink(BOOTLOADER_BIN_ADDR);
#endif	
}



/******************************
函数名  ：BinFileInit
功    能：查找是否存在要升级的BIN文件
入口参数：无
出口参数：bin文件数
返回值  ：无
备注: 二维数组指针用来传文件列表
*******************************/
uint8_t  _ifBinFile(uint8_t (*fileList)[FileList_SIZE]){	
	uint8_t file_num = 0;
	FRESULT res = f_opendir(&dir, (const TCHAR*)"0:");
	if(res){
		printf("opendir error: %d\r\n",res);
		return 0;
	}
	
	/*查找是否存在要升级的BIN文件*/
	while(1){
		res = f_readdir(&dir, &fileinfo);
		//读取目录下的一个文件
		if (res != FR_OK || fileinfo.fname[0] == 0){
			break;	//错误了/到末尾了,退出
		}
		
		uint8_t fileLen = strlen(fileinfo.fname);
		
		// 判断是否为.bin文件
		for(uint8_t i = 0; i<fileLen; ++i){
			if(fileinfo.fname[i]=='.'){
				// 没有bin文件，直接跳到下一个文件
				if(strcmp(&fileinfo.fname[i+1], "BIN") != 0){
					break;
				}

				if(strlen(fileinfo.fname) > FileList_SIZE){
					printf("file name is too long: len=%d\r\n",strlen(fileinfo.fname));
				}
				else {
					strcpy((char*)fileList[file_num],fileinfo.fname);
					file_num++;	
				}
			}
		}
		
	}
	
	f_closedir(&dir);
	
	// 对文件列表的文件按照字母表顺序进行排序
	uint8_t *temp_file = (uint8_t *)mymalloc(SRAMIN,sizeof(char)*FileList_SIZE);
	for(uint8_t i=0; i < file_num-1; ++i){
		for(uint8_t j=0; j<file_num-1-i;++j){
			if(strcmp((const char *)fileList[j],(const char *)fileList[j+1]) > 0){
				strcpy((char *)temp_file,(const char *)fileList[j]);
				strcpy((char *)fileList[j],(const char *)fileList[j+1]);
				strcpy((char *)fileList[j+1],(const char *)temp_file);
			}
		}
	}

	myfree(SRAMIN,temp_file);
	if(!file_num){
		// 没有升级文件
		printf("No Updatable Firmware \r\n");
		return 0;
	}
	else{
		return file_num;
	}
}



