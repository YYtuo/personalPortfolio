/***************************************************
 * mode name:BSP_IIC
 * brief:iic�ײ�������
 * @author tuo
 * @created 2022/12/29 21:31:13
 * @imfor H:\library\STM32\test\F4\core_boardV1.2\bootloader\bootloader_V1.2\USER_CODE\BSP\BSP_IIC.h
***************************************************/

#ifndef __BSP_IIC_H
#define __BSP_IIC_H

#include "stm32f4xx.h"
#include "stdlib.h"
#include "stdio.h"

#include "BSP_GPIO.h"

#define IIC_DEMO	// �û�����ʾ������Ҫ�Զ��壬��ע�͸ú�

#define IIC_WR	0		/* д����bit */
#define IIC_RD	1		/* ������bit */

// iic�����࣬��Ҫ�û����г�ʼ��
typedef struct iic_bus{
	// �Զ����־λ�Ĵ���
	uint8_t iic_pull_up_flag:1;	// iic�������Ƿ���������1:������SDA��ʼ��Ϊ��©��������Ա������������Ƶ���ı�SDA���ݷ���
													//0:���������ʼ��Ϊ���������
	
	uint8_t BUS_ID:2;	// IIC����ID,0��1��2��3��IIC���ߣ�������࣬����������
	uint8_t :0;

	// IIC��������λ��
	BSP_GPIOSource_TypeDef* BSP_IIC_SCL_GPIO;
	BSP_GPIOSource_TypeDef* BSP_IIC_SDA_GPIO;


	volatile unsigned long* IIC_SCL;	// iicʱ����IO
	volatile unsigned long* IIC_SDA;	// iic������IO
	volatile unsigned long*	READ_SCL;
	volatile unsigned long*	READ_SDA;

	// �û��Զ��庯��
	// IIC���ų�ʼ������
	void(*IIC_GPIO_Init)\
		(struct iic_bus* iic_bus);

	// SDA�������ú���
	void(*SDA_IN)(BSP_GPIOSource_TypeDef* BSP_IIC_SDA_GPIO);
	void(*SDA_OUT)(BSP_GPIOSource_TypeDef* BSP_IIC_SDA_GPIO);	
	// IIC��ʱ����
	void(*IIC_Delay)(void);	
}iic_bus_TypeDef;

// iic�豸���� ���� ��ʼ������
void BSP_iic_bus_parameterInit(iic_bus_TypeDef* iic_bus,
                                    uint8_t iic_pull_up_flag,
                                    uint8_t BUS_ID,
                                    
                                    BSP_GPIOSource_TypeDef* BSP_IIC_SCL_GPIO,
	                                BSP_GPIOSource_TypeDef* BSP_IIC_SDA_GPIO,

                                    volatile unsigned long* IIC_SCL,
	                                volatile unsigned long* IIC_SDA,
                                    volatile unsigned long* READ_SCL,
	                                volatile unsigned long* READ_SDA,
                                    
                                    void(*IIC_GPIO_Init)\
									(iic_bus_TypeDef* iic_bus),
                                    void(*SDA_IN)\
		                            (BSP_GPIOSource_TypeDef* BSP_IIC_SDA_GPIO),
	                                void(*SDA_OUT)\
		                            (BSP_GPIOSource_TypeDef* BSP_IIC_SDA_GPIO),
                                    void (*IIC_Delay)(void));

// �û�����demo�������Զ��壬��ע��IIC_DEMO��
#ifdef IIC_DEMO
void IIC_GPIO_Init_demo(iic_bus_TypeDef* iic_bus);
void SDA_IN_demo(BSP_GPIOSource_TypeDef* BSP_IIC_SDA_GPIO);
void SDA_OUT_demo(BSP_GPIOSource_TypeDef* BSP_IIC_SDA_GPIO);
void IIC_Delay_demoBase(uint16_t t);
void IIC_Delay_demo(void);
#endif

// ����IIC��ʼ�ź�
void BSP_IIC_start(iic_bus_TypeDef* iic_bus);
// ����IICֹͣ�ź�
void BSP_IIC_stop(iic_bus_TypeDef* iic_bus);
// �ȴ�Ӧ��
uint8_t BSP_IIC_wait_ack(iic_bus_TypeDef* iic_bus);
// ����Ӧ��
void BSP_IIC_ack(iic_bus_TypeDef* iic_bus);
// ������Ӧ��
void BSP_IIC_Nack(iic_bus_TypeDef* iic_bus);
// IIC����һ���ֽ�
void BSP_IIC_send_byte(iic_bus_TypeDef* iic_bus, uint8_t sendData);
// IIC��ȡһ���ֽ�
uint8_t BSP_IIC_read_byte(iic_bus_TypeDef* iic_bus, uint8_t ack);
// ����豸
uint8_t BSP_IIC_CheckDevice(iic_bus_TypeDef* iic_bus, uint8_t Device_Address);

uint8_t BSP_IIC_writeData(iic_bus_TypeDef* iic_bus, uint8_t dev_addr,uint8_t regAddr,uint8_t data);
uint8_t BSP_IIC_readData(iic_bus_TypeDef* iic_bus, uint8_t dev_addr,uint8_t regAddr,uint8_t *buf,uint8_t length);
#endif
