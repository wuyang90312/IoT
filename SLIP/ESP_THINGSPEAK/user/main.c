#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "rest.h"
#include "wifi.h"
#include "driver/uart.h"
#include "debug.h"
#include "esp_server.h"
#include "esp_string.h"

void ICACHE_FLASH_ATTR
user_init(void)
{
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	os_delay_us(1000000);
	
	WIFI_AP_Config((uint8_t*)"AI-THINKER-SERVER", (uint8_t*) "23456789");
	ESP_SetupIpServer();
}

