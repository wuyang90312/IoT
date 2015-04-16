#ifndef __ESP_SERVER_H
#define __ESP_SERVER_H

#include "os_type.h"
#include "cmd.h"
#include "c_types.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"
#include "osapi.h"

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

uint32_t ICACHE_FLASH_ATTR ESP_SetupIpServer();
#endif
