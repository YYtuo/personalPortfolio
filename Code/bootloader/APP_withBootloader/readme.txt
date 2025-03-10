
APP的地址为0x0801 0000
注意：APP程序需要对中断向量表进行偏移，在system_stm32f4xx.c中，修改
“#define VECT_TAB_OFFSET  0x10000”

