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
String API = "GPVP0E6QQVWU47LZ";

void setup()
{
  Serial.begin(115200);
  
  PROM prom;
  Serial.println(prom.readConfig());
  Serial.println(prom.readMode());
  Serial.println(prom.readSTAIP());
  Serial.println(prom.readMQTTIP());
  Serial.println(prom.readCLOUDIP());
  Serial.println(prom.readMQTTPort());
  Serial.println(prom.readCLOUDPort());
  Serial.println(prom.readSSID());
  Serial.println(prom.readPWD());
  Serial.println(prom.readAPI());
  delay(5*1000);
  
 /* if(prom.readConfig())
  {
    Serial.println("Set all values back to the default");
    prom.reset(1024);
    Serial.println("Done with setting");
  }*/
  prom.setMode(1);
  prom.setSTAIP(STA_IP);
  prom.setMQTTIP(MQTT_IP);
  prom.setCLOUDIP(CLOUD_IP);
  prom.setMQTTPort(MQTT_PORT);
  prom.setCLOUDPort(CLOUD_PORT);
  prom.setSSID(_SSID);
  prom.setPWD(PWD);
  prom.setAPI(API);
  prom.setConfig();

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
  Serial.println(prom.readAPI());
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


