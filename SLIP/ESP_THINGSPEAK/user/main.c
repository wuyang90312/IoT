#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "rest.h"
#include "wifi.h"
#include "driver/uart.h"
#include "debug.h"
#include "esp_server.h"
#include "esp_string.h"
#include "global_var.h"

static ETSTimer timer;
static uint16_t waitCycle;
static uint16_t cycleCnter = 0;
char str[255];

void ICACHE_FLASH_ATTR
timer_cb(void *arg)
{
	if(cycleCnter == 0)
	{
		uint32_t rst = (uint32_t) arg;
		REST_Request(rst, "GET", str);
	}
	else
	{
		INFO("WAIT FOR NXT TERM:%u\n", cycleCnter);
	}
	cycleCnter = (cycleCnter+1)%waitCycle;
}

void ICACHE_FLASH_ATTR
func_repeat(os_timer_func_t *timer_cb, uint32_t arg_ptr, uint16_t time)
{
	uint16_t time_slice = TIME_ELEMENT;
	if(time<=65){
		time_slice = time*1000;
		waitCycle = 1;
	}else{
		waitCycle = time;	/* time is how many seconds here */
	}

	os_timer_disarm(&timer);
	os_timer_setfn(&timer, timer_cb, arg_ptr);
	os_timer_arm(&timer, time_slice, 1);
}

void ICACHE_FLASH_ATTR
user_continue(void)
{
	uint16_t len;
	char* hst;
	uint32_t rst;
	uint16_t time = 30;
	configInfo* configuration = (configInfo*)os_zalloc(sizeof(configInfo));
	
	/* Read configuration information from permanent memory */
	spi_flash_read((EEPROM_SECTION)*SECTION_SIZE+4, (uint32 *) configuration, sizeof(configInfo));
	time = (uint16_t)atoi(configuration->TIME);

	INFO("\nThe configuration is:||%s||%s||%s||%u||\n", 
	configuration->SSID, configuration->PWD, configuration->API,time);

	os_sprintf(str, "/update?api_key=%s&field1=40", &configuration->API);
	hst = "api.thingspeak.com";
	len = os_strlen(hst);

	WIFI_Connect((uint8_t*)configuration->SSID, (uint8_t*)configuration->PWD, NULL);
	rst = REST_Setup(hst, len, 80,0x00000000);

	func_repeat((os_timer_func_t *)timer_cb, rst, time);
}

void ICACHE_FLASH_ATTR
user_init(void)
{
	uint32_t buff=DOWN_FLAG;
	uart_init(BIT_RATE_9600, BIT_RATE_9600);
	os_delay_us(1000000);

	//spi_flash_erase_sector(EEPROM_SECTION);
	//spi_flash_write((EEPROM_SECTION)*SECTION_SIZE, (uint32 *) &buff, 4);
	spi_flash_read((EEPROM_SECTION)*SECTION_SIZE, (uint32 *) &buff, 4);
	INFO("\nThe memeory is: %x\n", buff);
	if(buff == UP_FLAG)
	{
		user_continue();
	}
	else 
	{
		WIFI_AP_Config((uint8_t*)"AI-THINKER-SERV", (uint8_t*) "23456789");
		ESP_SetupIpServer(); // prepare for the client UI
	}

}

