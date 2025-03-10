/***************************************************
 * mode name:BSP_IIC
 * brief:iic�ײ�������
 * @author tuo
 * @created 2022/12/29 21:31:13
 * @imfor H:\library\STM32\test\F4\core_boardV1.2\bootloader\bootloader_V1.2\USER_CODE\BSP\BSP_IIC.c
***************************************************/
#include "BSP_IIC.h"

/**********************************
 * ��������BSP_iic_bus_parameterInit
 * ���ܣ�iic���߶��� ���� ��ʼ������
 * ���룺
 * �������
 * ��ע����
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
    // ���������־λ��ʼ��
    iic_bus->iic_pull_up_flag = para_iic_pull_up_flag;
    // ��ַ��ʼ��
    iic_bus->BUS_ID = para_BUS_ID;
	// IIC��������λ�ų�ʼ�� 
    iic_bus->BSP_IIC_SCL_GPIO = para_BSP_IIC_SCL_GPIO;
    iic_bus->BSP_IIC_SDA_GPIO = para_BSP_IIC_SDA_GPIO;

    // ���߳�ʼ��
    iic_bus->IIC_SCL = para_IIC_SCL;
    iic_bus->IIC_SDA = para_IIC_SDA;
    iic_bus->READ_SCL = para_READ_SCL;
    iic_bus->READ_SDA = para_READ_SDA;

    // GPIO��ʼ��
    iic_bus->IIC_GPIO_Init = para_IIC_GPIO_Init;
    iic_bus->IIC_GPIO_Init(iic_bus);
	// SDA�������ú�����ʼ��
    iic_bus->SDA_IN = para_SDA_IN;
    iic_bus->SDA_OUT = para_SDA_OUT;
    // ��ʱ��ʼ��
    iic_bus->IIC_Delay = para_IIC_Delay;

}

#ifdef IIC_DEMO
/*IIC_GPIO_Init_demo*/
void IIC_GPIO_Init_demo(iic_bus_TypeDef* iic_bus){
    
    // �������������裬��ʼ��Ϊ��©��������������л�SDA���� 
    if(iic_bus->iic_pull_up_flag){
    	BSP_GPIO_Init(iic_bus->BSP_IIC_SDA_GPIO, GPIO_Mode_Out_OD);
    }

    // �������������ʼ��Ϊ����
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
 * ��������BSP_IIC_Start
 * ���ܣ�����IIC��ʼ�ź�
 * ���룺iic���߶���
 * �������
 * ��ע����
**********************************/
void BSP_IIC_start(iic_bus_TypeDef* iic_bus){
    // ���������裬�ı�SDAΪ���
    if(!iic_bus->iic_pull_up_flag){
        iic_bus->SDA_OUT(iic_bus->BSP_IIC_SDA_GPIO);     //sda�����
    }
	
	*(iic_bus->IIC_SDA) = 1;	  	  
	*(iic_bus->IIC_SCL) = 1;
    iic_bus->IIC_Delay();

	*(iic_bus->IIC_SDA) = 0;    //START:when CLK is high,DATA change form high to low
    iic_bus->IIC_Delay();
	*(iic_bus->IIC_SCL) = 0;    //ǯסI2C���ߣ�׼�����ͻ��������
    // iic_bus->IIC_Delay();							   	
}	  

/**********************************
 * ��������BSP_IIC_Stop
 * ���ܣ�����IICֹͣ�ź�
 * ���룺iic���߶���
 * �������
 * ��ע����
**********************************/
void BSP_IIC_stop(iic_bus_TypeDef* iic_bus){
    // ���������裬�ı�SDAΪ���
    if(!iic_bus->iic_pull_up_flag){
        iic_bus->SDA_OUT(iic_bus->BSP_IIC_SDA_GPIO);     //sda�����
    }

	*(iic_bus->IIC_SCL) = 0;
	*(iic_bus->IIC_SDA) = 0;	//STOP:when CLK is high DATA change form low to high  	  
    iic_bus->IIC_Delay();

	*(iic_bus->IIC_SCL) = 1;
    iic_bus->IIC_Delay();
	*(iic_bus->IIC_SDA) = 1;    //����I2C���߽����ź�
    // iic_bus->IIC_Delay();							   	
}

/**********************************
 * ��������BSP_IIC_Wait_Ack
 * ���ܣ��ȴ�Ӧ���źŵ���
 * ���룺iic���߶���
 * �����1������Ӧ��ʧ��
		0������Ӧ��ɹ�
 * ��ע����
**********************************/
uint8_t BSP_IIC_wait_ack(iic_bus_TypeDef* iic_bus){
    uint16_t ucErrTime=0;   // ���µȴ���ЧӦ���Դ���

	*(iic_bus->IIC_SDA) = 1;    // �����ͷ�SDA��	  	  
    if(!iic_bus->iic_pull_up_flag){
    	iic_bus->SDA_IN(iic_bus->BSP_IIC_SDA_GPIO);      //SDA����Ϊ����  
    }
    iic_bus->IIC_Delay();
	*(iic_bus->IIC_SCL) = 1;	//�ȴ��ӻ�����ack  	  
    iic_bus->IIC_Delay();
    	 
    //SCLΪ�ߵ�ƽ�ڼ䣬��ȡSDA��Ӧ������
	while(*(iic_bus->READ_SDA)){
		ucErrTime++;
		if(ucErrTime>1000){
			BSP_IIC_stop(iic_bus);
			return 1;
		}
	}
	*(iic_bus->IIC_SCL) = 0;    //���Ӧ�������SCL���ͣ�Ϊ������������������׼��
    // iic_bus->IIC_Delay();
    return 0;

}

/**********************************
 * ��������BSP_IIC_Ack
 * ���ܣ�����ACKӦ��
 * ���룺iic���߶���
 * �������
 * ��ע����
**********************************/
void BSP_IIC_ack(iic_bus_TypeDef* iic_bus){
    *(iic_bus->IIC_SCL) = 0;
    if(!iic_bus->iic_pull_up_flag){
        iic_bus->SDA_OUT(iic_bus->BSP_IIC_SDA_GPIO);     //sda�����  
    }
    *(iic_bus->IIC_SDA) = 0;
    iic_bus->IIC_Delay();

    *(iic_bus->IIC_SCL) = 1;
    iic_bus->IIC_Delay();
    *(iic_bus->IIC_SCL) = 0;
    iic_bus->IIC_Delay();
}

/**********************************
 * ��������BSP_IIC_Nack
 * ���ܣ�������ACKӦ��
 * ���룺iic���߶���
 * �������
 * ��ע����
**********************************/
void BSP_IIC_Nack(iic_bus_TypeDef* iic_bus){
    *(iic_bus->IIC_SCL) = 0;
    if(!iic_bus->iic_pull_up_flag){
        iic_bus->SDA_OUT(iic_bus->BSP_IIC_SDA_GPIO);     //sda�����
    }
    *(iic_bus->IIC_SDA) = 1;
    iic_bus->IIC_Delay();

    *(iic_bus->IIC_SCL) = 1;
    iic_bus->IIC_Delay();
    *(iic_bus->IIC_SCL) = 0;
    iic_bus->IIC_Delay();
}

/**********************************
 * ��������BSP_IIC_send_byte
 * ���ܣ�IIC����һ���ֽ�
 * ���룺iic_bus��iic���߶���
 *      sendData�����͵�����
 * �������
 * ��ע����
**********************************/
void BSP_IIC_send_byte(iic_bus_TypeDef* iic_bus, uint8_t sendData){
    uint8_t cnt;
    if(!iic_bus->iic_pull_up_flag){
        iic_bus->SDA_OUT(iic_bus->BSP_IIC_SDA_GPIO);     //sda�����
    }

    *(iic_bus->IIC_SCL) = 0;    //����ʱ�ӿ�ʼ���ݴ���

    for(cnt=0; cnt<8; cnt++){              
		*(iic_bus->IIC_SDA) = (sendData & 0x80) >> 7;	//ȡ�����λ
		sendData <<= 1;					//����һλ���ôθ�λ�Ƶ����λ�������´�ȡ
        iic_bus->IIC_Delay();
		*(iic_bus->IIC_SCL) = 1;
        iic_bus->IIC_Delay();
        *(iic_bus->IIC_SCL) = 0;
        iic_bus->IIC_Delay();
	}

    *(iic_bus->IIC_SDA) = 1;	//���ͽ������ͷ�SDA����
}

/**********************************
 * ��������BSP_IIC_read_byte
 * ���ܣ���1���ֽ�
 * ���룺iic_bus��iic���߶���
 *       ack=1ʱ������ACK��ack=0������nACK
 * ��������յ�������
 * ��ע����
**********************************/
uint8_t BSP_IIC_read_byte(iic_bus_TypeDef* iic_bus, uint8_t ack){
	uint8_t i, receive=0;

    if(!iic_bus->iic_pull_up_flag){
        iic_bus->SDA_IN(iic_bus->BSP_IIC_SDA_GPIO);     // SDA����Ϊ����
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
		BSP_IIC_ack(iic_bus); //����ACK 
	else
		BSP_IIC_Nack(iic_bus);//����nACK 
	return receive;
}

/**********************************
 * ��������BSP_IIC_checkDevice
 * ���ܣ����I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
 * ���룺iic_bus��iic���߶���
 *      Device_Address���豸��IIC���ߵ�ַ
 * �����0����ʾ��ȷ
		1����ʾδ̽�⵽
 * ��ע����
**********************************/
uint8_t BSP_IIC_CheckDevice(iic_bus_TypeDef* iic_bus, uint8_t Device_Address){
	uint8_t ucAck;

    // // �ȴ����߿���
    // while(!(READ_SDA && READ_SCL));

	if (*(iic_bus->READ_SCL) && *(iic_bus->READ_SDA)){
		BSP_IIC_start(iic_bus);		/* ���������ź� */

		/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
		BSP_IIC_send_byte(iic_bus, Device_Address | IIC_WR);
		ucAck = BSP_IIC_wait_ack(iic_bus);		/* ����豸��ACKӦ�� */

		BSP_IIC_stop(iic_bus);		/* ����ֹͣ�ź� */

		return ucAck;
	}
	return 1;	/* I2C�����쳣 */
}

/**********************************
 * ��������BSP_IIC_WriteData
 * ���ܣ����IICд����
 * ���룺iic_bus: iic���߶���
        dev_addr��������ַ
		regAddr����д�ļĴ�����ַ
		data����д������
 * �����0�����ͳɹ�
        1������ʧ��
 * ��ע����
**********************************/
uint8_t BSP_IIC_writeData(iic_bus_TypeDef* iic_bus, uint8_t dev_addr,uint8_t regAddr,uint8_t data){
	BSP_IIC_start(iic_bus);
	/*************����������ַ***************/
	BSP_IIC_send_byte(iic_bus, dev_addr);
	if(BSP_IIC_wait_ack(iic_bus)){	//�ȴ�Ӧ��
		BSP_IIC_stop(iic_bus);		 
		return 1;
	}
	/*************���ͼĴ�����ַ***************/
	BSP_IIC_send_byte(iic_bus, regAddr);
	if(BSP_IIC_wait_ack(iic_bus)){	//�ȴ�Ӧ��
		BSP_IIC_stop(iic_bus);		 
		return 1;
	}
	/*************��������***************/
	BSP_IIC_send_byte(iic_bus, data);
	if(BSP_IIC_wait_ack(iic_bus)){	//�ȴ�Ӧ��
		BSP_IIC_stop(iic_bus);		 
		return 1;
	}
	BSP_IIC_stop(iic_bus);	
	return 0;
}

/**********************************
 * ��������BSP_IIC_readData
 * ���ܣ����IIC������
 * ���룺iic_bus��iic���߶���
        dev_addr��������ַ
        regAddr������ļĴ�����ַ
		buf����Ŷ������ݵ�ָ�����
		length��������ݵĳ���
 * �����0�����ճɹ�
        1������ʧ��
 * ��ע����
**********************************/
uint8_t BSP_IIC_readData(iic_bus_TypeDef* iic_bus, uint8_t dev_addr,uint8_t regAddr,uint8_t *buf,uint8_t length){
	BSP_IIC_start(iic_bus);
	/*************����������ַ***************/
	BSP_IIC_send_byte(iic_bus, dev_addr);
	if(BSP_IIC_wait_ack(iic_bus)){	//�ȴ�Ӧ��
		BSP_IIC_stop(iic_bus);
		return 1;
	}
	/*************���ͼĴ�����ַ***************/
	BSP_IIC_send_byte(iic_bus, regAddr);
	if(BSP_IIC_wait_ack(iic_bus)){	//�ȴ�Ӧ��
		BSP_IIC_stop(iic_bus);	
		return 1;
	}
	/*************���Ͷ���������***************/
	BSP_IIC_start(iic_bus);
	BSP_IIC_send_byte(iic_bus, dev_addr+1);
	if(BSP_IIC_wait_ack(iic_bus)){	//�ȴ�Ӧ��
		BSP_IIC_stop(iic_bus);	
//		printf("hehe3\r\n");
		return 1;
	}
	while(length){
		if(length == 1)
			*buf = BSP_IIC_read_byte(iic_bus, 0);	//������,����nACK 
		else 
			*buf = BSP_IIC_read_byte(iic_bus, 1);	//������,����ACK  
		length--;
		buf++;
	}
	
	BSP_IIC_stop(iic_bus);	
	return 0;
}
