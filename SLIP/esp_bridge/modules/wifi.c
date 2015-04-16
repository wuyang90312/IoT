/*
 * wifi.c
 *
 *  Created on: Dec 30, 2014
 *      Author: Minh
 */
#include "wifi.h"
#include "user_interface.h"
#include "osapi.h"
#include "espconn.h"
#include "mem.h"

#include "user_config.h"
#include "debug.h"

static ETSTimer WiFiLinker;
uint32_t wifiCb = 0;
static uint8_t wifiStatus = STATION_IDLE, lastWifiStatus = STATION_IDLE;
static void ICACHE_FLASH_ATTR wifi_check_ip(void *arg)
{
	struct ip_info ipConfig;

	os_timer_disarm(&WiFiLinker);
	wifi_get_ip_info(STATION_IF, &ipConfig);
	wifiStatus = wifi_station_get_connect_status();
	if (wifiStatus == STATION_GOT_IP && ipConfig.ip.addr != 0)
	{
		os_timer_setfn(&WiFiLinker, (os_timer_func_t *)wifi_check_ip, NULL);
		os_timer_arm(&WiFiLinker, 2000, 0);
	}
	else
	{
		if(wifi_station_get_connect_status() == STATION_WRONG_PASSWORD)
		{
			INFO("STATION_WRONG_PASSWORD\r\n");
			wifi_station_connect();
		}
		else if(wifi_station_get_connect_status() == STATION_NO_AP_FOUND)
		{
			INFO("STATION_NO_AP_FOUND\r\n");
			wifi_station_connect();
		}
		else if(wifi_station_get_connect_status() == STATION_CONNECT_FAIL)
		{
			INFO("STATION_CONNECT_FAIL\r\n");
			wifi_station_connect();
		}
		else
		{
			INFO("STATION_IDLE\r\n");
		}

		os_timer_setfn(&WiFiLinker, (os_timer_func_t *)wifi_check_ip, NULL);
		os_timer_arm(&WiFiLinker, 500, 0);
	}
	if(wifiStatus != lastWifiStatus){
		lastWifiStatus = wifiStatus;
		if(wifiCb){
			uint16_t crc = CMD_ResponseStart(CMD_WIFI_CONNECT, wifiCb, 0, 1);
			crc = CMD_ResponseBody(crc, (uint8_t*)&wifiStatus, 1);
			CMD_ResponseEnd(crc);
		}
	}
}

uint32_t ICACHE_FLASH_ATTR
WIFI_Connect(PACKET_CMD *cmd)
{
	struct station_config stationConf;
	uint8_t *ssid, *pwd;
	ssid = (uint8_t*)&cmd->args;
	pwd = ssid + *(uint16_t*)ssid + 2;

	INFO("WIFI_INIT\r\n");


	os_memset(&stationConf, 0, sizeof(struct station_config));

	os_memcpy(stationConf.ssid, ssid + 2, *(uint16_t*)ssid);
	os_memcpy(stationConf.password, pwd + 2, *(uint16_t*)pwd);

	INFO("WIFI: set ssid = %s, pass= %s\r\n", stationConf.ssid, stationConf.password);
	wifi_station_set_auto_connect(FALSE);
	wifi_set_opmode(STATION_MODE);

	if(cmd->argc != 2 || cmd->callback == 0)
		return 0xFFFFFFFF;

	wifiCb = cmd->callback;
	wifi_station_set_config(&stationConf);

	os_timer_disarm(&WiFiLinker);
	os_timer_setfn(&WiFiLinker, (os_timer_func_t *)wifi_check_ip, NULL);
	os_timer_arm(&WiFiLinker, 1000, 0);

	wifi_station_set_auto_connect(TRUE);
	wifi_station_connect();
	return 0;
}

/* @brief	Set up an the SSID and password when ESP is at AP mode 
 * @param	(PACKET_CMD) command package
 * @retval	 0: Success
 * @error	-1: Failure
 * */
uint32_t ICACHE_FLASH_ATTR
WIFI_AP_Config(PACKET_CMD *cmd)
{
	/* If the argument is not exactly 2 or no callback, wrong packet */
	//if(cmd->argc != 2)
	//	return 0xFFFFFFFF;
	
	struct softap_config apConfig;
	uint8_t *ssid, *pwd, ssidLength, pwdLength, result;
	ssid = (uint8_t*)&cmd->args;
	ssidLength = *(uint16_t*)ssid; /* The first 2 bytes of each arg describe the length of that argument */
	pwd = ssid + ssidLength + 2; /* Move to the ptr of nxt argument */ 
	pwdLength = *(uint16_t*)pwd;

	INFO("AP_CONFIG\r\n");

	/* Set up the memory space of the configuration file */
	os_bzero(&apConfig, sizeof(struct softap_config));
	INFO("1 AP MODE CONFIGURATION: set ssid = %s, pass= %s, authen = %d, result = %d, mode = %d\r\n", \
	apConfig.ssid, apConfig.password, apConfig.authmode, result, wifi_get_opmode());
	wifi_set_opmode(SOFTAP_MODE); /* set the ESP into an AP mode */
	wifi_softap_get_config(&apConfig); /* Configure the ESP at AP mode */

	/* Copy the content into the corresponding vairables of configuration file */
	os_memcpy(apConfig.ssid, ssid + 2, ssidLength); /* Skip the first 2 bytes cuz it is just the length */
	os_memcpy(apConfig.password, pwd + 2, pwdLength);
	//apConfig.ssid_len = ssidLength;
	/* TODO: For this moment, hardcode the rest of configuration. Later, implemented if needed */
	apConfig.authmode = AUTH_WPA_WPA2_PSK;
	//apConfig.ssid_hidden = 0;
	apConfig.channel = 5;
	apConfig.max_connection = 4;

	wifi_softap_set_config(&apConfig);

	INFO(" AP MODE CONFIGURATION: set ssid = %s, pass= %s, authen = %d, result = %d, mode = %d\r\n", \
	apConfig.ssid, apConfig.password, apConfig.authmode, result, wifi_get_opmode());


	return 0;
}
