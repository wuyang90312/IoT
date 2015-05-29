#ifndef __GLOBAL_VAR_H__
#define __GLOBAL_VAR_H__

#include "user_interface.h"

/* The Global Variables of Permanent Memory Section */
#define	EEPROM_SECTION	0X3F			/* The memory section number of EEPROM --> the corresponding starting address is 0x3F000 */
#define	SECTION_SIZE	0x1000			/* Each memory section contains 4KB */
#define	DOWN_FLAG		0xFFFFFFFF		/* The flag meaning EEPROM is empty */
#define	UP_FLAG			0x10101010		/* The flag meaning EEPROM is already written */

#define	NUM_GPIO	13

/* The enumeration of 3 axes of ADXL345 */
enum
{
  X_AXIS=0,
  Y_AXIS,
  Z_AXIS
};

/* The MSB/LSB address of ADXL345 */
static const uint8_t MSB_ADDR_ADXL[3]={0x33,0x35,0x37};
static const uint8_t LSB_ADDR_ADXL[3]={0x32,0x34,0x36};

#define	TIME_ELEMENT	1000			/* Make 1000 ms as the elementary time interval */
#endif
