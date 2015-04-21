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

/* Define a type of the function pointer */
typedef void (*ptrFunction)(void); 
ptrFunction fcn_cb_ptr;

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

char* GLOBAL_SSID[32];
char* GLOBAL_PWD[32];
char* GLOBAL_API[32];
char* GLOBAL_TIME[32];

uint32_t ICACHE_FLASH_ATTR ESP_SetupIpServer();
#endif
