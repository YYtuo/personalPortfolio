#include "bsp_usart.h"	
#include "BSP_DMA.h"
#include "BSP_DMA_Define.h"


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#ifndef PRINT
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif

u16 recv_end_flag=0;       //����״̬���


uint8_t uart1_sendBuff[Length_USART1_TX_Buff] = {0};
uint8_t uart1_recBuff[Length_USART1_RX_Buff] = {0};

/*bootloader �£�ֻ������һ���е��ԣ���һ�����ٴ�����*/
void USARTInit(USART_TypeDef* USARTx, BSP_GPIOSource_TypeDef *USART_RX, BSP_GPIOSource_TypeDef *USART_TX, 
                    uint32_t baudRate, uint8_t PreemptionPriority, uint8_t SubPriority, uint8_t uart_sendBuff[],uint8_t uart_recBuff[],uint16_t rec_bufLen){
	
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

//	BSP_DMA_TypeDef *BSP_DMA_Streamx_chx = (BSP_DMA_TypeDef*)USART_TO_BSP_DMA_Streamx_chx(USARTx,USART_DMAReq_Tx);

//	USART_RCC_Init(USARTx,USART_RX, USART_TX, NULL);				//��ʼ������ʱ��,������DMA
	// USART_RCC_Init(USARTx,USART_RX, USART_TX, DMA_on);//��ʼ������ʱ��,����DMA

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	
	//IO����						
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = USART_TX->Pin;
    GPIO_Init(USART_TX->GPIOx, &GPIO_InitStructure);
						
    GPIO_InitStructure.GPIO_Pin = USART_RX->Pin;
    GPIO_Init(USART_TX->GPIOx, &GPIO_InitStructure);
	
	//����ӳ��
	uint8_t GPIO_PinSource = GPIO_Pin_TO_GPIO_PinSource(USART_TX);
	GPIO_PinAFConfig(USART_TX->GPIOx, GPIO_PinSource, GPIO_AF_USART1);
	GPIO_PinSource = GPIO_Pin_TO_GPIO_PinSource(USART_RX);
    GPIO_PinAFConfig(USART_RX->GPIOx, GPIO_PinSource, GPIO_AF_USART1);
	
    USART_InitStructure.USART_BaudRate = baudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USARTx, &USART_InitStructure);
    
	/*����DMA*/
	// DMA_init(USARTx, uart_sendBuff,uart_recBuff,rec_bufLen);
	// USART_DMACmd(USARTx, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
   
//	get_USART_NVIC_IRQChannel(USARTx);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // USART_ITConfig(USARTx, USART_IT_IDLE,ENABLE); // ���������ж�
    USART_ITConfig(USARTx, USART_IT_RXNE,ENABLE); // ������ͨ�ж�

    USART_Cmd(USARTx, ENABLE);
}
														
//void USART2_Init(USART_TypeDef* USARTx, BSP_GPIOSource_TypeDef *USART_RX, BSP_GPIOSource_TypeDef *USART_TX, 
//														uint32_t baudRate, uint8_t PreemptionPriority, uint8_t SubPriority){
//	BSP_USART_TypeDef USART_InitPara;
//															
//	USART_InitPara.USARTx = USARTx;
//	USART_InitPara.USART_RX = USART_RX;
//	USART_InitPara.USART_TX = USART_TX;
//	USART_InitPara.USART_InitStructure.USART_BaudRate = baudRate;							
//	USART_InitPara.USART_InitStructure.USART_WordLength = USART_WordLength_8b;	/*�ֳ�Ϊ8λ���ݸ�ʽ*/
//	USART_InitPara.USART_InitStructure.USART_StopBits = USART_StopBits_1;			/*һ��ֹͣλ*/
//	USART_InitPara.USART_InitStructure.USART_Parity = USART_Parity_No;					/*��У��λ*/
//	USART_InitPara.USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;							/*����/����ģʽ*/	
//	USART_InitPara.USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None,	/*��Ӳ������������*/	
//	//��ʼ������
//	BSP_USART_Init(&USART_InitPara,PreemptionPriority,SubPriority);
//	
//	//��ʼ������DMA
////	BSP_USART_TX_DMA_Init(&USART_InitPara);
////	BSP_USART_RX_DMA_Init(&USART_InitPara);
//										
//	//ʹ����ͨ�ж�
//	USART_ITConfig(USART2, USART_IT_IDLE, DISABLE); 	//�رմ��ڿ����ж�
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		//����������ͨ�����ж�														
//}

//void USART3_Init(USART_TypeDef* USARTx, BSP_GPIOSource_TypeDef *USART_RX, BSP_GPIOSource_TypeDef *USART_TX, 
//														uint32_t baudRate, uint8_t PreemptionPriority, uint8_t SubPriority){
//	BSP_USART_TypeDef USART_InitPara;
//															
//	USART_InitPara.USARTx = USARTx;
//	USART_InitPara.USART_RX = USART_RX;
//	USART_InitPara.USART_TX = USART_TX;
//	USART_InitPara.USART_InitStructure.USART_BaudRate = baudRate;							
//	USART_InitPara.USART_InitStructure.USART_WordLength = USART_WordLength_8b;	/*�ֳ�Ϊ8λ���ݸ�ʽ*/
//	USART_InitPara.USART_InitStructure.USART_StopBits = USART_StopBits_1;			/*һ��ֹͣλ*/
//	USART_InitPara.USART_InitStructure.USART_Parity = USART_Parity_No;					/*��У��λ*/
//	USART_InitPara.USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;							/*����/����ģʽ*/	
//	USART_InitPara.USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None,	/*��Ӳ������������*/	
//	//��ʼ������
//	BSP_USART_Init(&USART_InitPara,PreemptionPriority,SubPriority);
//	
//	//��ʼ������DMA
////	BSP_USART_TX_DMA_Init(&esp8266_USART);
////	BSP_USART_RX_DMA_Init(&esp8266_USART);
//										
//	//ʹ����ͨ�ж�
//	USART_ITConfig(USART3, USART_IT_IDLE, DISABLE); 	//�رմ��ڿ����ж�
//	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);		//����������ͨ�����ж�														
//}


/*
***************************************************
��������BSP_USART1_DMA_SendData
���ܣ�����1 DMA����һ������
��ڲ�����	Str�������ַ���ָ��
					len�����ݳ���
����ֵ��	0�����ͳɹ�
				1��DMA����δ��������
				2��DMA���ó�ʱ
Ӧ�÷�Χ���ⲿ����
��ע��
***************************************************
*/
#if 0
uint8_t BSP_USART1_DMA_SendData(uint8_t *Array,uint16_t len){
	unsigned char retry=0;
	if(DMA2_Stream7->NDTR){														//DMAͨ�����Ƿ���δ���������
		return 1;
	}
	DMA_Cmd(DMA2_Stream7, DISABLE); 
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE){//ȷ�� DMA ���Ա�����
		retry++;
		if(retry > 200){
			DMA_Cmd(DMA2_Stream7, ENABLE);
			return 2;
		}
	}
	DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7);	//��ձ�־λ
	DMA2_Stream7->M0AR = (uint32_t)Array;
	DMA2_Stream7->NDTR = len;
	DMA_Cmd(DMA2_Stream7, ENABLE);   
	return 0;
}

/*
***************************************************
��������BSP_USART2_DMA_SendData
���ܣ�����2 DMA����һ������
��ڲ�����	Str�������ַ���ָ��
					len������ָ��
����ֵ��	0�����ͳɹ�
				1��DMA����δ��������
				2��DMA���ó�ʱ
Ӧ�÷�Χ���ⲿ����
��ע��
***************************************************
*/
uint8_t BSP_USART2_DMA_SendData(uint8_t *Array,uint16_t len){
	unsigned char retry=0;
	if(DMA1_Stream6->NDTR){														//DMAͨ�����Ƿ���δ���������
		return 1;
	}
	DMA_Cmd(DMA1_Stream6, DISABLE); 
	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE){//ȷ�� DMA ���Ա�����
		retry++;
		if(retry > 200){
			DMA_Cmd(DMA1_Stream6, ENABLE);
			return 2;
		}
	}
	DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6 | DMA_FLAG_HTIF6);	//��ձ�־λ
	DMA1_Stream6->M0AR = (uint32_t)Array;
	DMA1_Stream6->NDTR = len;
	DMA_Cmd(DMA1_Stream6, ENABLE);   
	return 0;
}
#endif

/*
��������: �ַ�������
*/
void USARTx_StringSend(USART_TypeDef *USARTx,char *str){
   while(*str!='\0')
   {
       USARTx->DR=*str++;
       while(!(USARTx->SR&1<<7)){}
   }
}
 
/*
��������: ���ݷ���
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
��������: ��ʽ����ӡ����
*/
char USART1_PRINTF_BUFF[1024];
void USART1_Printf(char *fmt,...){
   va_list ap;
   /*1. ��ʼ���β��б�*/
   va_start(ap,fmt);
   /*2. ��ȡ�ɱ��β�����*/
    vsprintf(USART1_PRINTF_BUFF,fmt,ap);
   /*3. �������ͷſռ�*/
    va_end(ap);
   /*4. ������ݵ�����1*/
   USARTx_StringSend(USART1,USART1_PRINTF_BUFF);
   //USART1_Printf("%d%s",123,454656); 
   //int data=va_arg(ap,int);
}

char USART2_PRINTF_BUFF[1024];
void USART2_Printf(char *fmt,...){
   va_list ap;
   /*1. ��ʼ���β��б�*/
   va_start(ap,fmt);
   /*2. ��ȡ�ɱ��β�����*/
    vsprintf(USART2_PRINTF_BUFF,fmt,ap);
   /*3. �������ͷſռ�*/
    va_end(ap);
   /*4. ������ݵ�����1*/
   USARTx_StringSend(USART2,USART2_PRINTF_BUFF);
   //USART1_Printf("%d%s",123,454656); 
   //int data=va_arg(ap,int);
}

char USART3_PRINTF_BUFF[1024];
void USART3_Printf(char *fmt,...){
   va_list ap;
   /*1. ��ʼ���β��б�*/
   va_start(ap,fmt);
   /*2. ��ȡ�ɱ��β�����*/
    vsprintf(USART3_PRINTF_BUFF,fmt,ap);
   /*3. �������ͷſռ�*/
    va_end(ap);
   /*4. ������ݵ�����1*/
   USARTx_StringSend(USART3,USART3_PRINTF_BUFF);
   //USART1_Printf("%d%s",123,454656); 
   //int data=va_arg(ap,int);
}
#endif















 



