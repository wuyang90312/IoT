#include <stdlib.h>
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
	struct ip_info ipConfig;
	configInfo* configuration = (configInfo*)os_zalloc(sizeof(configInfo));
	wifi_get_ip_info(SOFTAP_IF, &ipConfig);
	
	/* Read configuration information from permanent memory */
	spi_flash_read((0x7d)*4096+4, (uint32 *) &configuration, sizeof(configInfo));
	INFO("\nThe configuration is: %s\t%s\t%s\t%s\n", 
	configuration->SSID, configuration->PWD, configuration->API,configuration->TIME);
	time = atoi(configuration->TIME);
	INFO("\nThe configuration is: %s\t%s\t%s\t%d\n", 
	configuration->SSID, configuration->PWD, configuration->API,time);

	INFO("1\n");
	os_sprintf(str, "/update?api_key=%s&field1=40", configuration->API);
	INFO("3\n");
	time *= 1000;
	hst = "api.thingspeak.com";
	len = os_strlen(hst);

	INFO("4\n");
	WIFI_Connect(configuration->SSID, configuration->PWD, NULL);
	INFO("5\n");
	rst = REST_Setup(hst, len, 80,0x00000000);

	os_timer_disarm(&timer);
	os_timer_setfn(&timer, (os_timer_func_t *)timer_cb, rst);
	os_timer_arm(&timer, time, 1);
}

void ICACHE_FLASH_ATTR
user_init(void)
{
	uint8_t buff=0;
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	os_delay_us(1000000);

	//spi_flash_erase_sector(0x7d);
	spi_flash_read((0x7d)*4096, (uint32 *) &buff, 1);
	INFO("\nThe memeory is: %02x\n", buff);
	if(buff == 101)
	{
		user_continue();
	}
	else 
	{
		WIFI_AP_Config((uint8_t*)"AI-THINKER-SERV", (uint8_t*) "23456789");
		ESP_SetupIpServer(); // prepare for the client UI
	}

}

