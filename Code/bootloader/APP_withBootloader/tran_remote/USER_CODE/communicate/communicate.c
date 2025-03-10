#include "communicate.h"	

#ifdef PRO
struct TestData{
	uint8_t test[4];
};

ReceiveStr receiver = {0};
ProtocolStr protocol={0};
int decode_size=0;//解码出来的有效数据长度
struct TestData testData;//为了方便解析数据，初始化一个结构体来直接将数据转换到结构体的数据
#endif

/**********************************
 * 函数名：communicate_init
 * 功能：通信功能初始化
 * 输入：
 * 输出：无
 * 备注：无
**********************************/
void communicate_init(void){
	USARTInit(USART1,US1_Rx, US1_Tx, 115200,1,1,uart1_sendBuff, uart1_recBuff,Length_USART1_RX_Buff);
	USARTInit(USART2,US2_Rx, US2_Tx, 115200,1,2,uart2_sendBuff, uart2_recBuff,Length_USART2_RX_Buff);
	
	#ifdef PRO
		//初始化协议
		init_protocol(&protocol, 0xabcd, 0xbcde, 0xcdef, 0);
		//将协议绑定到接收结构体中
		bind_receive(&receiver, &protocol);
	#endif

}


/*=======================demo==============================*/
//struct TestData{
//	uint8_t test[4];
//};

//ReceiveStr receiver = {0};
//ProtocolStr protocol={0};
//int decode_size=0;//解码出来的有效数据长度
//struct TestData testData;//为了方便解析数据，初始化一个结构体来直接将数据转换到结构体的数据

//void communicate_WithProtocol(void){
//	
//	/*初始化函数，只调用一次*/
//	
//	USARTInit(USART1,US1_Rx, US1_Tx, 115200,1,1,uart1_sendBuff, uart1_recBuff,Length_USART1_RX_Buff);
//	USARTInit(USART2,US2_Rx, US2_Tx, 115200,1,2,uart2_sendBuff, uart2_recBuff,Length_USART2_RX_Buff);
//	//初始化协议
//	init_protocol(&protocol, 0xabcd, 0xbcde, 0xcdef, 0);
//	//将协议绑定到接收结构体中
//	bind_receive(&receiver, &protocol);
//	/*end of 初始化函数*/
//	
//	if(reciver_size>0){
//		receiveBuffer_is_free=0;//即将要拷贝数据，不要让dma修改receiveBuffer里的数据
//		send_to_processes_buffer(&receiver,receiveBuffer,reciver_size);//将数据送入处理缓冲区
//		reciver_size=0;//已经拷贝过这段数据了，清空buf
//		receiveBuffer_is_free=1;//释放可修改锁，已经拷贝完毕，DMA可以继续往接收buf填充数据
//		
//		decode_size=unpack_buf(&receiver,0);//执行解包数据
//		//不等于0代表成功解析出一个数据包，否则未解析出任何有效数据
//		if(decode_size!=0){
//			//为了稳定起见最好判断一下是否包长度与结构体长度一直，否则若上位机发送其他数据的时候无法分辨导致这边内存错误
//			if(sizeof(struct TestData)==decode_size){
//				uint8_t *data_ptr_u8=get_data(&receiver);//取出指向有效数据的第一个字节的地址
//				struct TestData *data_ptr=(struct TestData *)data_ptr_u8;//指针强制转换
//				testData=*data_ptr;//将指针的取值赋值给mydata
//				//等效于下边这句代码
//				//memcpy(&mydata,data_ptr_u8,sizeof(struct MyData));
//				
//				#define Response
//				#ifdef Response
//				//将收到是数据返回给发送方
//				uint16_t send_size=create_packet(uart1_sendBuff,(uint8_t *)&testData,sizeof(struct TestData),&protocol,0,0);//封包数据，得到封包后的包数据长度
////				u1_sendlendata(sendBuffer, send_size);//发送数据
//				BSP_USART1_DMA_SendData(uart1_sendBuff,send_size);//发送数据
//				#endif
//				
//			}
//		
//		}
//	}
//	
//	
//}


