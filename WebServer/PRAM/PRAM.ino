#include <EEPROM.h>
#include "PROM.h"

/* 1 KB (ATmega32u4) || 4 KB (Mega2560)*/
int a = 0;
int value;
int LENGTH=0;
uint8_t STA_IP[4] = {192,168,2,102};
uint8_t MQTT_IP[4] = {192,168,2,201};
uint8_t CLOUD_IP[4] = {233,251,75,12};
uint16_t MQTT_PORT = 1884;
uint16_t CLOUD_PORT = 654;
String _SSID= "aBCRLovs";
String PWD = "123456";

void setup()
{
  Serial.begin(9600);
  
  PROM prom(1, STA_IP, MQTT_IP, CLOUD_IP, MQTT_PORT, CLOUD_PORT, _SSID, PWD);
  Serial.println(prom.readConfig());
  Serial.println(prom.readMode());
  Serial.println(prom.readSTAIP());
  Serial.println(prom.readMQTTIP());
  Serial.println(prom.readCLOUDIP());
  Serial.println(prom.readMQTTPort());
  Serial.println(prom.readCLOUDPort());
  Serial.println(prom.readSSID());
  Serial.println(prom.readPWD());
  delay(5*1000);
  
  if(prom.readConfig())
  {
    Serial.println("Set all values back to the default");
    prom.reset(1024);
    Serial.println("Done with setting");
  }
  /*setMode(1);
  setSTAIP(STA_IP);
  setMQTTIP(MQTT_IP);
  setCLOUDIP(CLOUD_IP);
  setMQTTPort(MQTT_PORT);
  setCLOUDPort(CLOUD_PORT);
  setSSID("BCRLovs");
  setPWD("23456");
  setConfig();
  
  delay(5*1000); */
  delay(5*1000);
  
  Serial.println(prom.readConfig());
  Serial.println(prom.readMode());
  Serial.println(prom.readSTAIP());
  Serial.println(prom.readMQTTIP());
  Serial.println(prom.readCLOUDIP());
  Serial.println(prom.readMQTTPort());
  Serial.println(prom.readCLOUDPort());
  Serial.println(prom.readSSID());
  Serial.println(prom.readPWD());
}

// Display the data stored in EEPROM
void loop()
{
  char tmp;
  value = EEPROM.read(a);
  tmp = value;
  Serial.print(a);
  Serial.print("\t");
  Serial.print(value);
  Serial.print("\t");
  Serial.println(tmp);

  a = a + 1;

  if (a == 1024*4)
  {
    Serial.println("----------------");
    a = 0;
  }

  delay(500);
}


