#include "util.h"
#include "radio.h"
#include "hardware/radio.h"
#include "uart.h"
#include "gpio.h"
#include "timer.h"
#include "ultrasn0w-unlockstrings.h"

int ultrasn0w()
{
	bufferPrintf("Summoning the power of Ultrasn0w.\r\n");

	radio_write("AT+XGENDATA\r\n");

	char* buf = malloc(0x1000);
	int c = radio_read(buf, 0x1000);

	if (!c) {
		free(buf);
		bufferPrintf("No response. Exiting!\r\n");
		return -1;
	}

	if(strstr(buf, "ICE2_MODEM_05.13.04") != NULL)
		radio_cmd(bb051304, 1);
	else if(strstr(buf, "ICE2_MODEM_05.12.01") != NULL)
		radio_cmd(bb051201, 1);
	else if(strstr(buf, "ICE2_MODEM_05.11.07") != NULL)
		radio_cmd(bb051107, 1);
	else if(strstr(buf, "ICE2_MODEM_04.26.08") != NULL)
		radio_cmd(bb042608, 1);
	else {
		free(buf);
		bufferPrintf("No suitable baseband found. Exiting!\r\n");
		return -1;
	}

	free(buf);

	bufferPrintf("Exploit finished. Sending unlock keys.\r\n");

	radio_cmd("at+xlck=0\r\n", 1);
	radio_cmd("at+xlck=1,1,\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\"\r\n", 1);
	radio_cmd("at+xlck=1,2,\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\"\r\n", 1);
	radio_cmd("at+xlck=1,3,\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\"\r\n", 1);
	radio_cmd("at+xlck=1,4,\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\"\r\n", 1);
	radio_cmd("at+xlck=2\r\n", 1);

	radio_write("AT\r\n");
	radio_write("AT+CPIN?\r\n");

	buf = malloc(0x1000);
	c = radio_read(buf, 0x1000);

	if (!c) {
		free(buf);
		bufferPrintf("No response. Exiting!\r\n");
		return -1;
	}

	if((strstr(buf, "PH-NET") == NULL) && (strstr(buf, "ERROR") == NULL)) {
		free(buf);
		bufferPrintf("Unlocked!\r\n");
		return 0;
	}

	free(buf);

	bufferPrintf("Unlock failed!\r\n");

	return -1;	
}
