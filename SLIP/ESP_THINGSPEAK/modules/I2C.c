#include "I2C.h"

// *****************************************************************************
// I2C platform interface
/* The code is originated from the platform.c of Node MCU */

uint32_t platform_i2c_setup( unsigned id, uint8_t sda, uint8_t scl, uint32_t speed ){
  if (sda >= NUM_GPIO || scl >= NUM_GPIO)
    return 0;

  i2c_master_gpio_init(sda, scl);
  return PLATFORM_I2C_SPEED_SLOW;
}

void platform_i2c_send_start( unsigned id ){
  i2c_master_start();
}

void platform_i2c_send_stop( unsigned id ){
  i2c_master_stop();
}

int platform_i2c_send_address( unsigned id, uint16_t address, int direction ){
  // Convert enum codes to R/w bit value.
  // If TX == 0 and RX == 1, this test will be removed by the compiler
  if ( ! ( PLATFORM_I2C_DIRECTION_TRANSMITTER == 0 &&
           PLATFORM_I2C_DIRECTION_RECEIVER == 1 ) ) {
    direction = ( direction == PLATFORM_I2C_DIRECTION_TRANSMITTER ) ? 0 : 1;
  }

  i2c_master_writeByte( (uint8_t) ((address << 1) | direction ));
  // Low-level returns nack (0=acked); we return ack (1=acked).
  return ! i2c_master_getAck();
}

int platform_i2c_send_byte( unsigned id, uint8_t data ){
  i2c_master_writeByte(data);
  // Low-level returns nack (0=acked); we return ack (1=acked).
  return ! i2c_master_getAck();
}

int platform_i2c_recv_byte( unsigned id, int ack ){
  uint8_t r = i2c_master_readByte();
  i2c_master_setAck( !ack );
  return r;
}
