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
char str[255];

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
	configInfo* configuration = (configInfo*)os_zalloc(sizeof(configInfo));
	
	/* Read configuration information from permanent memory */
	spi_flash_read((0x7d)*4096+4, (uint32 *) configuration, sizeof(configInfo));
	time = (uint16_t)atoi(configuration->TIME);
	time *=1000;
	INFO("\nThe configuration is:||%s||%s||%s||%u||\n", 
	configuration->SSID, configuration->PWD, configuration->API,time);

	//str = "/update?api_key=GPVP0E6QQVWU47LZ&field1=40";
	os_sprintf(str, "/update?api_key=%s&field1=40", &configuration->API);
	hst = "api.thingspeak.com";
	len = os_strlen(hst);

	WIFI_Connect((uint8_t*)configuration->SSID, (uint8_t*)configuration->PWD, NULL);
	rst = REST_Setup(hst, len, 80,0x00000000);

	os_timer_disarm(&timer);
	os_timer_setfn(&timer, (os_timer_func_t *)timer_cb, rst);
	os_timer_arm(&timer, time, 1);
}

void ICACHE_FLASH_ATTR
user_init(void)
{
	uint32_t buff=0xffffffff;
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	os_delay_us(1000000);

	//spi_flash_erase_sector(0x7d);
	//spi_flash_write((0x7d)*4096, (uint32 *) &buff, 4);
	spi_flash_read((0x7d)*4096, (uint32 *) &buff, 4);
	INFO("\nThe memeory is: %x\n", buff);
	if(buff == 0x10101010)
	{
		user_continue();
	}
	else 
	{
		WIFI_AP_Config((uint8_t*)"AI-THINKER-SERV", (uint8_t*) "23456789");
		ESP_SetupIpServer(); // prepare for the client UI
	}

}

