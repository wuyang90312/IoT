#ifndef __ESP_SERVER_H
#define __ESP_SERVER_H

#include "os_type.h"
#include "c_types.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"
#include "osapi.h"
#include "debug.h"
#include "esp_string.h"
#include "wifi.h"

typedef enum
{
  teClient,
  teServer
}teType;

typedef struct
{
	BOOL linkEn;
	BOOL teToff;
	uint8_t linkId;
	teType teType;
	uint8_t repeaTime;
	uint8_t changType;
	uint8_t remoteIp[4];
	int32_t remotePort;
	struct espconn *pCon;
}linkConType;

typedef struct
{
char SSID[32];
char PWD[32];
char API[32];
char TIME[32];
}configInfo;



uint32_t ICACHE_FLASH_ATTR ESP_SetupIpServer();
#endif
