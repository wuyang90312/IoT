#ifndef __I2C_H__
#define __I2C_H__

#include "global_var.h"
#include "user_interface.h"
#include "driver/uart.h"
// *****************************************************************************
// I2C platform interface

// I2C speed
enum
{
  PLATFORM_I2C_SPEED_SLOW = 100000,
  PLATFORM_I2C_SPEED_FAST = 400000
};

// I2C direction
enum
{
  PLATFORM_I2C_DIRECTION_TRANSMITTER,
  PLATFORM_I2C_DIRECTION_RECEIVER
};

int platform_i2c_exists( unsigned id );
uint32_t platform_i2c_setup( unsigned id, uint8_t sda, uint8_t scl, uint32_t speed );
void platform_i2c_send_start( unsigned id );
void platform_i2c_send_stop( unsigned id );
int platform_i2c_send_address( unsigned id, uint16_t address, int direction );
int platform_i2c_send_byte( unsigned id, uint8_t data );
int platform_i2c_recv_byte( unsigned id, int ack );

#endif
