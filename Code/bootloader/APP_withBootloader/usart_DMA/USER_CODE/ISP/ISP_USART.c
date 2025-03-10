#include "bsp_usart.h"
#include "string.h"


#ifdef USART1_Handler_DMA
/*
***************************************************
函数名：USART1_IRQHandler
功能：串口1中断服务函数
备注：本串口中断为空闲中断+DMA中断
***************************************************
*/

//DMA2-2
uint16_t USART1_len;
extern u8 receiveBuffer_is_free;

void USART1_IRQHandler(void){																	
	//接收数据的长度

	//检测是否是空闲中断
	if(USART_GetITStatus(USART1,USART_IT_IDLE) == SET){
		//清USART_IT_IDLE标志
		USART1_len = USART1->SR;
		USART1_len = USART1->DR; 
		//关闭DMA
		DMA_Cmd(DMA2_Stream2,DISABLE); 
		//读取接收数据长度		
		USART1_len = Length_USART1_RX_Buff - DMA2_Stream2->NDTR;
		//清空标志位		
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);	
		
		while(DMA_GetCmdStatus(DMA2_Stream2) != DISABLE);
		DMA2_Stream2->NDTR = Length_USART1_RX_Buff;
		DMA_Cmd(DMA2_Stream2, ENABLE);
		/*********以下是自定义部分**********/
		if(USART1_len){
			
			if(receiveBuffer_is_free){//读取可修改标志位，否则将抛弃这一帧数据
				memcpy(receiveBuffer,uart1_recBuff,USART1_len);
				reciver_size = USART1_len;
			}
		}
		recv_end_flag = USART1_len;
	}
}
#endif

#ifdef USART1_Handler_Command
/*
***************************************************
函数名：UART1_IRQHandler
功能：串口1普通中断服务函数
备注：本串口中断为普通中断
***************************************************
*/
void USART1_IRQHandler(void)                	
	{
		uint8_t res;
		if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)  
		{
			//读取接收到的数据
			res = USART1->DR;        /*USART_ReceiveData(UART1); */ 
			if(res){
				;
			}
		} 
	}
#endif

/*
***************************************************
函数名：USART2_IRQHandler
功能：串口2中断服务函数
备注：本串口中断为空闲中断+DMA中断
***************************************************
*/	
//DMA1-5
void USART2_IRQHandler(void){
	//接收数据的长度	
	uint16_t USART2_len;	
	if(USART_GetITStatus(USART2,USART_IT_IDLE) == SET){
		//清USART_IT_IDLE标志
		USART2_len = USART2->SR;
		USART2_len = USART2->DR; 
		//关闭DMA
		DMA_Cmd(DMA1_Stream5,DISABLE);   
		//清空标志位		
		DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5 | DMA_FLAG_HTIF5);	
		while(DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);
		//读取接收数据长度
		USART2_len = Length_USART2_RX_Buff - DMA1_Stream5->NDTR;	
		DMA1_Stream5->NDTR = Length_USART2_RX_Buff;
		DMA_Cmd(DMA1_Stream5, ENABLE);
		
		/*********以下是自定义部分**********/	

		if(USART2_len){
			if(receiveBuffer_is_free){//读取可修改标志位，否则将抛弃这一帧数据
				memcpy(receiveBuffer,uart2_recBuff,USART2_len);
				reciver_size = USART2_len;
			}
		}
		recv_end_flag = USART2_len;
	}
}

/*
***************************************************
函数名：USART3_IRQHandler
功能：串口3中断服务函数
备注：本串口中断为空闲中断+DMA中断
***************************************************
*/
//DMA1-1
//void USART3_IRQHandler(void){	
//	//接收数据的长度
//	uint16_t USART3_len;	
//	if(USART_GetITStatus(USART3,USART_IT_IDLE) == SET){
//		//清USART_IT_IDLE标志
//		USART3_len = USART3->SR;
//		USART3_len = USART3->DR; 
//		//关闭DMA
//		DMA_Cmd(DMA1_Stream1,DISABLE);    
//		DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);
//		//清空标志位		
//		while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);
//		//读取接收数据长度
//		USART3_len = Length_USART3_RX_Buff - DMA1_Stream1->NDTR;		
//		DMA1_Stream1->NDTR = Length_USART3_RX_Buff;
//		DMA_Cmd(DMA1_Stream1, ENABLE);
//		
//		/*********以下是自定义部分**********/	
//		if(USART3_len){
////			slaveSensorRead((uint8_t*)DMA1_Stream1->M0AR);
////			digitalIncreasing(&slaveSensorData.slaveErrorCount);
//		}
//	}
//}

//void USART3_IRQHandler(void)                	//串口1中断服务程序
//{
//	u8 Res;
//#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//	OSIntEnter();    
//#endif
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//	{
//		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
//		
//		if((USART3_RX_STA&0x8000)==0)//接收未完成
//		{
//			if(USART3_RX_STA&0x4000)//接收到了0x0d
//			{
//				if(Res!=0x0a)USART3_RX_STA=0;//接收错误,重新开始
//				else USART3_RX_STA|=0x8000;	//接收完成了 
//			}
//			else //还没收到0X0D
//			{	
//				if(Res==0x0d)USART3_RX_STA|=0x4000;
//				else
//				{
//					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
//					USART3_RX_STA++;
//					if(USART3_RX_STA>(USART_REC_LEN-1))USART3_RX_STA=0;//接收数据错误,重新开始接收	  
//				}		 
//			}
//		}   		 
//  } 
//#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//	OSIntExit();  											 
//#endif
//} 

/*
***************************************************
函数名：UART4_IRQHandler
功能：串口4中断服务函数
备注：本串口中断为空闲中断+DMA中断
***************************************************
*/
//DMA1-2
void UART4_IRQHandler(void){																			
	//接收数据的长度
	uint16_t UART4_len;	
	if(USART_GetITStatus(UART4,USART_IT_IDLE) == SET){
		//清USART_IT_IDLE标志
		UART4_len = UART4->SR;
		UART4_len = UART4->DR; 
		//关闭DMA
		DMA_Cmd(DMA1_Stream2,DISABLE); 
		//清空标志位		
		DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);	
		while(DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);
		//读取接收数据长度
		UART4_len = Length_UART4_RX_Buff - DMA1_Stream2->NDTR;				
		DMA1_Stream2->NDTR = Length_UART4_RX_Buff;
		DMA_Cmd(DMA1_Stream2, ENABLE);
		
		/*********以下是自定义部分**********/	
		if(UART4_len){
//			if(receiveBuffer_is_free){//读取可修改标志位，否则将抛弃这一帧数据
				memcpy(receiveBuffer,uart4_recBuff,UART4_len);
				reciver_size = UART4_len;
//			}	
		};	//这里只是为了减少warning提示，使用时可以删掉
		
		recv_end_flag = UART4_len;
	}
}
//void UART4_IRQHandler(void){
//	//接收数据的长度	
//	uint16_t UART4_len;	
//	if(USART_GetITStatus(UART4,USART_IT_IDLE) == SET){
//		//清USART_IT_IDLE标志
//		UART4_len = UART4->SR;
//		UART4_len = UART4->DR; 
//		//关闭DMA
//		DMA_Cmd(DMA1_Stream2,DISABLE);   
//		//清空标志位		
//		DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);	
//		while(DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);
//		//读取接收数据长度
//		UART4_len = Length_UART4_RX_Buff - DMA1_Stream2->NDTR;	
//		DMA1_Stream2->NDTR = Length_UART4_RX_Buff;
//		DMA_Cmd(DMA1_Stream2, ENABLE);
//		
//		/*********以下是自定义部分**********/	

//		if(UART4_len){
//			if(receiveBuffer_is_free){//读取可修改标志位，否则将抛弃这一帧数据
//				memcpy(receiveBuffer,uart4_recBuff,UART4_len);
//				reciver_size = UART4_len;
//			}
//		}
//		recv_end_flag = UART4_len;
//	}
//}

/*
***************************************************
函数名：UART5_IRQHandler
功能：串口5中断服务函数
备注：本串口中断为空闲中断+DMA中断
***************************************************
*/
//DMA1-0
//void UART5_IRQHandler(void){																			
//	//接收数据的长度
//	uint16_t UART5_len;	
//	if(USART_GetITStatus(UART5,USART_IT_IDLE) == SET){
//		//清USART_IT_IDLE标志
//		UART5_len = UART5->SR;
//		UART5_len = UART5->DR; 
//		//关闭DMA
//		DMA_Cmd(DMA1_Stream0,DISABLE); 
//		//清空标志位
//		DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0 | DMA_FLAG_HTIF0);	
//		while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);
//		//读取接收数据长度
//		UART5_len = Length_UART5_RX_Buff - DMA1_Stream0->NDTR;	
//		DMA1_Stream0->NDTR = Length_UART5_RX_Buff;
//		DMA_Cmd(DMA1_Stream0, ENABLE);
//		
//		/*********以下是自定义部分**********/	
//		if(UART5_len){
//		}
//	}
//}

/*
***************************************************
函数名：USART6_IRQHandler
功能：串口6中断服务函数
备注：本串口中断为空闲中断+DMA中断
***************************************************
*/
//DMA2-6
//void USART6_IRQHandler(void){	
//	//接收数据的长度
//	uint16_t USART6_len;	
//	if(USART_GetITStatus(USART6,USART_IT_IDLE) == SET){
//		//清USART_IT_IDLE标志
//		USART6_len = USART6->SR;
//		USART6_len = USART6->DR; 
//		//关闭DMA
//		DMA_Cmd(DMA2_Stream1,DISABLE); 
//		//清空标志位		
//		DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);	
//		while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE);
//		//读取接收数据长度
//		USART6_len = Length_USART6_RX_Buff - DMA2_Stream1->NDTR;			
//		DMA2_Stream1->NDTR = Length_USART6_RX_Buff;
//		DMA_Cmd(DMA2_Stream1, ENABLE);
//		
//		/*********以下是自定义部分**********/	
//		if(USART6_len){
//		}	//这里只是为了减少warning提示，使用时可以删掉
//	}
//}


/*
***************************************************
函数名：UART7_IRQHandler
功能：串口7中断服务函数
备注：本串口中断为空闲中断+DMA中断
***************************************************
*/
//DMA1-3
#ifdef UART7_Handler_DMA
	void UART7_IRQHandler(void){	
		//接收数据的长度
		uint16_t UART7_len;	
		if(USART_GetITStatus(UART7,USART_IT_IDLE) == SET){
			//清USART_IT_IDLE标志
			UART7_len = UART7->SR;
			UART7_len = UART7->DR; 
			//关闭DMA
			DMA_Cmd(DMA1_Stream3,DISABLE);
			//清空标志位
			DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3 | DMA_FLAG_HTIF3);	
			while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);
			//读取接收数据长度
			UART7_len = Length_UART7_RX_Buff - DMA1_Stream3->NDTR;				
			DMA1_Stream3->NDTR = Length_UART7_RX_Buff;
			DMA_Cmd(DMA1_Stream3, ENABLE);
			
			/*********以下是自定义部分**********/	
			if(UART7_len){
			;
			}
		}
	}
#endif

/*
***************************************************
函数名：UART7_IRQHandler
功能：串口7普通中断服务函数
备注：本串口中断为普通中断
***************************************************
*/
#ifdef UART7_Handler_Command
	void UART7_IRQHandler(void)                	
	{
		static uint16_t RxCount = 0;
		uint8_t res;
		if(USART_GetITStatus(UART7,USART_IT_RXNE) != RESET)  
		{
			//读取接收到的数据
			res = UART7->DR;        /*USART_ReceiveData(UART7); */ 
			Array_UART7_RX[RxCount++] = res; 
			if(RxCount==5)
			{
			 RxCount=0;
			 GetupperMonitorReceiveData(Array_UART7_RX);
			}	
		} 
	}
#endif

/*
***************************************************
函数名：UART8_IRQHandler
功能：串口8中断服务函数
备注：本串口中断为空闲中断+DMA中断					
***************************************************
*/
//DMA1-6
/********串口8DMA1数据流与裁判系统串口2DMA数据流冲突*************/

//void UART8_IRQHandler(void){	
//		//接收数据的长度
//		uint16_t UART8_len;	
//		if(USART_GetITStatus(UART8,USART_IT_IDLE) == SET){
//			//清USART_IT_IDLE标志
//			UART8_len = UART8->SR;
//			UART8_len = UART8->DR; 
//			
//			//关闭DMA
//			DMA_Cmd(DMA1_Stream6,DISABLE); 
//			//清空标志位
//			DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6 | DMA_FLAG_HTIF6);	
//			while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);
//			//读取接收数据长度
//			UART8_len = Length_UART8_RX_Buff - DMA1_Stream6->NDTR;				
//			DMA1_Stream6->NDTR = Length_UART8_RX_Buff;
//			DMA_Cmd(DMA1_Stream6, ENABLE);
//			
//			/*********以下是自定义部分**********/	
//			if(UART8_len){
//			  rs485ReceiveData(Array_UART8_RX,YAW);
//			}
//		}
//}

///*串口8中断服务函数*/
//void UART8_IRQHandler(void){		
//	if(USART_GetITStatus(UART8, USART_IT_RXNE) != RESET){
//	}
//}
