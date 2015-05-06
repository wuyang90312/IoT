#ifndef _ESP_STRING_H_
#define _ESP_STRING_H_

#include "osapi.h"
#include "os_type.h"
#include "c_types.h"
#include "mem.h"

uint16_t ICACHE_FLASH_ATTR
ESP_STR_ExtractData(uint16_t start,const char* target, char* pdata, uint8_t length);

BOOL ICACHE_FLASH_ATTR
ESP_STR_Delimit(char token, uint8_t num, char* target, uint16_t* ptr);

void ICACHE_FLASH_ATTR
ESP_STR_URL_Decode(char *str, int size);
#endif
