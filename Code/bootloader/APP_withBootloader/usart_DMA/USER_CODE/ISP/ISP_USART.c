#include "bsp_usart.h"
#include "string.h"


#ifdef USART1_Handler_DMA
/*
***************************************************
��������USART1_IRQHandler
���ܣ�����1�жϷ�����
��ע���������ж�Ϊ�����ж�+DMA�ж�
***************************************************
*/

//DMA2-2
uint16_t USART1_len;
extern u8 receiveBuffer_is_free;

void USART1_IRQHandler(void){																	
	//�������ݵĳ���

	//����Ƿ��ǿ����ж�
	if(USART_GetITStatus(USART1,USART_IT_IDLE) == SET){
		//��USART_IT_IDLE��־
		USART1_len = USART1->SR;
		USART1_len = USART1->DR; 
		//�ر�DMA
		DMA_Cmd(DMA2_Stream2,DISABLE); 
		//��ȡ�������ݳ���		
		USART1_len = Length_USART1_RX_Buff - DMA2_Stream2->NDTR;
		//��ձ�־λ		
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);	
		
		while(DMA_GetCmdStatus(DMA2_Stream2) != DISABLE);
		DMA2_Stream2->NDTR = Length_USART1_RX_Buff;
		DMA_Cmd(DMA2_Stream2, ENABLE);
		/*********�������Զ��岿��**********/
		if(USART1_len){
			
			if(receiveBuffer_is_free){//��ȡ���޸ı�־λ������������һ֡����
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
��������UART1_IRQHandler
���ܣ�����1��ͨ�жϷ�����
��ע���������ж�Ϊ��ͨ�ж�
***************************************************
*/
void USART1_IRQHandler(void)                	
	{
		uint8_t res;
		if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)  
		{
			//��ȡ���յ�������
			res = USART1->DR;        /*USART_ReceiveData(UART1); */ 
			if(res){
				;
			}
		} 
	}
#endif

/*
***************************************************
��������USART2_IRQHandler
���ܣ�����2�жϷ�����
��ע���������ж�Ϊ�����ж�+DMA�ж�
***************************************************
*/	
//DMA1-5
void USART2_IRQHandler(void){
	//�������ݵĳ���	
	uint16_t USART2_len;	
	if(USART_GetITStatus(USART2,USART_IT_IDLE) == SET){
		//��USART_IT_IDLE��־
		USART2_len = USART2->SR;
		USART2_len = USART2->DR; 
		//�ر�DMA
		DMA_Cmd(DMA1_Stream5,DISABLE);   
		//��ձ�־λ		
		DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5 | DMA_FLAG_HTIF5);	
		while(DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);
		//��ȡ�������ݳ���
		USART2_len = Length_USART2_RX_Buff - DMA1_Stream5->NDTR;	
		DMA1_Stream5->NDTR = Length_USART2_RX_Buff;
		DMA_Cmd(DMA1_Stream5, ENABLE);
		
		/*********�������Զ��岿��**********/	

		if(USART2_len){
			if(receiveBuffer_is_free){//��ȡ���޸ı�־λ������������һ֡����
				memcpy(receiveBuffer,uart2_recBuff,USART2_len);
				reciver_size = USART2_len;
			}
		}
		recv_end_flag = USART2_len;
	}
}

/*
***************************************************
��������USART3_IRQHandler
���ܣ�����3�жϷ�����
��ע���������ж�Ϊ�����ж�+DMA�ж�
***************************************************
*/
//DMA1-1
//void USART3_IRQHandler(void){	
//	//�������ݵĳ���
//	uint16_t USART3_len;	
//	if(USART_GetITStatus(USART3,USART_IT_IDLE) == SET){
//		//��USART_IT_IDLE��־
//		USART3_len = USART3->SR;
//		USART3_len = USART3->DR; 
//		//�ر�DMA
//		DMA_Cmd(DMA1_Stream1,DISABLE);    
//		DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);
//		//��ձ�־λ		
//		while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);
//		//��ȡ�������ݳ���
//		USART3_len = Length_USART3_RX_Buff - DMA1_Stream1->NDTR;		
//		DMA1_Stream1->NDTR = Length_USART3_RX_Buff;
//		DMA_Cmd(DMA1_Stream1, ENABLE);
//		
//		/*********�������Զ��岿��**********/	
//		if(USART3_len){
////			slaveSensorRead((uint8_t*)DMA1_Stream1->M0AR);
////			digitalIncreasing(&slaveSensorData.slaveErrorCount);
//		}
//	}
//}

//void USART3_IRQHandler(void)                	//����1�жϷ������
//{
//	u8 Res;
//#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntEnter();    
//#endif
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//	{
//		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
//		
//		if((USART3_RX_STA&0x8000)==0)//����δ���
//		{
//			if(USART3_RX_STA&0x4000)//���յ���0x0d
//			{
//				if(Res!=0x0a)USART3_RX_STA=0;//���մ���,���¿�ʼ
//				else USART3_RX_STA|=0x8000;	//��������� 
//			}
//			else //��û�յ�0X0D
//			{	
//				if(Res==0x0d)USART3_RX_STA|=0x4000;
//				else
//				{
//					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
//					USART3_RX_STA++;
//					if(USART3_RX_STA>(USART_REC_LEN-1))USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//				}		 
//			}
//		}   		 
//  } 
//#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntExit();  											 
//#endif
//} 

/*
***************************************************
��������UART4_IRQHandler
���ܣ�����4�жϷ�����
��ע���������ж�Ϊ�����ж�+DMA�ж�
***************************************************
*/
//DMA1-2
void UART4_IRQHandler(void){																			
	//�������ݵĳ���
	uint16_t UART4_len;	
	if(USART_GetITStatus(UART4,USART_IT_IDLE) == SET){
		//��USART_IT_IDLE��־
		UART4_len = UART4->SR;
		UART4_len = UART4->DR; 
		//�ر�DMA
		DMA_Cmd(DMA1_Stream2,DISABLE); 
		//��ձ�־λ		
		DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);	
		while(DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);
		//��ȡ�������ݳ���
		UART4_len = Length_UART4_RX_Buff - DMA1_Stream2->NDTR;				
		DMA1_Stream2->NDTR = Length_UART4_RX_Buff;
		DMA_Cmd(DMA1_Stream2, ENABLE);
		
		/*********�������Զ��岿��**********/	
		if(UART4_len){
//			if(receiveBuffer_is_free){//��ȡ���޸ı�־λ������������һ֡����
				memcpy(receiveBuffer,uart4_recBuff,UART4_len);
				reciver_size = UART4_len;
//			}	
		};	//����ֻ��Ϊ�˼���warning��ʾ��ʹ��ʱ����ɾ��
		
		recv_end_flag = UART4_len;
	}
}
//void UART4_IRQHandler(void){
//	//�������ݵĳ���	
//	uint16_t UART4_len;	
//	if(USART_GetITStatus(UART4,USART_IT_IDLE) == SET){
//		//��USART_IT_IDLE��־
//		UART4_len = UART4->SR;
//		UART4_len = UART4->DR; 
//		//�ر�DMA
//		DMA_Cmd(DMA1_Stream2,DISABLE);   
//		//��ձ�־λ		
//		DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);	
//		while(DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);
//		//��ȡ�������ݳ���
//		UART4_len = Length_UART4_RX_Buff - DMA1_Stream2->NDTR;	
//		DMA1_Stream2->NDTR = Length_UART4_RX_Buff;
//		DMA_Cmd(DMA1_Stream2, ENABLE);
//		
//		/*********�������Զ��岿��**********/	

//		if(UART4_len){
//			if(receiveBuffer_is_free){//��ȡ���޸ı�־λ������������һ֡����
//				memcpy(receiveBuffer,uart4_recBuff,UART4_len);
//				reciver_size = UART4_len;
//			}
//		}
//		recv_end_flag = UART4_len;
//	}
//}

/*
***************************************************
��������UART5_IRQHandler
���ܣ�����5�жϷ�����
��ע���������ж�Ϊ�����ж�+DMA�ж�
***************************************************
*/
//DMA1-0
//void UART5_IRQHandler(void){																			
//	//�������ݵĳ���
//	uint16_t UART5_len;	
//	if(USART_GetITStatus(UART5,USART_IT_IDLE) == SET){
//		//��USART_IT_IDLE��־
//		UART5_len = UART5->SR;
//		UART5_len = UART5->DR; 
//		//�ر�DMA
//		DMA_Cmd(DMA1_Stream0,DISABLE); 
//		//��ձ�־λ
//		DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0 | DMA_FLAG_HTIF0);	
//		while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);
//		//��ȡ�������ݳ���
//		UART5_len = Length_UART5_RX_Buff - DMA1_Stream0->NDTR;	
//		DMA1_Stream0->NDTR = Length_UART5_RX_Buff;
//		DMA_Cmd(DMA1_Stream0, ENABLE);
//		
//		/*********�������Զ��岿��**********/	
//		if(UART5_len){
//		}
//	}
//}

/*
***************************************************
��������USART6_IRQHandler
���ܣ�����6�жϷ�����
��ע���������ж�Ϊ�����ж�+DMA�ж�
***************************************************
*/
//DMA2-6
//void USART6_IRQHandler(void){	
//	//�������ݵĳ���
//	uint16_t USART6_len;	
//	if(USART_GetITStatus(USART6,USART_IT_IDLE) == SET){
//		//��USART_IT_IDLE��־
//		USART6_len = USART6->SR;
//		USART6_len = USART6->DR; 
//		//�ر�DMA
//		DMA_Cmd(DMA2_Stream1,DISABLE); 
//		//��ձ�־λ		
//		DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);	
//		while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE);
//		//��ȡ�������ݳ���
//		USART6_len = Length_USART6_RX_Buff - DMA2_Stream1->NDTR;			
//		DMA2_Stream1->NDTR = Length_USART6_RX_Buff;
//		DMA_Cmd(DMA2_Stream1, ENABLE);
//		
//		/*********�������Զ��岿��**********/	
//		if(USART6_len){
//		}	//����ֻ��Ϊ�˼���warning��ʾ��ʹ��ʱ����ɾ��
//	}
//}


/*
***************************************************
��������UART7_IRQHandler
���ܣ�����7�жϷ�����
��ע���������ж�Ϊ�����ж�+DMA�ж�
***************************************************
*/
//DMA1-3
#ifdef UART7_Handler_DMA
	void UART7_IRQHandler(void){	
		//�������ݵĳ���
		uint16_t UART7_len;	
		if(USART_GetITStatus(UART7,USART_IT_IDLE) == SET){
			//��USART_IT_IDLE��־
			UART7_len = UART7->SR;
			UART7_len = UART7->DR; 
			//�ر�DMA
			DMA_Cmd(DMA1_Stream3,DISABLE);
			//��ձ�־λ
			DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3 | DMA_FLAG_HTIF3);	
			while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);
			//��ȡ�������ݳ���
			UART7_len = Length_UART7_RX_Buff - DMA1_Stream3->NDTR;				
			DMA1_Stream3->NDTR = Length_UART7_RX_Buff;
			DMA_Cmd(DMA1_Stream3, ENABLE);
			
			/*********�������Զ��岿��**********/	
			if(UART7_len){
			;
			}
		}
	}
#endif

/*
***************************************************
��������UART7_IRQHandler
���ܣ�����7��ͨ�жϷ�����
��ע���������ж�Ϊ��ͨ�ж�
***************************************************
*/
#ifdef UART7_Handler_Command
	void UART7_IRQHandler(void)                	
	{
		static uint16_t RxCount = 0;
		uint8_t res;
		if(USART_GetITStatus(UART7,USART_IT_RXNE) != RESET)  
		{
			//��ȡ���յ�������
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
��������UART8_IRQHandler
���ܣ�����8�жϷ�����
��ע���������ж�Ϊ�����ж�+DMA�ж�					
***************************************************
*/
//DMA1-6
/********����8DMA1�����������ϵͳ����2DMA��������ͻ*************/

//void UART8_IRQHandler(void){	
//		//�������ݵĳ���
//		uint16_t UART8_len;	
//		if(USART_GetITStatus(UART8,USART_IT_IDLE) == SET){
//			//��USART_IT_IDLE��־
//			UART8_len = UART8->SR;
//			UART8_len = UART8->DR; 
//			
//			//�ر�DMA
//			DMA_Cmd(DMA1_Stream6,DISABLE); 
//			//��ձ�־λ
//			DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6 | DMA_FLAG_HTIF6);	
//			while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);
//			//��ȡ�������ݳ���
//			UART8_len = Length_UART8_RX_Buff - DMA1_Stream6->NDTR;				
//			DMA1_Stream6->NDTR = Length_UART8_RX_Buff;
//			DMA_Cmd(DMA1_Stream6, ENABLE);
//			
//			/*********�������Զ��岿��**********/	
//			if(UART8_len){
//			  rs485ReceiveData(Array_UART8_RX,YAW);
//			}
//		}
//}

///*����8�жϷ�����*/
//void UART8_IRQHandler(void){		
//	if(USART_GetITStatus(UART8, USART_IT_RXNE) != RESET){
//	}
//}
