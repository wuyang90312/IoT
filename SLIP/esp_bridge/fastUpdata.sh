#!/bin/bash
# help to fasten the whole uploading process

make clean
make SDK_BASE="/opt/Espressif/ESP8266_SDK" FLAVOR="release" all
cd firmware
sudo /opt/Espressif/esptool-py/esptool.py --port /dev/ttyUSB0 write_flash 0x00000 0x00000.bin 0x40000 0x40000.bin
