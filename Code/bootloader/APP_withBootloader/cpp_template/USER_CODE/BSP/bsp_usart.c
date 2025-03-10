#include "bsp_usart.h"	
//#include "BSP_DMA.h"
#include "BSP_DMA_Define.h"
//#include <stdio.h>


#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#ifndef PRINT
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数  

struct std::__FILE 
{ 
	int handle; 
};


FILE __stdout;
	
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 

//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif






u16 recv_end_flag=0;       //接收状态标记

//uint16_t USART1_len;
//uint16_t USART2_len;

uint8_t uart1_sendBuff[Length_USART1_TX_Buff] = {0};
uint8_t uart1_recBuff[Length_USART1_RX_Buff] = {0};

uint8_t uart2_sendBuff[Length_USART2_TX_Buff] = {0};
uint8_t uart2_recBuff[Length_USART2_RX_Buff] = {0};

uint8_t uart4_sendBuff[Length_UART4_TX_Buff] = {0};
uint8_t uart4_recBuff[Length_UART4_RX_Buff] = {0};

u8 receiveBuffer_is_free=1;
u8 receiveBuffer[256] = {0};
int reciver_size=0;

//u8 USART3_RX_BUFFER[USART3_RX_LENGTH]; //保存接收数据的缓冲区
//u32 USART3_RX_CNT=0;  //当前接收到的数据长度
//u8 USART3_RX_FLAG=0; //1表示数据接收完毕 0表示没有接收完毕

uint8_t USARTx_TO_GPIO_AF(USART_TypeDef* USARTx){
	uint8_t GPIO_AF;
	
	if(USARTx == USART1)
		GPIO_AF = GPIO_AF_USART1;
	else if(USARTx == USART2)
		GPIO_AF = GPIO_AF_USART2;
	else if(USARTx == USART3)
		GPIO_AF = GPIO_AF_USART3;
	else if(USARTx == UART4)
		GPIO_AF = GPIO_AF_UART4;
	else if(USARTx == UART5)
		GPIO_AF = GPIO_AF_UART5;
	else if(USARTx == USART6)
		GPIO_AF = GPIO_AF_USART6;
	else if(USARTx == UART7)
		GPIO_AF = GPIO_AF_UART7;
	else if(USARTx == UART8)
		GPIO_AF = GPIO_AF_UART8;
	
	return GPIO_AF;
}

void* USART_TO_BSP_DMA_Streamx_chx(USART_TypeDef* USARTx,uint16_t USART_DMAReq){
	BSP_DMA_TypeDef *DMA_Streamx_chx;
	if(USART_DMAReq == USART_DMAReq_Tx){
		if(USARTx == USART1)
			DMA_Streamx_chx = &BSP_DMA_USART1_TX;
		else if(USARTx == USART2)
			DMA_Streamx_chx = &BSP_DMA_USART2_TX;
		else if(USARTx == USART3)
			DMA_Streamx_chx = &BSP_DMA_USART3_TX;
		else if(USARTx == UART4)
			DMA_Streamx_chx = &BSP_DMA_UART4_TX;
		else if(USARTx == UART5)
			DMA_Streamx_chx = &BSP_DMA_UART5_TX;
		else if(USARTx == USART6)
			DMA_Streamx_chx = &BSP_DMA_USART6_TX;
		else if(USARTx == UART7)
			DMA_Streamx_chx = &BSP_DMA_UART7_TX;
		else if(USARTx == UART8)
			DMA_Streamx_chx = &BSP_DMA_UART8_TX;		
	}
	else if(USART_DMAReq == USART_DMAReq_Rx){
		if(USARTx == USART1)
			DMA_Streamx_chx = &BSP_DMA_USART1_RX;
		else if(USARTx == USART2)
			DMA_Streamx_chx = &BSP_DMA_USART2_RX;
		else if(USARTx == USART3)
			DMA_Streamx_chx = &BSP_DMA_USART3_RX;
		else if(USARTx == UART4)
			DMA_Streamx_chx = &BSP_DMA_UART4_RX;
		else if(USARTx == UART5)
			DMA_Streamx_chx = &BSP_DMA_UART5_RX;
		else if(USARTx == USART6)
			DMA_Streamx_chx = &BSP_DMA_USART6_RX;
		else if(USARTx == UART7)
			DMA_Streamx_chx = &BSP_DMA_UART7_RX;
		else if(USARTx == UART8)
			DMA_Streamx_chx = &BSP_DMA_UART8_RX;
	}
	return (void*)DMA_Streamx_chx;
}

/*
***************************************************
函数名：USART_RCC_Init
功能：配置USART外设时钟
入口参数：	
返回值：无
应用范围：内部调用
备注：
***************************************************
*/
void USART_RCC_Init(USART_TypeDef* USARTx, BSP_GPIOSource_TypeDef* USART_RX, BSP_GPIOSource_TypeDef *USART_TX, uint8_t DMA_en){
	//IO时钟初始化
	uint32_t GPIO_RCC;
	GPIO_TypeDef* GPIOx = USART_RX->GPIOx;
	switch((uint32_t)GPIOx){
		case GPIOA_BASE:GPIO_RCC = RCC_AHB1Periph_GPIOA;break;
		case GPIOB_BASE:GPIO_RCC = RCC_AHB1Periph_GPIOB;break;
		case GPIOC_BASE:GPIO_RCC = RCC_AHB1Periph_GPIOC;break;
		case GPIOD_BASE:GPIO_RCC = RCC_AHB1Periph_GPIOD;break;
		case GPIOE_BASE:GPIO_RCC = RCC_AHB1Periph_GPIOE;break;
		case GPIOF_BASE:GPIO_RCC = RCC_AHB1Periph_GPIOF;break;
		case GPIOG_BASE:GPIO_RCC = RCC_AHB1Periph_GPIOG;break;
		case GPIOH_BASE:GPIO_RCC = RCC_AHB1Periph_GPIOH;break;
		case GPIOI_BASE:GPIO_RCC = RCC_AHB1Periph_GPIOI;break;
		default:break;
	}
		
	GPIOx = USART_TX->GPIOx;
	switch((uint32_t) GPIOx){
		case GPIOA_BASE:GPIO_RCC |= RCC_AHB1Periph_GPIOA;break;
		case GPIOB_BASE:GPIO_RCC |= RCC_AHB1Periph_GPIOB;break;
		case GPIOC_BASE:GPIO_RCC |= RCC_AHB1Periph_GPIOC;break;
		case GPIOD_BASE:GPIO_RCC |= RCC_AHB1Periph_GPIOD;break;
		case GPIOE_BASE:GPIO_RCC |= RCC_AHB1Periph_GPIOE;break;
		case GPIOF_BASE:GPIO_RCC |= RCC_AHB1Periph_GPIOF;break;
		case GPIOG_BASE:GPIO_RCC |= RCC_AHB1Periph_GPIOG;break;
		case GPIOH_BASE:GPIO_RCC |= RCC_AHB1Periph_GPIOH;break;
		case GPIOI_BASE:GPIO_RCC |= RCC_AHB1Periph_GPIOI;break;
		default:break;
	}
	
	//DMA时钟初始化
	if(DMA_en){
		BSP_DMA_TypeDef *BSP_DMA_Streamx_chx = (BSP_DMA_TypeDef*)USART_TO_BSP_DMA_Streamx_chx(USARTx,USART_DMAReq_Rx);
		
		//判断是DMA1还是DMA2
		if((uint32_t)BSP_DMA_Streamx_chx->DMA_Streamx > (uint32_t)DMA2)
		//DMA2时钟使能
			GPIO_RCC |= RCC_AHB1Periph_DMA2;
		else
		//DMA1时钟使能
			GPIO_RCC |= RCC_AHB1Periph_DMA1;
	}
	RCC_AHB1PeriphClockCmd(GPIO_RCC,ENABLE);
	
	//串口时钟初始化
	switch((uint32_t)USARTx){
		case USART1_BASE:RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);break;
		case USART2_BASE:RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);break;
		case USART3_BASE:RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);break;	
		case UART4_BASE:RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);break;
		case UART5_BASE:RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);break;
		case USART6_BASE:RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);break;
		case UART7_BASE:RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7,ENABLE);break;
		case UART8_BASE:RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8,ENABLE);break;
		default:break;
	}
}

uint8_t get_USART_NVIC_IRQChannel(USART_TypeDef* USARTx){
	uint8_t NVIC_IRQChannel = 0;
	switch((uint32_t)USARTx){
		case USART1_BASE:NVIC_IRQChannel = USART1_IRQn;break;
		case USART2_BASE:NVIC_IRQChannel = USART2_IRQn;break;
		case USART3_BASE:NVIC_IRQChannel = USART3_IRQn;break;
		case UART4_BASE:NVIC_IRQChannel = UART4_IRQn;break;
		case UART5_BASE:NVIC_IRQChannel = UART5_IRQn;break;
		case USART6_BASE:NVIC_IRQChannel = USART6_IRQn;break;
//		case UART7_BASE:NVIC_IRQChannel = UART7_IRQn;break;
//		case UART8_BASE:NVIC_IRQChannel = UART8_IRQn;break;
		default:break;
	}
	return NVIC_IRQChannel;	
}
/*
***************************************************
函数名：DMA_init
功能：USART的DMA配置
入口参数：	
返回值：无
应用范围：内部调用
备注：
***************************************************
*/

void DMA_init(USART_TypeDef* USARTx, uint8_t sendBuf[], uint8_t recBuf[], uint16_t ndtr){
	DMA_InitTypeDef DMA_InitStructure;
		
	DMA_InitStructure.DMA_BufferSize = 0 ;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USARTx->DR));
	
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
    /* Configure TX DMA */
	BSP_DMA_TypeDef *BSP_DMA_Streamx_chx = (BSP_DMA_TypeDef*)USART_TO_BSP_DMA_Streamx_chx(USARTx,USART_DMAReq_Tx);
	
    DMA_InitStructure.DMA_Channel = BSP_DMA_Streamx_chx->DMA_channel ;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
	
    DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)sendBuf ;
	
    DMA_Init(BSP_DMA_Streamx_chx->DMA_Streamx,&DMA_InitStructure);
    DMA_ITConfig(BSP_DMA_Streamx_chx->DMA_Streamx, DMA_IT_TC, ENABLE);
	
    DMA_InitStructure.DMA_BufferSize = ndtr ;

    /* Configure RX DMA */
	BSP_DMA_Streamx_chx = (BSP_DMA_TypeDef*)USART_TO_BSP_DMA_Streamx_chx(USARTx,USART_DMAReq_Rx);
    DMA_InitStructure.DMA_Channel = BSP_DMA_Streamx_chx->DMA_channel ;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
	
	
    DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)recBuf ;

    DMA_Init(BSP_DMA_Streamx_chx->DMA_Streamx,&DMA_InitStructure);
    DMA_Cmd(BSP_DMA_Streamx_chx->DMA_Streamx,ENABLE);
}



void USARTInit(USART_TypeDef* USARTx, BSP_GPIOSource_TypeDef *USART_RX, BSP_GPIOSource_TypeDef *USART_TX, 
                    uint32_t baudRate, uint8_t PreemptionPriority, uint8_t SubPriority, uint8_t uart_sendBuff[],uint8_t uart_recBuff[],uint16_t rec_bufLen){
	
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

//	BSP_DMA_TypeDef *BSP_DMA_Streamx_chx = (BSP_DMA_TypeDef*)USART_TO_BSP_DMA_Streamx_chx(USARTx,USART_DMAReq_Tx);

//	USART_RCC_Init(USARTx,USART_RX, USART_TX, NULL);				//初始化串口时钟,不开启DMA
	USART_RCC_Init(USARTx,USART_RX, USART_TX, DMA_on);//初始化串口时钟,开启DMA
	
	//IO配置						
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = USART_TX->Pin;
    GPIO_Init(USART_TX->GPIOx, &GPIO_InitStructure);
						
    GPIO_InitStructure.GPIO_Pin = USART_RX->Pin;
    GPIO_Init(USART_RX->GPIOx, &GPIO_InitStructure);
	
	//复用映射
	uint8_t GPIO_AF = USARTx_TO_GPIO_AF(USARTx);
	uint8_t GPIO_PinSource = GPIO_Pin_TO_GPIO_PinSource(USART_TX);
	GPIO_PinAFConfig(USART_TX->GPIOx, GPIO_PinSource, GPIO_AF);
	GPIO_PinSource = GPIO_Pin_TO_GPIO_PinSource(USART_RX);
    GPIO_PinAFConfig(USART_RX->GPIOx, GPIO_PinSource, GPIO_AF);
	
    USART_InitStructure.USART_BaudRate = baudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USARTx, &USART_InitStructure);
    
	DMA_init(USARTx, uart_sendBuff,uart_recBuff,rec_bufLen);
	USART_DMACmd(USARTx, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
   
//	get_USART_NVIC_IRQChannel(USARTx);
    NVIC_InitStructure.NVIC_IRQChannel = get_USART_NVIC_IRQChannel(USARTx);;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(USARTx, USART_IT_IDLE,ENABLE);
    USART_Cmd(USARTx, ENABLE);
//	
//	DMA1_Stream6->NDTR = 1;
//	DMA_Cmd(DMA1_Stream6, ENABLE);
	
}
														
//void USART2_Init(USART_TypeDef* USARTx, BSP_GPIOSource_TypeDef *USART_RX, BSP_GPIOSource_TypeDef *USART_TX, 
//														uint32_t baudRate, uint8_t PreemptionPriority, uint8_t SubPriority){
//	BSP_USART_TypeDef USART_InitPara;
//															
//	USART_InitPara.USARTx = USARTx;
//	USART_InitPara.USART_RX = USART_RX;
//	USART_InitPara.USART_TX = USART_TX;
//	USART_InitPara.USART_InitStructure.USART_BaudRate = baudRate;							
//	USART_InitPara.USART_InitStructure.USART_WordLength = USART_WordLength_8b;	/*字长为8位数据格式*/
//	USART_InitPara.USART_InitStructure.USART_StopBits = USART_StopBits_1;			/*一个停止位*/
//	USART_InitPara.USART_InitStructure.USART_Parity = USART_Parity_No;					/*无校验位*/
//	USART_InitPara.USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;							/*接收/发送模式*/	
//	USART_InitPara.USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None,	/*无硬件数据流控制*/	
//	//初始化串口
//	BSP_USART_Init(&USART_InitPara,PreemptionPriority,SubPriority);
//	
//	//初始化串口DMA
////	BSP_USART_TX_DMA_Init(&USART_InitPara);
////	BSP_USART_RX_DMA_Init(&USART_InitPara);
//										
//	//使用普通中断
//	USART_ITConfig(USART2, USART_IT_IDLE, DISABLE); 	//关闭串口空闲中断
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		//开启串口普通接受中断														
//}

//void USART3_Init(USART_TypeDef* USARTx, BSP_GPIOSource_TypeDef *USART_RX, BSP_GPIOSource_TypeDef *USART_TX, 
//														uint32_t baudRate, uint8_t PreemptionPriority, uint8_t SubPriority){
//	BSP_USART_TypeDef USART_InitPara;
//															
//	USART_InitPara.USARTx = USARTx;
//	USART_InitPara.USART_RX = USART_RX;
//	USART_InitPara.USART_TX = USART_TX;
//	USART_InitPara.USART_InitStructure.USART_BaudRate = baudRate;							
//	USART_InitPara.USART_InitStructure.USART_WordLength = USART_WordLength_8b;	/*字长为8位数据格式*/
//	USART_InitPara.USART_InitStructure.USART_StopBits = USART_StopBits_1;			/*一个停止位*/
//	USART_InitPara.USART_InitStructure.USART_Parity = USART_Parity_No;					/*无校验位*/
//	USART_InitPara.USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;							/*接收/发送模式*/	
//	USART_InitPara.USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None,	/*无硬件数据流控制*/	
//	//初始化串口
//	BSP_USART_Init(&USART_InitPara,PreemptionPriority,SubPriority);
//	
//	//初始化串口DMA
////	BSP_USART_TX_DMA_Init(&esp8266_USART);
////	BSP_USART_RX_DMA_Init(&esp8266_USART);
//										
//	//使用普通中断
//	USART_ITConfig(USART3, USART_IT_IDLE, DISABLE); 	//关闭串口空闲中断
//	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);		//开启串口普通接受中断														
//}

uint8_t UART4_DMA_RX_Buffer[UART4_DMA_RX_BUFFER_MAX_LENGTH];
uint8_t UART4_DMA_TX_Buffer[UART4_DMA_TX_BUFFER_MAX_LENGTH];
void UART4_DMA_Tx_Configuration(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 , ENABLE);					//DMA1时钟使能
	DMA_DeInit(DMA1_Stream4);
	while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);						//等待DMA可配置
	DMA_InitStructure.DMA_Channel = DMA_Channel_4; 							//DMA通道配置
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART4->DR;		//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)UART4_DMA_TX_Buffer;	//发送缓存指针
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;					//DMA传输方向：内存--->外设
	DMA_InitStructure.DMA_BufferSize = UART4_DMA_TX_BUFFER_MAX_LENGTH;		//数据传输字节数量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//使用普通模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					//中等优先级 DMA_Priority_High
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//外设突发单次传输
	DMA_Init(DMA1_Stream4, &DMA_InitStructure);								//初始化DMA Stream
	DMA_Cmd(DMA1_Stream4, DISABLE); 										//开启DMA传输
}


void UART4_DMA_Rx_Configuration(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
 
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 , ENABLE);					//DMA1时钟使能
	DMA_DeInit(DMA1_Stream2);
	while (DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);						//等待DMA可配置  
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  						//通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART4->DR;		//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)UART4_DMA_RX_Buffer;	//接收缓存指针
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;				//DMA传输方向：外设到存储器模式：外设--->内存
	DMA_InitStructure.DMA_BufferSize = UART4_DMA_RX_BUFFER_MAX_LENGTH;		//缓冲大小 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//使用普通模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					//中等优先级 DMA_Priority_VeryHigh
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//外设突发单次传输
	DMA_Init(DMA1_Stream2 , &DMA_InitStructure);							//初始化DMA_Stream	
	DMA_Cmd(DMA1_Stream2, ENABLE);  										//开启DMA传输
}

void UART4_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	
	USART_DeInit(UART4);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4 , ENABLE); 		//for USART2, USART3, UART4 or UART5.	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);     	
 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);
	USART_Cmd(UART4, ENABLE);
 
	USART_ClearFlag(UART4, USART_FLAG_TC); //清除发送完成标志
	while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);	//等待空闲帧发送完成后再清零发送完成标志（警告：增加这条在蓝牙的硬件框架下会导致死机，原因是前面只使能了USART_Mode_Rx，而没有使能USART_Mode_Tx）
	USART_ClearFlag(UART4, USART_FLAG_TC); //清除发送完成标志
 
    USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);				//禁止USART1接收不为空中断
	USART_ITConfig(UART4, USART_IT_TXE, DISABLE);				//禁止USART1发送空中断
	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);				//开启USART1空闲中断
	USART_ITConfig(UART4, USART_IT_TC, DISABLE);				//禁止USART1传输完成中断
	
	USART_DMACmd(UART4 ,   USART_DMAReq_Tx,DISABLE);  			//禁止串口的DMA发送
	USART_DMACmd(UART4 ,   USART_DMAReq_Rx,ENABLE);  			//使能串口的DMA接收
}


void UART4_Init(USART_TypeDef* USARTx, BSP_GPIOSource_TypeDef *USART_RX, BSP_GPIOSource_TypeDef *USART_TX, 
														uint32_t baudRate, uint8_t PreemptionPriority, uint8_t SubPriority){
												
}
/*
***************************************************
函数名：BSP_USART1_DMA_SendData
功能：串口1 DMA发送一组数据
入口参数：	Str：发送字符串指针
					len：数据长度
返回值：	0：发送成功
				1：DMA中有未传输数据
				2：DMA设置超时
应用范围：外部调用
备注：
***************************************************
*/
uint8_t BSP_USART1_DMA_SendData(uint8_t *Array,uint16_t len){
	unsigned char retry=0;
	if(DMA2_Stream7->NDTR){														//DMA通道中是否还有未传输的数据
		return 1;
	}
	DMA_Cmd(DMA2_Stream7, DISABLE); 
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE){//确保 DMA 可以被设置
		retry++;
		if(retry > 200){
			DMA_Cmd(DMA2_Stream7, ENABLE);
			return 2;
		}
	}
	DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7);	//清空标志位
	DMA2_Stream7->M0AR = (uint32_t)Array;
	DMA2_Stream7->NDTR = len;
	DMA_Cmd(DMA2_Stream7, ENABLE);   
	return 0;
}

/*
***************************************************
函数名：BSP_USART2_DMA_SendData
功能：串口2 DMA发送一组数据
入口参数：	Str：发送字符串指针
					len：数据指针
返回值：	0：发送成功
				1：DMA中有未传输数据
				2：DMA设置超时
应用范围：外部调用
备注：
***************************************************
*/
uint8_t BSP_USART2_DMA_SendData(uint8_t *Array,uint16_t len){
	unsigned char retry=0;
	if(DMA1_Stream6->NDTR){														//DMA通道中是否还有未传输的数据
		return 1;
	}
	DMA_Cmd(DMA1_Stream6, DISABLE); 
	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE){//确保 DMA 可以被设置
		retry++;
		if(retry > 200){
			DMA_Cmd(DMA1_Stream6, ENABLE);
			return 2;
		}
	}
	DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6 | DMA_FLAG_HTIF6);	//清空标志位
	DMA1_Stream6->M0AR = (uint32_t)Array;
	DMA1_Stream6->NDTR = len;
	DMA_Cmd(DMA1_Stream6, ENABLE);   
	return 0;
}

/*
***************************************************
函数名：BSP_UART4_DMA_SendData
功能：串口4 DMA发送一组数据
入口参数：	Str：发送字符串指针
					len：数据指针
返回值：	0：发送成功
				1：DMA中有未传输数据
				2：DMA设置超时
应用范围：外部调用
备注：
***************************************************
*/
uint8_t BSP_UART4_DMA_SendData(uint8_t *Array,uint16_t len){
	unsigned char retry=0;
	if(DMA1_Stream4->NDTR){														//DMA通道中是否还有未传输的数据
		return 1;
	}
	DMA_Cmd(DMA1_Stream4, DISABLE); 
	while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE){//确保 DMA 可以被设置
		retry++;
		if(retry > 200){
			DMA_Cmd(DMA1_Stream4, ENABLE);
			return 2;
		}
	}
	DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4 | DMA_FLAG_HTIF4);	//清空标志位
	DMA1_Stream4->M0AR = (uint32_t)Array;
	DMA1_Stream4->NDTR = len;
	DMA_Cmd(DMA1_Stream4, ENABLE);   
	return 0;
}

void Uart1_SendStr(u8* SendBuf,u8 len)
{
//	//发送数据前将485使能引脚置位，执行485输出
//	GPIO_SetBits(GPIOE,GPIO_Pin_4);//LED1
	while(len>0)
    {
        while((USART1->SR&0X40)==0);//等待发送完成
        USART1->DR = *SendBuf;
        SendBuf++;
		len--;
    }
	while((USART1->SR&0X40)==0);		//这一步绝对不能少，保证最后一个字节发送成功
//	//发送完成之后设置485使能引脚置位，转为接收状态
//	GPIO_ResetBits(GPIOE,GPIO_Pin_4);
}

void Uart2_SendStr(u8* SendBuf,u8 len)
{
//	//发送数据前将485使能引脚置位，执行485输出
//	GPIO_SetBits(GPIOE,GPIO_Pin_4);//LED1
	while(len>0)
    {
        while((USART2->SR&0X40)==0);//等待发送完成
        USART2->DR = *SendBuf;
        SendBuf++;
		len--;
    }
	while((USART2->SR&0X40)==0);		//这一步绝对不能少，保证最后一个字节发送成功
//	//发送完成之后设置485使能引脚置位，转为接收状态
//	GPIO_ResetBits(GPIOE,GPIO_Pin_4);
}

/*
函数功能: 字符串发送
*/
void USARTx_StringSend(USART_TypeDef *USARTx,char *str){
   while(*str!='\0')
   {
       USARTx->DR=*str++;
       while(!(USARTx->SR&1<<7)){}
   }
}
 
/*
函数功能: 数据发送
*/
void USARTx_DataSend(USART_TypeDef *USARTx,u8 *data,u32 len){
	u32 i;
	for(i=0;i<len;i++)
	{
	   USARTx->DR=*data++;
	   while(!(USARTx->SR&1<<7)){}
	}
}
#ifdef PRINT
/*
函数功能: 格式化打印函数
*/
char USART1_PRINTF_BUFF[1024];
void USART1_Printf(char *fmt,...){
   va_list ap;
   /*1. 初始化形参列表*/
   va_start(ap,fmt);
   /*2. 提取可变形参数据*/
    vsprintf(USART1_PRINTF_BUFF,fmt,ap);
   /*3. 结束，释放空间*/
    va_end(ap);
   /*4. 输出数据到串口1*/
   USARTx_StringSend(USART1,USART1_PRINTF_BUFF);
   //USART1_Printf("%d%s",123,454656); 
   //int data=va_arg(ap,int);
}

char USART2_PRINTF_BUFF[1024];
void USART2_Printf(char *fmt,...){
   va_list ap;
   /*1. 初始化形参列表*/
   va_start(ap,fmt);
   /*2. 提取可变形参数据*/
    vsprintf(USART2_PRINTF_BUFF,fmt,ap);
   /*3. 结束，释放空间*/
    va_end(ap);
   /*4. 输出数据到串口1*/
   USARTx_StringSend(USART2,USART2_PRINTF_BUFF);
   //USART1_Printf("%d%s",123,454656); 
   //int data=va_arg(ap,int);
}

char USART3_PRINTF_BUFF[1024];
void USART3_Printf(char *fmt,...){
   va_list ap;
   /*1. 初始化形参列表*/
   va_start(ap,fmt);
   /*2. 提取可变形参数据*/
    vsprintf(USART3_PRINTF_BUFF,fmt,ap);
   /*3. 结束，释放空间*/
    va_end(ap);
   /*4. 输出数据到串口1*/
   USARTx_StringSend(USART3,USART3_PRINTF_BUFF);
   //USART1_Printf("%d%s",123,454656); 
   //int data=va_arg(ap,int);
}
#endif

#ifdef __cplusplus
 }
#endif /* __cplusplus */













 



