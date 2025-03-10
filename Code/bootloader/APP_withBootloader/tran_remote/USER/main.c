#include "init.h"


int main(void){
	board_init();
	
	while(1){
		#ifdef REMOTE
		USBH_Process(&USB_OTG_Core_dev, &USB_Host);
		#endif
		
		
	}

}



