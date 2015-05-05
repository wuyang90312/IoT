#ifndef __GLOBAL_VAR_H__
#define __GLOBAL_VAR_H__

/* The Global Variables of Permanent Memory Section */
#define	EEPROM_SECTION	0X3F			/* The memory section number of EEPROM --> the corresponding starting address is 0x3F000 */
#define	SECTION_SIZE	0x1000			/* Each memory section contains 4KB */
#define	DOWN_FLAG		0xFFFFFFFF		/* The flag meaning EEPROM is empty */
#define	UP_FLAG			0x10101010		/* The flag meaning EEPROM is already written */

#endif
