#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#ifndef __OLED_H__
#define	__OLED_H__

#include "BSP_IIC.h"
#include "delay.h"

#define OLED_ADDR 0x78

/*************** OLED描点表  oled_WriteDat的入口参数 *****************************/
/*
		一个十六进制数控制8个列点

	1	. 。。。。。。。
	0	。。。。。。。。
	1	. 。。。。。。。
	0	。。。。。。。。
	0	。。。。。。。。   ==>   1100 0101  ==>  oled_WriteDat(0xC5)
	0	。。。。。。。。
	1	. 。。。。。。。
	1	. 。。。。。。。
	
		高、低列起始地址的作用
	
	高位： 可调范围0x10 ~ 0x17  ==>  设置当前显示列的区域坐标，0x10表示 在 第1列 ~ 第16列 区域内显示，0x11表示 在 第17列 ~ 第32列 区域内显示，以此类推
	低位： 可调范围0x00 ~ 0x0f  ==>  设置开始显示的列坐标，0x00表示从上面 “设定的区域” 的第1列开始显示，0x01表示从第2列显示，以此类推
	
								  0x10								0x11
	。。。。。。。。。。。。。。。。|。。。。。。。。。。。。。。。。|
	。。。。。。。。。。。。。。。。|。。。。。。。。。。。。。。。。|
	。。。。。。。。。。。。。。。。|。。。。。。。。。。。。。。。。|
	。。。。。。。。。。。。。。。。|。。。。。。。。。。。。。。。。|  ...
	。。。。。。。。。。。。。。。。|。。。。。。。。。。。。。。。。|
	。。。。。。。。。。。。。。。。|。。。。。。。。。。。。。。。。|
	。。。。。。。。。。。。。。。。|。。。。。。。。。。。。。。。。|
	。。。。。。。。。。。。。。。。|。。。。。。。。。。。。。。。。|
	  ^
	  |
    0x01
		
		oled_WriteCmd(0x00);		//低列起始地址
		oled_WriteCmd(0x10);		//高列起始地址
		就是从第一个大区域的第一列开始显示
*/

enum{
	small = 1,
	big
};

extern const unsigned char tuo[];

void oledPlayer_Init(void);
void oled_SetPos(unsigned char x, unsigned char y);
void drowPiont_8x8(unsigned char x, unsigned char y);
void cleanPiont_8x8(unsigned char x, unsigned char y);


void oled_Fill(unsigned char fill_Data);
void oled_Cls(void);
void oled_On(void);
void oled_Off(void);
void oled_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void oled_ShowNum(unsigned char x, unsigned char y, int32_t num,uint8_t size,uint8_t TextSize);
void oled_ShowFloat(unsigned char x, unsigned char y, float fl,uint8_t size_Int,uint8_t size_Float,uint8_t TextSize);
void oled_ShowCh(unsigned char x, unsigned char y, unsigned char N);
void oled_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void oled_DrawGIF(uint8_t speed);
void oledShow(void);
#endif 

#ifdef __cplusplus
}
#endif /* __cplusplus */
