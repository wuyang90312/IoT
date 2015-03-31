/**
 * @file    PROM.H
 * @brief   Configure a scheduler to manage multiple threads
 * @author  Yang Wu
 * @version 1.0
 * @see     
 *
 * Copyright (c) 2015
 */


#ifndef PROM_H
#define PROM_H

#include <EEPROM.h>
#include <Arduino.h>

class PROM {

private:
  void setIP(uint8_t location,uint8_t IP[4]);
  String readIP(uint8_t location);
  uint16_t readPORT(uint8_t location);
  void setPORT(uint8_t location, uint16_t port);
  void store16BIT(uint8_t location, uint16_t value);
  void setString(uint16_t recordLocation, uint16_t location, String input);
  void storeString(uint8_t length, uint16_t location, String input);
  String readSTRING(uint8_t location);
  
public:
  PROM();
  void Flash(int Mode, uint8_t STA_IP[4], uint8_t MQTT_IP[4], uint8_t CLOUD_IP[4], uint16_t MQTT_PORT,uint16_t CLOUD_PORT,String ssid, String pwd, String api);
  void reset(int Length);
  void setConfig();
  void setMode(uint8_t input);
  void setSTAIP(uint8_t IP[4]);
  void setMQTTIP(uint8_t IP[4]);
  void setCLOUDIP(uint8_t IP[4]);
  void setMQTTPort(uint16_t port);
  void setCLOUDPort(uint16_t port);
  void setSSID(String input);
  void setPWD(String input);
  void setAPI(String input);
  boolean readConfig();
  int readMode();
  String readSTAIP();
  String readCLOUDIP();
  String readMQTTIP();
  uint16_t readMQTTPort();
  uint16_t readCLOUDPort();
  String readSSID();
  String readPWD();
  String readAPI();
};

#endif

