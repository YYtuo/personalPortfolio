V1.0
1、从core_demo_V1.8中移植而来，功能与其相同。
2、APP的地址为0x0801 0000
注意：APP程序需要对中断向量表进行偏移，在system_stm32f4xx.c中，修改
“#define VECT_TAB_OFFSET  0x10000”

V1.2
使用通信协议，对遥控器的数据进行转发