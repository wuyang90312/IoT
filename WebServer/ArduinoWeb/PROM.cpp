/*
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE" (Revision 42):
* <wuyang90312@gmail.com> wrote this file.  As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a beer in return.   Yang Wu
* ----------------------------------------------------------------------------
* This code is written to read/write the EEPROM by following a certain data format
* March 27th 2015
*/

#include "PROM.h"

/*--------------------Private method helps to simplify the code--------------------------------*/

void PROM::setIP(uint8_t location,uint8_t IP[4])
{
   for(int i=0; i < 4; i++)
  {
     EEPROM.write((location+i), IP[i]);
  } 
}

String  PROM::readIP(uint8_t location)
{
  String tmp;
  
  for(int i=0; i < 3; i++)
  {
    tmp +=EEPROM.read(location+i);
    tmp +=".";
  }
  tmp +=EEPROM.read(location+3);
  
  return tmp;
}

uint16_t PROM::readPORT(uint8_t location)
{
  uint16_t tmp = EEPROM.read(location);
  tmp += EEPROM.read(location+1)*256;
  
  return tmp;
}

void PROM::setPORT(uint8_t location, uint16_t port) /* set uint16_t */
{
  store16BIT(location, port);
}

void PROM::store16BIT(uint8_t location, uint16_t value)
{
  uint8_t tmp = value&0xff;
  EEPROM.write(location, tmp);
  tmp = value>>8;
  EEPROM.write((location+1), tmp);
}

void PROM::setString(uint16_t recordLocation, uint16_t location, String input)
{
  /* Take record of where to store the string */
  store16BIT(recordLocation, location);
  uint8_t Length = input.length();
  EEPROM.write((recordLocation+2), Length);
  storeString(Length, location, input);
}

void PROM::storeString(uint8_t Length, uint16_t location, String input)
{
  uint8_t tmp;
  for(int i = location; i<(location+Length); i++)
  {
    tmp = input[i-location];
    EEPROM.write(i, tmp);
  }
}

String PROM::readSTRING(uint8_t location)
{
   String resp;
   char   tmp;
   uint8_t length = EEPROM.read(location+2);
   uint16_t loc = EEPROM.read(location);
   loc +=  EEPROM.read(location+1)*256;
   
   for(int i = loc; i<(length+loc);i++)
   {
     tmp = EEPROM.read(i);
     resp += tmp;
   }
   
   return resp;
}
/*-------------------------------SET Functions-------------------------------------------*/
PROM::PROM()
{
  
}

void PROM::Flash(int Mode, uint8_t STA_IP[4], uint8_t MQTT_IP[4], uint8_t CLOUD_IP[4], uint16_t MQTT_PORT,uint16_t CLOUD_PORT,String ssid, String pwd, String api, uint16_t time)
{
  setMode(Mode);
  setSTAIP(STA_IP);
  setMQTTIP(MQTT_IP);
  setCLOUDIP(CLOUD_IP);
  setMQTTPort(MQTT_PORT);
  setCLOUDPort(CLOUD_PORT);
  setSSID(ssid);
  setPWD(pwd);
  setAPI(api);
  setFREQUENCY(time);
  setConfig();
}

void PROM::reset(int Length)
{
  for(int i =0; i<Length; i++)
     EEPROM.write(i, 255);
}

void PROM::setConfig()
{
  EEPROM.write(0,0);
}

void PROM::setMode(uint8_t input)
{
  EEPROM.write(7, input); 
}

void PROM::setSTAIP(uint8_t IP[4])
{
  setIP(8, IP);
}

void PROM::setMQTTIP(uint8_t IP[4])
{
    setIP(12, IP);
}

void PROM::setCLOUDIP(uint8_t IP[4])
{
   setIP(18, IP);
}

void PROM::setMQTTPort(uint16_t port)
{
   setPORT(16, port);
}

void PROM::setCLOUDPort(uint16_t port)
{
   setPORT(22, port);
}

void PROM::setSSID(String input)
{
  setString(1, 124, input);
}

void PROM::setPWD(String input)
{
  uint8_t tmp = EEPROM.read(3);
  setString(4, (124+tmp), input);
}

void PROM::setAPI(String input)
{
   uint8_t tmp = EEPROM.read(6);
   uint16_t location = readPORT(4);
   location +=tmp;
   setString(24, location, input);
   Serial.println(location);
}

void PROM::setFREQUENCY(uint16_t time)
{
  setPORT(27, time);
}

/*-------------------------------GET Functions-------------------------------------------*/

boolean PROM::readConfig()
{
   if(EEPROM.read(0)==0)
     return true;
   else if(EEPROM.read(0)==255)
     return false;
   else
     Serial.println("[ERROR]: Unexpected Storage ");
     
     return false;
}

int PROM::readMode()
{
  uint8_t tmp = EEPROM.read(7);
  return tmp;
}

String PROM::readSTAIP()
{
 return readIP(8); 
}

String PROM::readCLOUDIP()
{
  return readIP(18);
}

String PROM::readMQTTIP()
{
  return readIP(12);
}

uint16_t PROM::readMQTTPort()
{
   return readPORT(16); 
}

uint16_t PROM::readCLOUDPort()
{
   return readPORT(22); 
}

String PROM::readSSID()
{
  return readSTRING(1); 
}

String PROM::readPWD()
{
   return readSTRING(4); 
}
String PROM::readAPI()
{
  return readSTRING(24); 
}
uint16_t PROM::readFREQUENCY()
{
  return readPORT(27); 
}
