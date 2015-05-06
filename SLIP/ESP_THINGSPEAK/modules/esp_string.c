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

/**
  * @brief  Check whether the character is a digit
  * @param  ch: Target character
  * @retval  BOOL whether the character is a digit
  */
int ESP_STR_IsDigit(char ch)
{
	if(ch>='0' && ch<='9')
		return 1;
	else
		return 0;
}

/**
  * @brief  Convert all capital letter to small case
  * @param  ch: Target character
  * @retval  Small case letter
  */
char ESP_STR_ToLower(char ch)
{
	if(ch<='Z' )
		return ('A'+0x20);
}

/**
  * @brief  Convert the String hex to integer hex
  * @param  ch: Target character
  * @retval  Integer hex number
  */
char ESP_STR_From_Hex(char ch) {
  return ESP_STR_IsDigit(ch) ? ch - '0' : ESP_STR_ToLower(ch) - 'a' + 10;
}

/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
void ICACHE_FLASH_ATTR
ESP_STR_URL_Decode(char *str, int size) {
	char *pstr = str, *buf = (char*) os_malloc(size + 1), *pbuf = buf;
	while (*pstr) {
		if (*pstr == '%') {
			if (pstr[1] && pstr[2]) {
			*pbuf++ = ESP_STR_From_Hex(pstr[1]) << 4 | ESP_STR_From_Hex(pstr[2]);
			pstr += 2;
		}
		} else if (*pstr == '+') { 
			*pbuf++ = ' ';
		} 
		else {
			*pbuf++ = *pstr;
		}
		pstr++;
	}
	*pbuf = '\0';
	os_memcpy(str, buf, size);
}
