#include "malloc.h"	   
#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

//内存池(32字节对齐)
__align(32) u8 mem1base[MEM1_MAX_SIZE];													//内部SRAM内存池
__align(32) u8 mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));					//外部SRAM内存池
__align(32) u8 mem3base[MEM3_MAX_SIZE] __attribute__((at(0X10000000)));					//内部CCM内存池
//内存管理表
u16 mem1mapbase[MEM1_ALLOC_TABLE_SIZE];													//内部SRAM内存池MAP
u16 mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));	//外部SRAM内存池MAP
u16 mem3mapbase[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0X10000000+MEM3_MAX_SIZE)));	//内部CCM内存池MAP
//内存管理参数	   
const u32 memtblsize[SRAMBANK]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE,MEM3_ALLOC_TABLE_SIZE};	//内存表大小
const u32 memblksize[SRAMBANK]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE,MEM3_BLOCK_SIZE};					//内存分块大小
const u32 memsize[SRAMBANK]={MEM1_MAX_SIZE,MEM2_MAX_SIZE,MEM3_MAX_SIZE};							//内存总大小


//内存管理控制器
struct _m_mallco_dev mallco_dev=
{
	memory_init,						//内存初始化
	memory_perused,						//内存使用率
	mem1base,mem2base,mem3base,			//内存池
	mem1mapbase,mem2mapbase,mem3mapbase,//内存管理状态表
	0,0,0,  		 					//内存管理未就绪
};

/************************************************
*函数名：mymemcpy
*功能：	复制内存
*输入：*des:目的地址
*		*src:源地址
*		n:需要复制的内存长度(字节为单位)
*输出：无
*备注：无
************************************************/
void mymemcpy(void *des,void *src,u32 n)  
{  
    u8 *xdes = (u8*)des;
	u8 *xsrc = (u8*)src; 
    while(n--)*xdes++=*xsrc++;  
} 

/************************************************
*函数名：mymemset
*功能：	设置内存
*输入：*s:内存首地址
*		c :要设置的值
*		count:需要设置的内存大小(字节为单位)
*输出：无
*备注：无
************************************************/
void mymemset(void *s,u8 c,u32 count)  
{  
    u8 *xs = (u8*)s;  
    while(count--)*xs++=c;  
}	   

/************************************************
*函数名：memory_init
*功能：	内存管理初始化  
*输入：memx:所属内存块
*输出：无
*备注：外/内部调用
************************************************/
void memory_init(u8 memx)  
{  
    mymemset(mallco_dev.memmap[memx], 0,memtblsize[memx]*2);//内存状态表数据清零  
	mymemset(mallco_dev.membase[memx], 0,memsize[memx]);	//内存池所有数据清零  
	mallco_dev.memrdy[memx]=1;								//内存管理初始化OK  
}  

/************************************************
*函数名：memory_perused
*功能：	获取内存使用率
*输入：memx:所属内存块
*输出：使用率(0~100)
*备注：外/内部调用
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
*函数名：memory_malloc
*功能：	内存分配
*输入：memx:所属内存块
*		size:要分配的内存大小(字节)
*输出：0XFFFFFFFF,代表错误;其他,内存偏移地址 
*备注：内部调用
************************************************/
u32 memory_malloc(u8 memx,u32 size)  
{  
    signed long offset=0;  
    u32 nmemb;	//需要的内存块数  
	u32 cmemb=0;//连续空内存块数
    u32 i;  
    if(!mallco_dev.memrdy[memx])mallco_dev.init(memx);//未初始化,先执行初始化 
    if(size==0)return 0XFFFFFFFF;//不需要分配
    nmemb=size/memblksize[memx];  	//获取需要分配的连续内存块数
    if(size%memblksize[memx])nmemb++;  
    for(offset=memtblsize[memx]-1;offset>=0;offset--)//搜索整个内存控制区  
    {     
		if(!mallco_dev.memmap[memx][offset])cmemb++;//连续空内存块数增加
		else cmemb=0;								//连续内存块清零
		if(cmemb==nmemb)							//找到了连续nmemb个空内存块
		{
            for(i=0;i<nmemb;i++)  					//标注内存块非空 
            {  
                mallco_dev.memmap[memx][offset+i]=nmemb;  
            }  
            return (offset*memblksize[memx]);//返回偏移地址  
		}
    }  
    return 0XFFFFFFFF;//未找到符合分配条件的内存块  
}  

/************************************************
*函数名：memory_free
*功能：	释放内存
*输入：memx:所属内存块
*		offsize:内存地址偏移
*输出：0,释放成功; 1,释放失败;
*备注：内部调用
************************************************/
u8 memory_free(u8 memx,u32 offset)  
{  
    int i;  
    if(!mallco_dev.memrdy[memx])//未初始化,先执行初始化
	{
		mallco_dev.init(memx);    
        return 1;//未初始化  
    }  
    if(offset<memsize[memx])//偏移在内存池内. 
    {  
        int index=offset/memblksize[memx];			//偏移所在内存块号码  
        int nmemb=mallco_dev.memmap[memx][index];	//内存块数量
        for(i=0;i<nmemb;i++)  						//内存块清零
        {  
            mallco_dev.memmap[memx][index+i]=0;  
        }  
        return 0;  
    }else return 2;//偏移超区了.  
}  

/************************************************
*函数名：myfree
*功能：	释放内存
*输入：memx:所属内存块
*		ptr:内存首地址 
*输出：无
*备注：外部调用
************************************************/
void myfree(u8 memx,void *ptr)  
{  
	u32 offset;   
	if(ptr==NULL)return;//地址为0.  
 	offset=(u32)ptr-(u32)mallco_dev.membase[memx];     
    memory_free(memx,offset);	//释放内存      
}  

/************************************************
*函数名：mymalloc
*功能：	分配内存
*输入：memx:所属内存块
*		size:内存大小(字节)
*输出：分配到的内存首地址.
*备注：外部调用
************************************************/
void *mymalloc(u8 memx,u32 size)  
{  
    u32 offset;   
	offset = memory_malloc(memx,size);  	   	 	   
    if(offset == 0XFFFFFFFF)return NULL;  
    else return (void*)((u32)mallco_dev.membase[memx]+offset);  
}  

/************************************************
*函数名：myremalloc
*功能：	重新分配内存
*输入：memx:所属内存块
*		*ptr:旧内存首地址
*		size:要分配的内存大小(字节)
*输出：新分配到的内存首地址.
*备注：外部调用
************************************************/
void *myremalloc(u8 memx,void *ptr,u32 size)  
{  
    u32 offset;    
    offset = memory_malloc(memx,size);   	
    if(offset == 0XFFFFFFFF)return NULL;     
    else  
    {  									   
	    mymemcpy((void*)((u32)mallco_dev.membase[memx]+offset),ptr,size);	//拷贝旧内存内容到新内存   
        myfree(memx,ptr);  											  		//释放旧内存
        return (void*)((u32)mallco_dev.membase[memx]+offset);  				//返回新内存首地址
    }  
}

#ifdef __cplusplus
 }
#endif /* __cplusplus */

