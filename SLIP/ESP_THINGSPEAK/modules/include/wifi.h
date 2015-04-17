/*
 * wifi.h
 *
 *  Created on: Dec 30, 2014
 *      Author: Minh
 */

#ifndef USER_WIFI_H_
#define USER_WIFI_H_
#include "os_type.h"

typedef void (*WifiCallback)(uint8_t);
uint32_t ICACHE_FLASH_ATTR
WIFI_Connect(uint8_t* ssid, uint8_t* pwd, WifiCallback cb);

/* This module is added in order to change the SSID/password at AP mode */
uint32_t ICACHE_FLASH_ATTR
WIFI_AP_Config(uint8_t* ssid, uint8_t* pwd);

#endif /* USER_WIFI_H_ */
