#include "esp_string.h"

/**
  * @brief  Extract a certain String from a large string
  * @param  start: the starting point to search
  * @param  target: the target word which we are looking for
  * @param  pdata: the data source where we are looking into
  * @param  length: the length of the target
  * @retval  The end location
  */
uint16_t ICACHE_FLASH_ATTR
ESP_STR_ExtractData(uint16_t start,const char* target, char* pdata, uint8_t length)
{
	uint16_t position = start;
	char tmp[32], tmp2[32];
	os_strcpy(tmp2, target);
	while(os_strlen(pdata)>=(position+length))
	{
		os_memset(tmp, 0x00, sizeof(tmp));
		os_memcpy(tmp, pdata+position, length);
		if(!os_strcmp(tmp, tmp2))
			return position;
		
		position ++;
	}
	return 0;
}

/**
  * @brief  Parse the string with certain deliminater
  * @param  token: the deliminater
  * @param  num: the amount of deliminaters which supposed to be in the string
  * @param  target: the target string
  * @param  ptr: the integer array storing the position of each deliminater
  * @retval  BOOL whether this works
  */
BOOL ICACHE_FLASH_ATTR
ESP_STR_Delimit(char token, uint8_t num, char* target, uint16_t* ptr)
{
	uint16_t length, i=0, loop=0;
	length = os_strlen(target);
	
	os_memset(ptr, 0x00, num*sizeof(int));

	while(loop<length)
	{
		if(target[loop] == token)
		{
			ptr[i] = loop;
			i++;
		}
		loop++;
	}

	if(i==num)
		return TRUE;

	return FALSE;
}
