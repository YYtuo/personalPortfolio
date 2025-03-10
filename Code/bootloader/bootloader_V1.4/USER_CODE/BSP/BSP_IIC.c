/***************************************************
 * mode name:BSP_IIC
 * brief:iic底层驱动，
 * @author tuo
 * @created 2022/12/29 21:31:13
 * @imfor H:\library\STM32\test\F4\core_boardV1.2\bootloader\bootloader_V1.2\USER_CODE\BSP\BSP_IIC.c
***************************************************/
#include "BSP_IIC.h"

/**********************************
 * 函数名：BSP_iic_bus_parameterInit
 * 功能：iic总线对象 参数 初始化函数
 * 输入：
 * 输出：无
 * 备注：无
**********************************/
void BSP_iic_bus_parameterInit(iic_bus_TypeDef* iic_bus,
                                    uint8_t para_iic_pull_up_flag,
                                    uint8_t para_BUS_ID,
                                    
                                    BSP_GPIOSource_TypeDef* para_BSP_IIC_SCL_GPIO,
	                                BSP_GPIOSource_TypeDef* para_BSP_IIC_SDA_GPIO,

                                    volatile unsigned long* para_IIC_SCL,
	                                volatile unsigned long* para_IIC_SDA,
                                    volatile unsigned long* para_READ_SCL,
	                                volatile unsigned long* para_READ_SDA,
                                    
                                    void(*para_IIC_GPIO_Init)\
		                            (iic_bus_TypeDef* iic_bus),
                                    void(*para_SDA_IN)\
		                            (BSP_GPIOSource_TypeDef* BSP_IIC_SDA_GPIO),
	                                void(*para_SDA_OUT)\
		                            (BSP_GPIOSource_TypeDef* BSP_IIC_SDA_GPIO),
                                    void (*para_IIC_Delay)(void)){
    // 上拉电阻标志位初始化
    iic_bus->iic_pull_up_flag = para_iic_pull_up_flag;
    // 地址初始化
    iic_bus->BUS_ID = para_BUS_ID;
	// IIC总线引脚位号初始化 
    iic_bus->BSP_IIC_SCL_GPIO = para_BSP_IIC_SCL_GPIO;
    iic_bus->BSP_IIC_SDA_GPIO = para_BSP_IIC_SDA_GPIO;

    // 总线初始化
    iic_bus->IIC_SCL = para_IIC_SCL;
    iic_bus->IIC_SDA = para_IIC_SDA;
    iic_bus->READ_SCL = para_READ_SCL;
    iic_bus->READ_SDA = para_READ_SDA;

    // GPIO初始化
    iic_bus->IIC_GPIO_Init = para_IIC_GPIO_Init;
    iic_bus->IIC_GPIO_Init(iic_bus);
	// SDA方向设置函数初始化
    iic_bus->SDA_IN = para_SDA_IN;
    iic_bus->SDA_OUT = para_SDA_OUT;
    // 延时初始化
    iic_bus->IIC_Delay = para_IIC_Delay;

}

#ifdef IIC_DEMO
/*IIC_GPIO_Init_demo*/
void IIC_GPIO_Init_demo(iic_bus_TypeDef* iic_bus){
    
    // 总线有上拉电阻，初始化为开漏输出，避免来回切换SDA方向 
    if(iic_bus->iic_pull_up_flag){
    	BSP_GPIO_Init(iic_bus->BSP_IIC_SDA_GPIO, GPIO_Mode_Out_OD);
    }

    // 无上拉电阻则初始化为推挽
    else{
    	BSP_GPIO_Init(iic_bus->BSP_IIC_SDA_GPIO, GPIO_Mode_Out_PP);
    }
    BSP_GPIO_Init(iic_bus->BSP_IIC_SCL_GPIO, GPIO_Mode_Out_PP);
	*(iic_bus->IIC_SCL) = 1;
    *(iic_bus->IIC_SDA) = 1;
}

/*SDA_IN_demo*/
void SDA_IN_demo(BSP_GPIOSource_TypeDef* BSP_IIC_SDA_GPIO){
    BSP_IIC_SDA_GPIO->GPIOx->MODER &= ~(3<<((BSP_IIC_SDA_GPIO->Pin)*2));
    BSP_IIC_SDA_GPIO->GPIOx->MODER |= 0<<(BSP_IIC_SDA_GPIO->Pin)*2;
}

/*SDA_OUT_demo*/
void SDA_OUT_demo(BSP_GPIOSource_TypeDef* BSP_IIC_SDA_GPIO){
    BSP_IIC_SDA_GPIO->GPIOx->MODER &= ~(3<<((BSP_IIC_SDA_GPIO->Pin)*2));
    BSP_IIC_SDA_GPIO->GPIOx->MODER |= 1<<(BSP_IIC_SDA_GPIO->Pin)*2;
}

/*IIC_Delay_demo*/
void IIC_Delay_demoBase(uint16_t t){
	uint16_t i;
	uint8_t a;
	for( i=0;i<t;i++){
		for (a = 0; a < 6; a++);
	}
}
void IIC_Delay_demo(void){
	IIC_Delay_demoBase(1);
}
#endif // IIC_DEMO

/**********************************
 * 函数名：BSP_IIC_Start
 * 功能：产生IIC起始信号
 * 输入：iic总线对象
 * 输出：无
 * 备注：无
**********************************/
void BSP_IIC_start(iic_bus_TypeDef* iic_bus){
    // 无上拉电阻，改变SDA为输出
    if(!iic_bus->iic_pull_up_flag){
        iic_bus->SDA_OUT(iic_bus->BSP_IIC_SDA_GPIO);     //sda线输出
    }
	
	*(iic_bus->IIC_SDA) = 1;	  	  
	*(iic_bus->IIC_SCL) = 1;
    iic_bus->IIC_Delay();

	*(iic_bus->IIC_SDA) = 0;    //START:when CLK is high,DATA change form high to low
    iic_bus->IIC_Delay();
	*(iic_bus->IIC_SCL) = 0;    //钳住I2C总线，准备发送或接收数据
    // iic_bus->IIC_Delay();							   	
}	  

/**********************************
 * 函数名：BSP_IIC_Stop
 * 功能：产生IIC停止信号
 * 输入：iic总线对象
 * 输出：无
 * 备注：无
**********************************/
void BSP_IIC_stop(iic_bus_TypeDef* iic_bus){
    // 无上拉电阻，改变SDA为输出
    if(!iic_bus->iic_pull_up_flag){
        iic_bus->SDA_OUT(iic_bus->BSP_IIC_SDA_GPIO);     //sda线输出
    }

	*(iic_bus->IIC_SCL) = 0;
	*(iic_bus->IIC_SDA) = 0;	//STOP:when CLK is high DATA change form low to high  	  
    iic_bus->IIC_Delay();

	*(iic_bus->IIC_SCL) = 1;
    iic_bus->IIC_Delay();
	*(iic_bus->IIC_SDA) = 1;    //发送I2C总线结束信号
    // iic_bus->IIC_Delay();							   	
}

/**********************************
 * 函数名：BSP_IIC_Wait_Ack
 * 功能：等待应答信号到来
 * 输入：iic总线对象
 * 输出：1，接收应答失败
		0，接收应答成功
 * 备注：无
**********************************/
uint8_t BSP_IIC_wait_ack(iic_bus_TypeDef* iic_bus){
    uint16_t ucErrTime=0;   // 重新等待有效应答尝试次数

	*(iic_bus->IIC_SDA) = 1;    // 主机释放SDA线	  	  
    if(!iic_bus->iic_pull_up_flag){
    	iic_bus->SDA_IN(iic_bus->BSP_IIC_SDA_GPIO);      //SDA设置为输入  
    }
    iic_bus->IIC_Delay();
	*(iic_bus->IIC_SCL) = 1;	//等待从机返回ack  	  
    iic_bus->IIC_Delay();
    	 
    //SCL为高电平期间，读取SDA的应答数据
	while(*(iic_bus->READ_SDA)){
		ucErrTime++;
		if(ucErrTime>1000){
			BSP_IIC_stop(iic_bus);
			return 1;
		}
	}
	*(iic_bus->IIC_SCL) = 0;    //检查应答结束，SCL拉低，为后续继续发送数据做准备
    // iic_bus->IIC_Delay();
    return 0;

}

/**********************************
 * 函数名：BSP_IIC_Ack
 * 功能：产生ACK应答
 * 输入：iic总线对象
 * 输出：无
 * 备注：无
**********************************/
void BSP_IIC_ack(iic_bus_TypeDef* iic_bus){
    *(iic_bus->IIC_SCL) = 0;
    if(!iic_bus->iic_pull_up_flag){
        iic_bus->SDA_OUT(iic_bus->BSP_IIC_SDA_GPIO);     //sda线输出  
    }
    *(iic_bus->IIC_SDA) = 0;
    iic_bus->IIC_Delay();

    *(iic_bus->IIC_SCL) = 1;
    iic_bus->IIC_Delay();
    *(iic_bus->IIC_SCL) = 0;
    iic_bus->IIC_Delay();
}

/**********************************
 * 函数名：BSP_IIC_Nack
 * 功能：不产生ACK应答
 * 输入：iic总线对象
 * 输出：无
 * 备注：无
**********************************/
void BSP_IIC_Nack(iic_bus_TypeDef* iic_bus){
    *(iic_bus->IIC_SCL) = 0;
    if(!iic_bus->iic_pull_up_flag){
        iic_bus->SDA_OUT(iic_bus->BSP_IIC_SDA_GPIO);     //sda线输出
    }
    *(iic_bus->IIC_SDA) = 1;
    iic_bus->IIC_Delay();

    *(iic_bus->IIC_SCL) = 1;
    iic_bus->IIC_Delay();
    *(iic_bus->IIC_SCL) = 0;
    iic_bus->IIC_Delay();
}

/**********************************
 * 函数名：BSP_IIC_send_byte
 * 功能：IIC发送一个字节
 * 输入：iic_bus：iic总线对象
 *      sendData：发送的数据
 * 输出：无
 * 备注：无
**********************************/
void BSP_IIC_send_byte(iic_bus_TypeDef* iic_bus, uint8_t sendData){
    uint8_t cnt;
    if(!iic_bus->iic_pull_up_flag){
        iic_bus->SDA_OUT(iic_bus->BSP_IIC_SDA_GPIO);     //sda线输出
    }

    *(iic_bus->IIC_SCL) = 0;    //拉低时钟开始数据传输

    for(cnt=0; cnt<8; cnt++){              
		*(iic_bus->IIC_SDA) = (sendData & 0x80) >> 7;	//取出最高位
		sendData <<= 1;					//左移一位，让次高位移到最高位，方便下次取
        iic_bus->IIC_Delay();
		*(iic_bus->IIC_SCL) = 1;
        iic_bus->IIC_Delay();
        *(iic_bus->IIC_SCL) = 0;
        iic_bus->IIC_Delay();
	}

    *(iic_bus->IIC_SDA) = 1;	//发送结束，释放SDA总线
}

/**********************************
 * 函数名：BSP_IIC_read_byte
 * 功能：读1个字节
 * 输入：iic_bus：iic总线对象
 *       ack=1时，发送ACK，ack=0，发送nACK
 * 输出：接收到的数据
 * 备注：无
**********************************/
uint8_t BSP_IIC_read_byte(iic_bus_TypeDef* iic_bus, uint8_t ack){
	uint8_t i, receive=0;

    if(!iic_bus->iic_pull_up_flag){
        iic_bus->SDA_IN(iic_bus->BSP_IIC_SDA_GPIO);     // SDA设置为输入
    }

    for(i=0;i<8;i++ ){
		*(iic_bus->IIC_SCL) = 0; 
        iic_bus->IIC_Delay();
		// *(iic_bus->IIC_SCL) = 1; 
		receive<<=1;
		if(*(iic_bus->READ_SDA))	receive++;   
		iic_bus->IIC_Delay();
	}	
	if (ack)
		BSP_IIC_ack(iic_bus); //发送ACK 
	else
		BSP_IIC_Nack(iic_bus);//发送nACK 
	return receive;
}

/**********************************
 * 函数名：BSP_IIC_checkDevice
 * 功能：检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
 * 输入：iic_bus：iic总线对象
 *      Device_Address：设备的IIC总线地址
 * 输出：0：表示正确
		1：表示未探测到
 * 备注：无
**********************************/
uint8_t BSP_IIC_CheckDevice(iic_bus_TypeDef* iic_bus, uint8_t Device_Address){
	uint8_t ucAck;

    // // 等待总线空闲
    // while(!(READ_SDA && READ_SCL));

	if (*(iic_bus->READ_SCL) && *(iic_bus->READ_SDA)){
		BSP_IIC_start(iic_bus);		/* 发送启动信号 */

		/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
		BSP_IIC_send_byte(iic_bus, Device_Address | IIC_WR);
		ucAck = BSP_IIC_wait_ack(iic_bus);		/* 检测设备的ACK应答 */

		BSP_IIC_stop(iic_bus);		/* 发送停止信号 */

		return ucAck;
	}
	return 1;	/* I2C总线异常 */
}

/**********************************
 * 函数名：BSP_IIC_WriteData
 * 功能：软件IIC写数据
 * 输入：iic_bus: iic总线对象
        dev_addr：器件地址
		regAddr：需写的寄存器地址
		data：需写的数据
 * 输出：0：发送成功
        1：发送失败
 * 备注：无
**********************************/
uint8_t BSP_IIC_writeData(iic_bus_TypeDef* iic_bus, uint8_t dev_addr,uint8_t regAddr,uint8_t data){
	BSP_IIC_start(iic_bus);
	/*************发送器件地址***************/
	BSP_IIC_send_byte(iic_bus, dev_addr);
	if(BSP_IIC_wait_ack(iic_bus)){	//等待应答
		BSP_IIC_stop(iic_bus);		 
		return 1;
	}
	/*************发送寄存器地址***************/
	BSP_IIC_send_byte(iic_bus, regAddr);
	if(BSP_IIC_wait_ack(iic_bus)){	//等待应答
		BSP_IIC_stop(iic_bus);		 
		return 1;
	}
	/*************发送数据***************/
	BSP_IIC_send_byte(iic_bus, data);
	if(BSP_IIC_wait_ack(iic_bus)){	//等待应答
		BSP_IIC_stop(iic_bus);		 
		return 1;
	}
	BSP_IIC_stop(iic_bus);	
	return 0;
}

/**********************************
 * 函数名：BSP_IIC_readData
 * 功能：软件IIC读数据
 * 输入：iic_bus：iic总线对象
        dev_addr：器件地址
        regAddr：需读的寄存器地址
		buf：存放读出数据的指针变量
		length：需读数据的长度
 * 输出：0：接收成功
        1：接收失败
 * 备注：无
**********************************/
uint8_t BSP_IIC_readData(iic_bus_TypeDef* iic_bus, uint8_t dev_addr,uint8_t regAddr,uint8_t *buf,uint8_t length){
	BSP_IIC_start(iic_bus);
	/*************发送器件地址***************/
	BSP_IIC_send_byte(iic_bus, dev_addr);
	if(BSP_IIC_wait_ack(iic_bus)){	//等待应答
		BSP_IIC_stop(iic_bus);
		return 1;
	}
	/*************发送寄存器地址***************/
	BSP_IIC_send_byte(iic_bus, regAddr);
	if(BSP_IIC_wait_ack(iic_bus)){	//等待应答
		BSP_IIC_stop(iic_bus);	
		return 1;
	}
	/*************发送读数据命令***************/
	BSP_IIC_start(iic_bus);
	BSP_IIC_send_byte(iic_bus, dev_addr+1);
	if(BSP_IIC_wait_ack(iic_bus)){	//等待应答
		BSP_IIC_stop(iic_bus);	
//		printf("hehe3\r\n");
		return 1;
	}
	while(length){
		if(length == 1)
			*buf = BSP_IIC_read_byte(iic_bus, 0);	//读数据,发送nACK 
		else 
			*buf = BSP_IIC_read_byte(iic_bus, 1);	//读数据,发送ACK  
		length--;
		buf++;
	}
	
	BSP_IIC_stop(iic_bus);	
	return 0;
}
