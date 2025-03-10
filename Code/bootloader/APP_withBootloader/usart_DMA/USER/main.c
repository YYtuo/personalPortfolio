#include "init.h"


uint8_t adc_test = 0;


int main(void){
	board_init();
	Adc_Init();
	while(1){
	
		if(reciver_size){
			BSP_UART4_DMA_SendData(uart4_recBuff,reciver_size);
			reciver_size = 0;
		}
		
		
		adc_test = Get_Adc(10);
		oled_ShowNum(0,0,adc_test,5,big);
		delay_ms(100);
		
	}

}



