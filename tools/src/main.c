#include <stdio.h>
#include <stdlib.h>
#include "easy-usb-daq-card_dll.h"

int main()
{
	float adResult;
	float m_ad[12];
	int ret;

	printf("\n"
		"-------------------------------------\n"
		"--------USB Card test program--------\n"
		"-------------------------------------\n\n");

	ret =  OpenUsb();
	if (ret) {
		printf("open usb device success!\n");
	}
	else {
		printf("open usb device fail!\n");
		// exit(1);
	}




	AD_single(0,&adResult);
	m_ad[0] =adResult;
	AD_single(1,&adResult);
	m_ad[1] =adResult;
	AD_single(2,&adResult);
	m_ad[2] =adResult;
	AD_single(3,&adResult);
	m_ad[3] =adResult;

	AD_single(4,&adResult);
	m_ad[4] =adResult;
	AD_single(5,&adResult);
	m_ad[5] =adResult;
	AD_single(6,&adResult);
	m_ad[6] =adResult;
	AD_single(7,&adResult);
	m_ad[7] =adResult;

	AD_single(8,&adResult);
	m_ad[8] =adResult;
	AD_single(9,&adResult);
	m_ad[9] =adResult;
	AD_single(10,&adResult);
	m_ad[10] =adResult;
	AD_single(11,&adResult);
	m_ad[11] =adResult;

	for (int i = 0; i < 12; i++) {
		printf("ch%d \t %8.2f\n", i, m_ad[i]);
	}
	ret = CloseUsb();
	if (ret) {
		printf("clsoe usb device success!\n");
	}
	else {
		printf("close usb device fail!\n");
		exit(1);
	}
	return 0;
}

