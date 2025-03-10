#include "usr_sdCard_demo.h"

/**********************************
 * ��������sdCard_init
 * ���ܣ�SD����ʼ��demo
 * ���룺
 * �����
 * ��ע����
**********************************/
void SDCard_init(void){
	//���SD��,��SD���򱨾��������˸
	uint8_t SD_flag = SD_Init();
	
	if(SD_flag){
		/*�û�����������������ʾSD����ʼ��ʧ�ܣ����Զ�γ�ʼ�����߶�����ʼ��*/
		/*end of �û���������*/
	}
	
	// ��SD�����ܣ���������
	if(!SD_flag){
		printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//��ʾ����
		
		#ifdef FATFS
        exfuns_init();							//Ϊfatfs��ر��������ڴ�				 
  	    f_mount(fs[0],"0:",1); 					//����SD�� 
	
        //�õ�SD������������ʣ������
	    while(exf_getfree("0",&sd_total,&sd_free)){
			printf("SD Card Fatfs Error!");
        }
		printf("SD Card Fatfs: total:%d, free:%d\r\n",sd_total,sd_free);
		#endif
	}
}

/**********************************
 * ��������SDCard_read
 * ���ܣ����ļ�ϵͳ��SD��
 * ���룺readbuf����������
 * 		sector:������ַ
 * 		cnt��������
 * �����
 * ��ע����
**********************************/
u8 SDCard_read(uint8_t* readbuf,u32 sector,u8 cnt){
	// u8 res = SD_ReadDisk(readbuf,sector,cnt);
	u8 res = SD_ReadDisk(readbuf,0,1);
	if(res==SD_OK){
		printf("SECTOR 0 DATA:\r\n");
		for(int sd_size=0;sd_size<512;sd_size++)printf("%x ",readbuf[sd_size]);//��ӡ0��������    	   
		printf("\r\nDATA ENDED\r\n");
	}
	return res;
}

/**********************************
 * ��������SDCard_write
 * ���ܣ����ļ�ϵͳдSD��
 * ���룺writeBuf����������
 * 		sector:������ַ
 * 		cnt��������
 * �����
 * ��ע����
**********************************/
u8 SDCard_write(uint8_t* writeBuf,u32 sector,u8 cnt){
	// u8 res = SD_ReadDisk(readbuf,sector,cnt);
	u8 res = SD_WriteDisk(writeBuf,sector,cnt);

	if(res==SD_OK){
		printf("write to SECTOR 0 DATA:\r\n");
		for(int sd_size=0;sd_size<512;sd_size++)printf("%x ",writeBuf[sd_size]);//��ӡ0��������    	   
		printf("\r\n wrire DATA ENDED\r\n");
	}
	/*�û��ȶԺ���*/
		//�����д�����������ж������������ݽ��бȶԣ�ȷ��д��ɹ�
	/*end of �û��ȶԺ���*/ 

	return res;
}

/*=================����Ϊ ʹ���ļ�ϵͳ�ĺ���demo================================================*/
#ifdef FATFS

/**********************************
 * ��������usr_f_mount
 * ���ܣ����ش���
 * ���룺path: ·��
 * 		mt: 0�ӳٹ��أ�1��������
 * �����
 * ��ע����
**********************************/
u8 usr_f_mount(u8* path,u8 mt)
{		   
	return f_mount(fs[2],(const TCHAR*)path,mt); 
}
/**********************************
 * ��������SDCard_write
 * ���ܣ�ɨ�����·���µ������ļ�
 * ���룺path:·��
 * �����
 * ��ע����
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
		res = f_readdir(&dir, &fileinfo);                   //��ȡĿ¼�µ�һ���ļ�
		if (res != FR_OK || fileinfo.fname[0] == 0) break;  //������/��ĩβ��,�˳�
		//if (fileinfo.fname[0] == '.') continue;             //�����ϼ�Ŀ¼
		#if _USE_LFN
		char* fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
		#else							   
		fn = fileinfo.fname;
		#endif	                                              /* It is a file. */
		
		// printf("%s/",path);//��ӡ·��
		printf("0:/");//��ӡ·��
		printf("%s\r\n",  fn);//��ӡ�ļ���	  
	}
	f_closedir(&dir);
	myfree(SRAMIN,fileinfo.lfname);
}

/**********************************
 * ��������usr_f_read
 * ���ܣ����ļ�
 * ���룺path:�ļ���
 * 		len:��Ҫ��ȡ���ļ����ݳ��ȣ��ֽڣ�
 * �����
 * ��ע����
**********************************/
void usr_f_read(const uint8_t* path, uint16_t len){
	u16 i,t;
	u16 tlen=0;
	// u8 res = f_open(file,(const TCHAR*)path,FA_READ);//���ļ�
	u8 res = f_open(file,(const TCHAR*)"0:/test.txt",FA_READ);//���ļ�
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
		//�����ݳ�����
		if(res){
			printf("\r\nRead Error:%d\r\n",res);   
		}
		else{
			tlen+=br;
			for(t=0;t<br;t++)printf("%c",fatbuf[t]); 
		}	 
	}
	if(tlen)printf("\r\nReaded data len:%d\r\n",tlen);//���������ݳ���
	printf("========== Read data over ============\r\n");	
	res=f_close(file);
	if(res){
		printf("\r\nread close error:%d\r\n",res);
	}
}

/**********************************
 * ��������usr_f_write
 * ���ܣ�д�ļ�
 * ���룺path:�ļ���
 * 		buf:д����ַ���
 * 		len:д������ݳ���
 * �����
 * ��ע��������д����ļ����ǿգ������ļ���ĩβ����
**********************************/
void usr_f_write(const uint8_t* path, uint8_t* buf, uint16_t len){
	printf("\r\n== Begin Write file... ==\r\n");

	// u8 res = f_open(file,(const TCHAR*)path,FA_READ);//���ļ�
	u8 res = f_open(file,(const TCHAR*)"0:/test.txt",FA_WRITE);//���ļ�
	if(res){
		printf("open file error: %d\r\n",res);
		return;
	}

	// ����ļ�����
	// printf("%d\r\n",(int)f_size(file));

	// ����ָ�뵽�ļ�ĩβ
	f_lseek(file,file->fsize);

	//  �����ǰָ��λ��
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


