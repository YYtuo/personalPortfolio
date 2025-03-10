#include "usr_sdCard_demo.h"

/**********************************
 * 函数名：sdCard_init
 * 功能：SD卡初始化demo
 * 输入：
 * 输出：
 * 备注：无
**********************************/
void SDCard_init(void){
	//检测SD卡,无SD卡则报警，红灯闪烁
	uint8_t SD_flag = SD_Init();
	
	if(SD_flag){
		/*用户报警函数，用于提示SD卡初始化失败，可以多次初始化或者堵塞初始化*/
		/*end of 用户报警函数*/
	}
	
	// 有SD卡则跑，无则跳过
	if(!SD_flag){
		printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//显示容量
		
		#ifdef FATFS
        exfuns_init();							//为fatfs相关变量申请内存				 
  	    f_mount(fs[0],"0:",1); 					//挂载SD卡 
	
        //得到SD卡的总容量和剩余容量
	    while(exf_getfree("0",&sd_total,&sd_free)){
			printf("SD Card Fatfs Error!");
        }
		printf("SD Card Fatfs: total:%d, free:%d\r\n",sd_total,sd_free);
		#endif
	}
}

/**********************************
 * 函数名：SDCard_read
 * 功能：无文件系统读SD卡
 * 输入：readbuf：读缓冲区
 * 		sector:扇区地址
 * 		cnt：扇区数
 * 输出：
 * 备注：无
**********************************/
u8 SDCard_read(uint8_t* readbuf,u32 sector,u8 cnt){
	// u8 res = SD_ReadDisk(readbuf,sector,cnt);
	u8 res = SD_ReadDisk(readbuf,0,1);
	if(res==SD_OK){
		printf("SECTOR 0 DATA:\r\n");
		for(int sd_size=0;sd_size<512;sd_size++)printf("%x ",readbuf[sd_size]);//打印0扇区数据    	   
		printf("\r\nDATA ENDED\r\n");
	}
	return res;
}

/**********************************
 * 函数名：SDCard_write
 * 功能：无文件系统写SD卡
 * 输入：writeBuf：读缓冲区
 * 		sector:扇区地址
 * 		cnt：扇区数
 * 输出：
 * 备注：无
**********************************/
u8 SDCard_write(uint8_t* writeBuf,u32 sector,u8 cnt){
	// u8 res = SD_ReadDisk(readbuf,sector,cnt);
	u8 res = SD_WriteDisk(writeBuf,sector,cnt);

	if(res==SD_OK){
		printf("write to SECTOR 0 DATA:\r\n");
		for(int sd_size=0;sd_size<512;sd_size++)printf("%x ",writeBuf[sd_size]);//打印0扇区数据    	   
		printf("\r\n wrire DATA ENDED\r\n");
	}
	/*用户比对函数*/
		//这里对写过的扇区进行读操作，对数据进行比对，确保写入成功
	/*end of 用户比对函数*/ 

	return res;
}

/*=================以下为 使用文件系统的函数demo================================================*/
#ifdef FATFS

/**********************************
 * 函数名：usr_f_mount
 * 功能：挂载磁盘
 * 输入：path: 路径
 * 		mt: 0延迟挂载，1立即挂载
 * 输出：
 * 备注：无
**********************************/
u8 usr_f_mount(u8* path,u8 mt)
{		   
	return f_mount(fs[2],(const TCHAR*)path,mt); 
}
/**********************************
 * 函数名：SDCard_write
 * 功能：扫描输出路径下的所有文件
 * 输入：path:路径
 * 输出：
 * 备注：无
**********************************/
// usr_scan_files("0:")
void usr_scan_files(uint8_t* path){
	FRESULT res = f_opendir(&dir, (const TCHAR*)path);
	if(res)printf("open error: %d\r\n",res);
	
	fileinfo.lfsize = _MAX_LFN * 2 + 1;
	#ifdef MALLOC
	fileinfo.lfname = mymalloc(SRAMIN,fileinfo.lfsize);
	#else
	fileinfo.lfname = (TCHAR*)malloc(fileinfo.lfsize);
	#endif
	
	while(1){
		res = f_readdir(&dir, &fileinfo);                   //读取目录下的一个文件
		if (res != FR_OK || fileinfo.fname[0] == 0) break;  //错误了/到末尾了,退出
		//if (fileinfo.fname[0] == '.') continue;             //忽略上级目录
		#if _USE_LFN
		char* fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
		#else							   
		fn = fileinfo.fname;
		#endif	                                              /* It is a file. */
		
		// printf("%s/",path);//打印路径
		printf("0:/");//打印路径
		printf("%s\r\n",  fn);//打印文件名	  
	}
	f_closedir(&dir);
	myfree(SRAMIN,fileinfo.lfname);
}

/**********************************
 * 函数名：usr_f_read
 * 功能：读文件
 * 输入：path:文件名
 * 		len:想要读取的文件数据长度（字节）
 * 输出：
 * 备注：无
**********************************/
void usr_f_read(const uint8_t* path, uint16_t len){
	u16 i,t;
	u16 tlen=0;
	// u8 res = f_open(file,(const TCHAR*)path,FA_READ);//打开文件
	u8 res = f_open(file,(const TCHAR*)"0:/test.txt",FA_READ);//打开文件
	if(res){
		printf("open file error: %d\r\n",res);
		return;
	}
	
	printf("\r\n== Read file data is:  ==\r\n");
	for(i=0;i<len/512;i++){
		res=f_read(file,fatbuf,512,&br);
		if(res){
			printf("Read Error:%d\r\n",res);
			break;
		}
		else{
			tlen+=br;
			for(t=0;t<br;t++)printf("%c",fatbuf[t]); 
		}
	}
	if(len%512){
		res=f_read(file,fatbuf,len%512,&br);
		//读数据出错了
		if(res){
			printf("\r\nRead Error:%d\r\n",res);   
		}
		else{
			tlen+=br;
			for(t=0;t<br;t++)printf("%c",fatbuf[t]); 
		}	 
	}
	if(tlen)printf("\r\nReaded data len:%d\r\n",tlen);//读到的数据长度
	printf("========== Read data over ============\r\n");	
	res=f_close(file);
	if(res){
		printf("\r\nread close error:%d\r\n",res);
	}
}

/**********************************
 * 函数名：usr_f_write
 * 功能：写文件
 * 输入：path:文件名
 * 		buf:写入的字符串
 * 		len:写入的数据长度
 * 输出：
 * 备注：本函数写入的文件若非空，则在文件的末尾插入
**********************************/
void usr_f_write(const uint8_t* path, uint8_t* buf, uint16_t len){
	printf("\r\n== Begin Write file... ==\r\n");

	// u8 res = f_open(file,(const TCHAR*)path,FA_READ);//打开文件
	u8 res = f_open(file,(const TCHAR*)"0:/test.txt",FA_WRITE);//打开文件
	if(res){
		printf("open file error: %d\r\n",res);
		return;
	}

	// 输出文件长度
	// printf("%d\r\n",(int)f_size(file));

	// 设置指针到文件末尾
	f_lseek(file,file->fsize);

	//  输出当前指针位置
	// int nowtell = f_tell(file);
	// printf("now tell is: %d\r\n",nowtell);

	printf("Write data len:%d\r\n",len);	 
	res=f_write(file,buf,len,&bw);
	if(res){
		printf("Write Error:%d\r\n",res);   
	}
	else printf("Writed data len:%d\r\n",bw);
	printf("========= Write data over =========\r\n");
	
	res=f_close(file);
	if(res){
		printf("\r\nwrite close error:%d\r\n",res);
	}

}
#endif


