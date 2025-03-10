#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#ifndef __BSP_USART_H
#define __BSP_USART_H
//#include "stdio.h"	
//#include "string.h"
//#include "stdarg.h"	 

#include "stm32f4xx_conf.h"
#include "BSP_GPIO.h"

//#define PRINT //是否使用自定义的printf函数
#define USART1_Handler_DMA

//宏定义串口接收数组长度
#define	Length_USART1_RX_Buff 	256
#define	Length_USART2_RX_Buff 	256
//#define	Length_USART3_RX_Buff 	256
#define	Length_UART4_RX_Buff 	256
//#define	Length_UART5_RX_Buff 	256
//#define	Length_USART6_RX_Buff 	128
//#define	Length_UART7_RX_Buff 	256
//#define	Length_UART8_RX_Buff 	256

//宏定义串口发送数组长度
#define	Length_USART1_TX_Buff 	256
#define	Length_USART2_TX_Buff 	256
//#define	Length_USART3_TX_Buff 	256
#define	Length_UART4_TX_Buff 	256
//#define	Length_UART5_TX_Buff 	256
//#define	Length_USART6_TX_Buff 	128
//#define	Length_UART7_TX_Buff 	256
//#define	Length_UART8_TX_Buff 	256



//#define USART_REC_LEN  			256  	//定义最大接收字节数 200
//#define USART_SEND_LEN			128		//定义最大发送字节数 128

//#define USART3_RX_LENGTH 		256

//#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收

#define US1_Rx   	BSP_GPIOB7
#define US1_Tx   	BSP_GPIOB6

#define US2_Rx   	BSP_GPIOD5
#define US2_Tx   	BSP_GPIOD6

// 串口3
//#define WIFI_Tx 	BSP_GPIOD8
//#define WIFI_Rx 	BSP_GPIOD9

// 串口4
#define WIFI_Tx 	BSP_GPIOA0
#define WIFI_Rx 	BSP_GPIOA1


#ifdef PRINT
void USART1_Printf(char *fmt,...);
void USART2_Printf(char *fmt,...);
void USART3_Printf(char *fmt,...);
#endif

enum {
	DMA_off = 0,
	DMA_on 
};

//extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 recv_end_flag;         		//接收状态标记	
extern u8 receiveBuffer_is_free;
extern u8 receiveBuffer[];
extern int reciver_size;

extern uint8_t uart1_sendBuff[];
extern uint8_t uart1_recBuff[];
extern uint8_t uart2_sendBuff[];
extern uint8_t uart2_recBuff[];

extern uint8_t uart4_sendBuff[];
extern uint8_t uart4_recBuff[];

//extern u8 USART3_RX_BUFFER[USART3_RX_LENGTH]; //保存接收数据的缓冲区
//extern u32 USART3_RX_CNT;  //当前接收到的数据长度
//extern u8 USART3_RX_FLAG; //1表示数据接收完毕 0表示没有接收完毕


//void USART1_Init(u32 bound);
void USARTInit(USART_TypeDef* USARTx, BSP_GPIOSource_TypeDef *USART_RX, BSP_GPIOSource_TypeDef *USART_TX, 
                    uint32_t baudRate, uint8_t PreemptionPriority, uint8_t SubPriority, uint8_t uart_sendBuff[],uint8_t uart_recBuff[],uint16_t rec_bufLen);
void USART2_Init(USART_TypeDef* USARTx, BSP_GPIOSource_TypeDef *USART_RX, BSP_GPIOSource_TypeDef *USART_TX, 
				uint32_t baudRate, uint8_t PreemptionPriority, uint8_t SubPriority);
void USART3_Init(USART_TypeDef* USARTx, BSP_GPIOSource_TypeDef *USART_RX, BSP_GPIOSource_TypeDef *USART_TX, 
				uint32_t baudRate, uint8_t PreemptionPriority, uint8_t SubPriority);
void UART4_Init(USART_TypeDef* USARTx, BSP_GPIOSource_TypeDef *USART_RX, BSP_GPIOSource_TypeDef *USART_TX, 
				uint32_t baudRate, uint8_t PreemptionPriority, uint8_t SubPriority);


uint8_t USART1_SendBuffer(u8 *buffer, u16 n);
void USARTx_DataSend(USART_TypeDef *USARTx,u8 *data,u32 len);
	
uint8_t BSP_USART1_DMA_SendData(uint8_t *Array,uint16_t len);
uint8_t BSP_USART2_DMA_SendData(uint8_t *Array,uint16_t len);

uint8_t BSP_UART4_DMA_SendData(uint8_t *Array,uint16_t len);



#define UART4_DMA_RX_BUFFER_MAX_LENGTH		(255)
#define UART4_DMA_TX_BUFFER_MAX_LENGTH		(255)
extern uint8_t UART4_DMA_RX_Buffer[];
extern uint8_t UART4_DMA_TX_Buffer[];

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
