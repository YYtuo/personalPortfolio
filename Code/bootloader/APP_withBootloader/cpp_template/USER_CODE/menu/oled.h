#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#ifndef __OLED_H__
#define	__OLED_H__

#include "BSP_IIC.h"
#include "delay.h"

#define OLED_ADDR 0x78

/*************** OLED����  oled_WriteDat����ڲ��� *****************************/
/*
		һ��ʮ������������8���е�

	1	. ��������������
	0	����������������
	1	. ��������������
	0	����������������
	0	����������������   ==>   1100 0101  ==>  oled_WriteDat(0xC5)
	0	����������������
	1	. ��������������
	1	. ��������������
	
		�ߡ�������ʼ��ַ������
	
	��λ�� �ɵ���Χ0x10 ~ 0x17  ==>  ���õ�ǰ��ʾ�е��������꣬0x10��ʾ �� ��1�� ~ ��16�� ��������ʾ��0x11��ʾ �� ��17�� ~ ��32�� ��������ʾ���Դ�����
	��λ�� �ɵ���Χ0x00 ~ 0x0f  ==>  ���ÿ�ʼ��ʾ�������꣬0x00��ʾ������ ���趨������ �ĵ�1�п�ʼ��ʾ��0x01��ʾ�ӵ�2����ʾ���Դ�����
	
								  0x10								0x11
	��������������������������������|��������������������������������|
	��������������������������������|��������������������������������|
	��������������������������������|��������������������������������|
	��������������������������������|��������������������������������|  ...
	��������������������������������|��������������������������������|
	��������������������������������|��������������������������������|
	��������������������������������|��������������������������������|
	��������������������������������|��������������������������������|
	  ^
	  |
    0x01
		
		oled_WriteCmd(0x00);		//������ʼ��ַ
		oled_WriteCmd(0x10);		//������ʼ��ַ
		���Ǵӵ�һ��������ĵ�һ�п�ʼ��ʾ
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
