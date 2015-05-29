#ifndef __ADXL_H__
#define __ADXL_H__

#include "c_types.h"

#define		ID		0 /* The communication channel ID */
#define		SDA		3 /* GPIO 2 */
#define		SCL		4 /* GPIO 0 */
#define		BASE_ADDR	0x53 /* The base address when SDO/CTL pin is low */
#define		PW_ADDR		0x2D /* The address of power control */
#define		FMT_ADDR	0x31 /* The address of data format */


uint32_t ICACHE_FLASH_ATTR ESP_set_up(unsigned id, uint8_t sda, uint8_t scl, uint32_t speed);
uint8_t ICACHE_FLASH_ATTR ESP_read_register(uint16_t dev_addr, uint8_t reg_addr);
uint8_t ICACHE_FLASH_ATTR ESP_write_register(uint16_t dev_addr, uint8_t reg_addr, uint8_t content);
void ICACHE_FLASH_ATTR ESP_init_config(uint8_t scale);
int ICACHE_FLASH_ATTR ESP_acquire_data(uint16_t msb_addr, uint16_t lsb_addr);

#endif
