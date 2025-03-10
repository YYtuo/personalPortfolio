#include "IAP.h" 



iapfun jump2app; 
u32 iapbuf[512]; 	//2K�ֽڻ���  
u8 Receive_dat_buffer[2048];			  	//���ݽ��ջ�������


//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}

/************************************************
*������:	iap_write_appbin
*���ܣ�	д��bin�ļ�
*���룺	appxaddr:Ӧ�ó������ʼ��ַ
*		appbuf:Ӧ�ó���CODE
*		appsize:Ӧ�ó����С(�ֽ�).
*�����	��
*��ע��	��
************************************************/
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize){
	u32 t;
	u16 i=0;
	u32 temp;
	u32 fwaddr=appxaddr;//��ǰд��ĵ�ַ
	u8 *dfu=appbuf;
	for(t=0;t<appsize;t+=4)
	{						   
		temp=(u32)dfu[3]<<24;   
		temp|=(u32)dfu[2]<<16;    
		temp|=(u32)dfu[1]<<8;
		temp|=(u32)dfu[0];	  
		dfu+=4;//ƫ��4���ֽ�
		iapbuf[i++]=temp;	    
		if(i==512)
		{
			i=0; 
			BSP_FLASH_Write(fwaddr,iapbuf,512);
			fwaddr+=2048;//ƫ��2048  512*4=2048
		}
	} 
	if(i)BSP_FLASH_Write(fwaddr,iapbuf,i);//������һЩ�����ֽ�д��ȥ.  
}

/************************************************
*������:	iap_load_app
*���ܣ�	��ת��Ӧ�ó����
*���룺	appxaddr:Ӧ�ó������ʼ��ַ
*�����	��
*��ע��	��
************************************************/
void iap_load_app(u32 appxaddr){
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		jump2app();									//��ת��APP.
	}
}		 


/******************************
������  ��FirmwareUpdate
��    �ܣ��̼���������
��ڲ�������
���ڲ�������
����ֵ  ����
*******************************/
void FirmwareUpdate(const char* file_name){
    u8 i=0;
    u8 res;
    u16 br;
    u16 readlen;
    u32 addrx;
    u32 Receive_data=0; //������յ���������
    u32 file_size=0;    //�ļ�size
    
	/*�����Ƿ����Ҫ������BIN�ļ�*/
    res = f_open(file, file_name, FA_OPEN_EXISTING | FA_READ);
    //res = f_open(&file_bin, "TEST.bin", FA_OPEN_EXISTING | FA_READ);
    file_size=f_size(file);    //��ȡ���ļ���СByte
//    printf("��ȡ���ļ���СΪ:%dByte\r\n",file_size);

	if(res!=FR_OK) {
		printf("No Updatable Firmware \r\n");
		delay_ms(1000);
		return;
	}
    addrx=FLASH_APP1_ADDR;
    
	/*ִ����Ҫ��IAP����*/
	printf("Firmware Updating...\r\n");
	printf("Size of APP: %d\r\n",file_size);
	while(1){
		/*ÿ�ζ�ȡ2K�����ݵ��ڴ滺����buffer*/
	    res = f_read(file, Receive_dat_buffer, STM_PAGE_SIZE, (UINT*)&br);
        i++;
        readlen=br;
        Receive_data+=br;   //��ȡ�����ֽ���
        if (res || br == 0){
            break; 
		}
        
        iap_write_appbin(addrx,Receive_dat_buffer,readlen);//����ȡ������д��Flash��
        addrx+=STM_PAGE_SIZE;//ƫ��2048  512*4=2048
    }
    delay_ms(500);
	
#ifdef UPDATE_AND_DEL
	f_unlink(BOOTLOADER_BIN_ADDR);
#endif	
}



/******************************
������  ��BinFileInit
��    �ܣ������Ƿ����Ҫ������BIN�ļ�
��ڲ�������
���ڲ�����bin�ļ���
����ֵ  ����
��ע: ��ά����ָ���������ļ��б�
*******************************/
uint8_t  _ifBinFile(uint8_t (*fileList)[FileList_SIZE]){	
	uint8_t file_num = 0;
	FRESULT res = f_opendir(&dir, (const TCHAR*)"0:");
	if(res){
		printf("opendir error: %d\r\n",res);
		return 0;
	}
	
	/*�����Ƿ����Ҫ������BIN�ļ�*/
	while(1){
		res = f_readdir(&dir, &fileinfo);
		//��ȡĿ¼�µ�һ���ļ�
		if (res != FR_OK || fileinfo.fname[0] == 0){
			break;	//������/��ĩβ��,�˳�
		}
		
		uint8_t fileLen = strlen(fileinfo.fname);
		
		// �ж��Ƿ�Ϊ.bin�ļ�
		for(uint8_t i = 0; i<fileLen; ++i){
			if(fileinfo.fname[i]=='.'){
				// û��bin�ļ���ֱ��������һ���ļ�
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
	
	// ���ļ��б���ļ�������ĸ��˳���������
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
		// û�������ļ�
		printf("No Updatable Firmware \r\n");
		return 0;
	}
	else{
		return file_num;
	}
}



