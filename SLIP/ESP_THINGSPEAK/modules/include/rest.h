/*
 * api.h
 *
 *  Created on: Mar 4, 2015
 *      Author: Minh
 */

#ifndef MODULES_API_H_
#define MODULES_API_H_
#include "c_types.h"
#include "ip_addr.h"

int ALREADYSENT;

typedef enum {
  HEADER_GENERIC = 0,
  HEADER_CONTENT_TYPE,
  HEADER_USER_AGENT
} HEADER_TYPE;


typedef struct {
	uint8_t* host;
	uint32_t port;
	uint32_t security;
	ip_addr_t ip;
	struct espconn *pCon;
	uint8_t* header;
	uint8_t* data;
	uint32_t data_len;
	uint8_t* content_type;
	uint8_t* user_agent;
	uint32_t resp_cb;
} REST_CLIENT;

uint32_t ICACHE_FLASH_ATTR REST_Setup(uint8_t *rest_host, uint16_t len, uint32_t port,uint32_t security);
uint32_t ICACHE_FLASH_ATTR REST_Request(uint32_t client_ptr, uint8_t *method, uint8_t *path);
#endif /* MODULES_INCLUDE_API_H_ */
