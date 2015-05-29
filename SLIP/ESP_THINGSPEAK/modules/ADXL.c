#include "I2C.h"
#include "debug.h"
#include "ADXL.h"

/* Set up the configuration of an I2C channel
 * @param	id		channel number
 * @param	sda		sda PIN number
 * @param	scl		scl PIN number
 * @param	speed		transmitting speed
 * @return	Slow spead of transmitting speed
 * */
uint32_t ICACHE_FLASH_ATTR ESP_set_up(unsigned id, uint8_t sda, uint8_t scl, uint32_t speed)
{
	return platform_i2c_setup(id, sda, scl, speed );
}

/* Read the response data from ADXL345
 * @param	dev_addr		The basic memory address
 * @param	reg_addr		The memory address corresponding to certain functions
 * @return	requested value
 * */
uint8_t ICACHE_FLASH_ATTR ESP_read_register(uint16_t dev_addr, uint8_t reg_addr)
{
	uint8_t res;
	platform_i2c_send_start(ID);
	res= platform_i2c_send_address(ID, dev_addr, PLATFORM_I2C_DIRECTION_TRANSMITTER);
	res= platform_i2c_send_byte(ID, reg_addr);
	platform_i2c_send_stop(ID);
	platform_i2c_send_start(ID);
	res = platform_i2c_send_address(ID, dev_addr, PLATFORM_I2C_DIRECTION_RECEIVER);
	res = platform_i2c_recv_byte(ID, 0);
	platform_i2c_send_stop(ID);
	return res;
}

/* Write data into ADXL345
 * @param	dev_addr		The basic memory address
 * @param	reg_addr		The memory address corresponding to certain functions
 * @param	content			The input data
 * @return	bolean statement whether data is passed successfully
 * */
uint8_t ICACHE_FLASH_ATTR ESP_write_register(uint16_t dev_addr, uint8_t reg_addr, uint8_t content)
{
	uint8_t res;
	platform_i2c_send_start(ID);
	platform_i2c_send_address(ID, dev_addr, PLATFORM_I2C_DIRECTION_TRANSMITTER);
	res= platform_i2c_send_byte(ID, reg_addr);
	res = platform_i2c_send_byte(ID, content);
	platform_i2c_send_stop(ID);
	return res;
}

/* Initialize all relevant configuration of ADXL345
 * @param	scale	Decide the mode set of 2g, 4g, 8g, 16g
 * @return	void
 * */
void ICACHE_FLASH_ATTR ESP_init_config(uint8_t scale)
{
	uint8_t debug;
	// set up the I2C communication
	ESP_set_up(ID, SDA, SCL, PLATFORM_I2C_SPEED_SLOW);
	// set the chip to a measurement mode
	debug = ESP_write_register(BASE_ADDR, PW_ADDR, 0x08);
	INFO("MESURE MODE: %d\n", debug);
	// PUT THE RIGHT G SCALE OF THE MEASUREMENT: 2g, 4g, 8g, and 16g
	debug = ESP_write_register(BASE_ADDR, FMT_ADDR, scale);
	INFO("MESURE SCALE: %d\n", debug);
}


/* Get the reading of certain axis from ADXL345
 * @param	msb_addr		The address where store MSB
 * @param	lsb_addr		The address where store MSB
 * @return	reading of the chosen axis
 * */
int ICACHE_FLASH_ATTR ESP_acquire_data(uint16_t msb_addr, uint16_t lsb_addr)
{
	int LSB_data;
	uint16_t MSB_data;
	// get content of the least significant byte
	LSB_data = ESP_read_register(BASE_ADDR, lsb_addr);
	
	// get content of the most significant byte
	MSB_data = ESP_read_register(BASE_ADDR, msb_addr);
	INFO("\n The initial data: %d  %d\n", MSB_data, LSB_data);

	/* If MSB is 0, the value should be + sign; if MSB is 255, the value should 
	 * take its 2's complementary */
	if(MSB_data == 255)
		LSB_data = -(256-LSB_data);
	//INFO("\n The result data: %d \n", LSB_data);
	return LSB_data;
}

