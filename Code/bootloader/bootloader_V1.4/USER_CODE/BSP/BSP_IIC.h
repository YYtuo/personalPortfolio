/***************************************************
 * mode name:BSP_IIC
 * brief:iic底层驱动，
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

#define IIC_DEMO	// 用户函数示例，若要自定义，需注释该宏

#define IIC_WR	0		/* 写控制bit */
#define IIC_RD	1		/* 读控制bit */

// iic总线类，需要用户进行初始化
typedef struct iic_bus{
	// 自定义标志位寄存器
	uint8_t iic_pull_up_flag:1;	// iic总线上是否有上拉，1:上拉则SDA初始化为开漏输出（可以避免推挽输出的频繁改变SDA数据方向）
													//0:无上拉则初始化为推挽输出，
	
	uint8_t BUS_ID:2;	// IIC总线ID,0，1，2，3号IIC总线，如需更多，请自行扩充
	uint8_t :0;

	// IIC总线引脚位号
	BSP_GPIOSource_TypeDef* BSP_IIC_SCL_GPIO;
	BSP_GPIOSource_TypeDef* BSP_IIC_SDA_GPIO;


	volatile unsigned long* IIC_SCL;	// iic时钟线IO
	volatile unsigned long* IIC_SDA;	// iic数据线IO
	volatile unsigned long*	READ_SCL;
	volatile unsigned long*	READ_SDA;

	// 用户自定义函数
	// IIC引脚初始化函数
	void(*IIC_GPIO_Init)\
		(struct iic_bus* iic_bus);

	// SDA方向设置函数
	void(*SDA_IN)(BSP_GPIOSource_TypeDef* BSP_IIC_SDA_GPIO);
	void(*SDA_OUT)(BSP_GPIOSource_TypeDef* BSP_IIC_SDA_GPIO);	
	// IIC延时函数
	void(*IIC_Delay)(void);	
}iic_bus_TypeDef;

// iic设备对象 参数 初始化函数
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

// 用户函数demo，若需自定义，请注释IIC_DEMO宏
#ifdef IIC_DEMO
void IIC_GPIO_Init_demo(iic_bus_TypeDef* iic_bus);
void SDA_IN_demo(BSP_GPIOSource_TypeDef* BSP_IIC_SDA_GPIO);
void SDA_OUT_demo(BSP_GPIOSource_TypeDef* BSP_IIC_SDA_GPIO);
void IIC_Delay_demoBase(uint16_t t);
void IIC_Delay_demo(void);
#endif

// 发送IIC开始信号
void BSP_IIC_start(iic_bus_TypeDef* iic_bus);
// 发送IIC停止信号
void BSP_IIC_stop(iic_bus_TypeDef* iic_bus);
// 等待应答
uint8_t BSP_IIC_wait_ack(iic_bus_TypeDef* iic_bus);
// 产生应答
void BSP_IIC_ack(iic_bus_TypeDef* iic_bus);
// 不产生应答
void BSP_IIC_Nack(iic_bus_TypeDef* iic_bus);
// IIC发送一个字节
void BSP_IIC_send_byte(iic_bus_TypeDef* iic_bus, uint8_t sendData);
// IIC读取一个字节
uint8_t BSP_IIC_read_byte(iic_bus_TypeDef* iic_bus, uint8_t ack);
// 检查设备
uint8_t BSP_IIC_CheckDevice(iic_bus_TypeDef* iic_bus, uint8_t Device_Address);

uint8_t BSP_IIC_writeData(iic_bus_TypeDef* iic_bus, uint8_t dev_addr,uint8_t regAddr,uint8_t data);
uint8_t BSP_IIC_readData(iic_bus_TypeDef* iic_bus, uint8_t dev_addr,uint8_t regAddr,uint8_t *buf,uint8_t length);
#endif
