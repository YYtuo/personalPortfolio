#include "communicate.h"	

#ifdef PRO
struct TestData{
	uint8_t test[4];
};

ReceiveStr receiver = {0};
ProtocolStr protocol={0};
int decode_size=0;//�����������Ч���ݳ���
struct TestData testData;//Ϊ�˷���������ݣ���ʼ��һ���ṹ����ֱ�ӽ�����ת�����ṹ�������
#endif

/**********************************
 * ��������communicate_init
 * ���ܣ�ͨ�Ź��ܳ�ʼ��
 * ���룺
 * �������
 * ��ע����
**********************************/
void communicate_init(void){
	USARTInit(USART1,US1_Rx, US1_Tx, 115200,1,1,uart1_sendBuff, uart1_recBuff,Length_USART1_RX_Buff);
	USARTInit(USART2,US2_Rx, US2_Tx, 115200,1,2,uart2_sendBuff, uart2_recBuff,Length_USART2_RX_Buff);
	
	#ifdef PRO
		//��ʼ��Э��
		init_protocol(&protocol, 0xabcd, 0xbcde, 0xcdef, 0);
		//��Э��󶨵����սṹ����
		bind_receive(&receiver, &protocol);
	#endif

}


/*=======================demo==============================*/
//struct TestData{
//	uint8_t test[4];
//};

//ReceiveStr receiver = {0};
//ProtocolStr protocol={0};
//int decode_size=0;//�����������Ч���ݳ���
//struct TestData testData;//Ϊ�˷���������ݣ���ʼ��һ���ṹ����ֱ�ӽ�����ת�����ṹ�������

//void communicate_WithProtocol(void){
//	
//	/*��ʼ��������ֻ����һ��*/
//	
//	USARTInit(USART1,US1_Rx, US1_Tx, 115200,1,1,uart1_sendBuff, uart1_recBuff,Length_USART1_RX_Buff);
//	USARTInit(USART2,US2_Rx, US2_Tx, 115200,1,2,uart2_sendBuff, uart2_recBuff,Length_USART2_RX_Buff);
//	//��ʼ��Э��
//	init_protocol(&protocol, 0xabcd, 0xbcde, 0xcdef, 0);
//	//��Э��󶨵����սṹ����
//	bind_receive(&receiver, &protocol);
//	/*end of ��ʼ������*/
//	
//	if(reciver_size>0){
//		receiveBuffer_is_free=0;//����Ҫ�������ݣ���Ҫ��dma�޸�receiveBuffer�������
//		send_to_processes_buffer(&receiver,receiveBuffer,reciver_size);//���������봦������
//		reciver_size=0;//�Ѿ���������������ˣ����buf
//		receiveBuffer_is_free=1;//�ͷſ��޸������Ѿ�������ϣ�DMA���Լ���������buf�������
//		
//		decode_size=unpack_buf(&receiver,0);//ִ�н������
//		//������0����ɹ�������һ�����ݰ�������δ�������κ���Ч����
//		if(decode_size!=0){
//			//Ϊ���ȶ��������ж�һ���Ƿ��������ṹ�峤��һֱ����������λ�������������ݵ�ʱ���޷��ֱ浼������ڴ����
//			if(sizeof(struct TestData)==decode_size){
//				uint8_t *data_ptr_u8=get_data(&receiver);//ȡ��ָ����Ч���ݵĵ�һ���ֽڵĵ�ַ
//				struct TestData *data_ptr=(struct TestData *)data_ptr_u8;//ָ��ǿ��ת��
//				testData=*data_ptr;//��ָ���ȡֵ��ֵ��mydata
//				//��Ч���±�������
//				//memcpy(&mydata,data_ptr_u8,sizeof(struct MyData));
//				
//				#define Response
//				#ifdef Response
//				//���յ������ݷ��ظ����ͷ�
//				uint16_t send_size=create_packet(uart1_sendBuff,(uint8_t *)&testData,sizeof(struct TestData),&protocol,0,0);//������ݣ��õ������İ����ݳ���
////				u1_sendlendata(sendBuffer, send_size);//��������
//				BSP_USART1_DMA_SendData(uart1_sendBuff,send_size);//��������
//				#endif
//				
//			}
//		
//		}
//	}
//	
//	
//}


