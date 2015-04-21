#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "rest.h"
#include "wifi.h"
#include "driver/uart.h"
#include "debug.h"
#include "esp_server.h"
#include "esp_string.h"

static ETSTimer timer;
char* str;

void ICACHE_FLASH_ATTR
timer_cb(void *arg)
{
	uint32_t rst = (uint32_t) arg;
	REST_Request(rst, "GET", str);
}

void ICACHE_FLASH_ATTR
user_continue(void)
{
	uint16_t len;
	char* hst;
	uint32_t rst;
	uint16_t time = 30000;

	str = "/update?api_key=GPVP0E6QQVWU47LZ&field1=40";
	hst = "api.thingspeak.com";
	len = os_strlen(hst);

	WIFI_Connect("BCRLovs", "23456", NULL);
	
	rst = REST_Setup(hst, len, 80,0x00000000);

	os_timer_disarm(&timer);
	os_timer_setfn(&timer, (os_timer_func_t *)timer_cb, rst);
	os_timer_arm(&timer, time, 1);

}

void ICACHE_FLASH_ATTR
user_init(void)
{
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	os_delay_us(1000000);

	fcn_cb_ptr = &user_continue; // Store the function ptr into global 
	WIFI_AP_Config((uint8_t*)"AI-THINKER-SERV", (uint8_t*) "23456789");
	ESP_SetupIpServer(); // prepare for the client UI
	//fcn_cb_ptr();
}



