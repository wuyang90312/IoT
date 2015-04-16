#ifndef __ESP_SERVER_H
#define __ESP_SERVER_H

typedef enum
{
  teClient,
  teServer
}teType;

typedef struct
{
	BOOL linkEn;
	BOOL teToff;
	uint8_t linkId=0;
	teType teType;
	uint8_t repeaTime;
	uint8_t changType;
	uint8_t remoteIp[4];
	int32_t remotePort;
	struct espconn *pCon;
}linkConType;

void ICACHE_FLASH_ATTR
ESP_SetupIpServer();
#endif
