#include "malloc.h"	   
#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

//�ڴ��(32�ֽڶ���)
__align(32) u8 mem1base[MEM1_MAX_SIZE];													//�ڲ�SRAM�ڴ��
__align(32) u8 mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));					//�ⲿSRAM�ڴ��
__align(32) u8 mem3base[MEM3_MAX_SIZE] __attribute__((at(0X10000000)));					//�ڲ�CCM�ڴ��
//�ڴ�����
u16 mem1mapbase[MEM1_ALLOC_TABLE_SIZE];													//�ڲ�SRAM�ڴ��MAP
u16 mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));	//�ⲿSRAM�ڴ��MAP
u16 mem3mapbase[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0X10000000+MEM3_MAX_SIZE)));	//�ڲ�CCM�ڴ��MAP
//�ڴ�������	   
const u32 memtblsize[SRAMBANK]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE,MEM3_ALLOC_TABLE_SIZE};	//�ڴ���С
const u32 memblksize[SRAMBANK]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE,MEM3_BLOCK_SIZE};					//�ڴ�ֿ��С
const u32 memsize[SRAMBANK]={MEM1_MAX_SIZE,MEM2_MAX_SIZE,MEM3_MAX_SIZE};							//�ڴ��ܴ�С


//�ڴ���������
struct _m_mallco_dev mallco_dev=
{
	memory_init,						//�ڴ��ʼ��
	memory_perused,						//�ڴ�ʹ����
	mem1base,mem2base,mem3base,			//�ڴ��
	mem1mapbase,mem2mapbase,mem3mapbase,//�ڴ����״̬��
	0,0,0,  		 					//�ڴ����δ����
};

/************************************************
*��������mymemcpy
*���ܣ�	�����ڴ�
*���룺*des:Ŀ�ĵ�ַ
*		*src:Դ��ַ
*		n:��Ҫ���Ƶ��ڴ泤��(�ֽ�Ϊ��λ)
*�������
*��ע����
************************************************/
void mymemcpy(void *des,void *src,u32 n)  
{  
    u8 *xdes = (u8*)des;
	u8 *xsrc = (u8*)src; 
    while(n--)*xdes++=*xsrc++;  
} 

/************************************************
*��������mymemset
*���ܣ�	�����ڴ�
*���룺*s:�ڴ��׵�ַ
*		c :Ҫ���õ�ֵ
*		count:��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
*�������
*��ע����
************************************************/
void mymemset(void *s,u8 c,u32 count)  
{  
    u8 *xs = (u8*)s;  
    while(count--)*xs++=c;  
}	   

/************************************************
*��������memory_init
*���ܣ�	�ڴ�����ʼ��  
*���룺memx:�����ڴ��
*�������
*��ע����/�ڲ�����
************************************************/
void memory_init(u8 memx)  
{  
    mymemset(mallco_dev.memmap[memx], 0,memtblsize[memx]*2);//�ڴ�״̬����������  
	mymemset(mallco_dev.membase[memx], 0,memsize[memx]);	//�ڴ��������������  
	mallco_dev.memrdy[memx]=1;								//�ڴ�����ʼ��OK  
}  

/************************************************
*��������memory_perused
*���ܣ�	��ȡ�ڴ�ʹ����
*���룺memx:�����ڴ��
*�����ʹ����(0~100)
*��ע����/�ڲ�����
************************************************/
u8 memory_perused(u8 memx)  
{  
    u32 used=0;  
    u32 i;  
    for(i=0;i<memtblsize[memx];i++)  
    {  
        if(mallco_dev.memmap[memx][i])used++; 
    } 
    return (used*100)/(memtblsize[memx]);  
}  

/************************************************
*��������memory_malloc
*���ܣ�	�ڴ����
*���룺memx:�����ڴ��
*		size:Ҫ������ڴ��С(�ֽ�)
*�����0XFFFFFFFF,�������;����,�ڴ�ƫ�Ƶ�ַ 
*��ע���ڲ�����
************************************************/
u32 memory_malloc(u8 memx,u32 size)  
{  
    signed long offset=0;  
    u32 nmemb;	//��Ҫ���ڴ����  
	u32 cmemb=0;//�������ڴ����
    u32 i;  
    if(!mallco_dev.memrdy[memx])mallco_dev.init(memx);//δ��ʼ��,��ִ�г�ʼ�� 
    if(size==0)return 0XFFFFFFFF;//����Ҫ����
    nmemb=size/memblksize[memx];  	//��ȡ��Ҫ����������ڴ����
    if(size%memblksize[memx])nmemb++;  
    for(offset=memtblsize[memx]-1;offset>=0;offset--)//���������ڴ������  
    {     
		if(!mallco_dev.memmap[memx][offset])cmemb++;//�������ڴ��������
		else cmemb=0;								//�����ڴ������
		if(cmemb==nmemb)							//�ҵ�������nmemb�����ڴ��
		{
            for(i=0;i<nmemb;i++)  					//��ע�ڴ��ǿ� 
            {  
                mallco_dev.memmap[memx][offset+i]=nmemb;  
            }  
            return (offset*memblksize[memx]);//����ƫ�Ƶ�ַ  
		}
    }  
    return 0XFFFFFFFF;//δ�ҵ����Ϸ����������ڴ��  
}  

/************************************************
*��������memory_free
*���ܣ�	�ͷ��ڴ�
*���룺memx:�����ڴ��
*		offsize:�ڴ��ַƫ��
*�����0,�ͷųɹ�; 1,�ͷ�ʧ��;
*��ע���ڲ�����
************************************************/
u8 memory_free(u8 memx,u32 offset)  
{  
    int i;  
    if(!mallco_dev.memrdy[memx])//δ��ʼ��,��ִ�г�ʼ��
	{
		mallco_dev.init(memx);    
        return 1;//δ��ʼ��  
    }  
    if(offset<memsize[memx])//ƫ�����ڴ����. 
    {  
        int index=offset/memblksize[memx];			//ƫ�������ڴ�����  
        int nmemb=mallco_dev.memmap[memx][index];	//�ڴ������
        for(i=0;i<nmemb;i++)  						//�ڴ������
        {  
            mallco_dev.memmap[memx][index+i]=0;  
        }  
        return 0;  
    }else return 2;//ƫ�Ƴ�����.  
}  

/************************************************
*��������myfree
*���ܣ�	�ͷ��ڴ�
*���룺memx:�����ڴ��
*		ptr:�ڴ��׵�ַ 
*�������
*��ע���ⲿ����
************************************************/
void myfree(u8 memx,void *ptr)  
{  
	u32 offset;   
	if(ptr==NULL)return;//��ַΪ0.  
 	offset=(u32)ptr-(u32)mallco_dev.membase[memx];     
    memory_free(memx,offset);	//�ͷ��ڴ�      
}  

/************************************************
*��������mymalloc
*���ܣ�	�����ڴ�
*���룺memx:�����ڴ��
*		size:�ڴ��С(�ֽ�)
*��������䵽���ڴ��׵�ַ.
*��ע���ⲿ����
************************************************/
void *mymalloc(u8 memx,u32 size)  
{  
    u32 offset;   
	offset = memory_malloc(memx,size);  	   	 	   
    if(offset == 0XFFFFFFFF)return NULL;  
    else return (void*)((u32)mallco_dev.membase[memx]+offset);  
}  

/************************************************
*��������myremalloc
*���ܣ�	���·����ڴ�
*���룺memx:�����ڴ��
*		*ptr:���ڴ��׵�ַ
*		size:Ҫ������ڴ��С(�ֽ�)
*������·��䵽���ڴ��׵�ַ.
*��ע���ⲿ����
************************************************/
void *myremalloc(u8 memx,void *ptr,u32 size)  
{  
    u32 offset;    
    offset = memory_malloc(memx,size);   	
    if(offset == 0XFFFFFFFF)return NULL;     
    else  
    {  									   
	    mymemcpy((void*)((u32)mallco_dev.membase[memx]+offset),ptr,size);	//�������ڴ����ݵ����ڴ�   
        myfree(memx,ptr);  											  		//�ͷž��ڴ�
        return (void*)((u32)mallco_dev.membase[memx]+offset);  				//�������ڴ��׵�ַ
    }  
}

#ifdef __cplusplus
 }
#endif /* __cplusplus */

