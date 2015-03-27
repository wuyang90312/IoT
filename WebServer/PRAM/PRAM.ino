#include <EEPROM.h>

/* 1 KB (ATmega32u4) || 4 KB (Mega2560)*/
int a = 0;
int value;
int LENGTH=0;
int STA_IP[4] = {192,168,2,102};
int MQTT_IP[4] = {192,168,2,201};
int CLOUD_IP[4] = {233,251,75,12};
uint16_t MQTT_PORT = 1884;
uint16_t CLOUD_PORT = 654;

void setup()
{
  Serial.begin(9600);
  /*if(readConfig())
  {
    Serial.println("Set all values back to the default");
    reset(1024);
    Serial.println("Done with setting");
  }
  setMode(1);
  setSTAIP(STA_IP);
  setMQTTIP(MQTT_IP);
  setCLOUDIP(CLOUD_IP);
  setMQTTPort(MQTT_PORT);
  setCLOUDPort(CLOUD_PORT);
  setSSID("BCRLovs");
  setPWD("23456");
  setConfig();
  
  delay(5*1000); */
  
  Serial.println(readConfig());
  Serial.println(readMode());
  Serial.println(readSTAIP());
  Serial.println(readMQTTIP());
  Serial.println(readCLOUDIP());
  Serial.println(readMQTTPort());
  Serial.println(readCLOUDPort());
  Serial.println(readSSID());
  Serial.println(readPWD());
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

void reset(int Length)
{
  for(int i =0; i<Length; i++)
     EEPROM.write(i, 255);
}

boolean readConfig()
{
   if(EEPROM.read(0)==0)
     return true;
   else if(EEPROM.read(0)==255)
     return false;
   else
     Serial.println("[ERROR]: Unexpected Storage ");
     
     return false;
}

void setConfig()
{
  EEPROM.write(0,0);
}

void setMode(uint8_t input)
{
  EEPROM.write(7, input); 
}

int readMode()
{
  uint8_t tmp = EEPROM.read(7);
  return tmp;
}

void setIP(uint8_t location,int IP[4])
{
   for(int i=0; i < 4; i++)
  {
     EEPROM.write((location+i), IP[i]);
  } 
}

String  readIP(uint8_t location)
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

void setSTAIP(int IP[4])
{
  setIP(8, IP);
}

String readSTAIP()
{
 return readIP(8); 
}

void setMQTTIP(int IP[4])
{
    setIP(12, IP);
}

String readMQTTIP()
{
  return readIP(12);
}

void setCLOUDIP(int IP[4])
{
   setIP(18, IP);
}

String readCLOUDIP()
{
  return readIP(18);
}

void setPORT(uint8_t location, uint16_t port)
{
  store16BIT(location, port);
}

uint16_t readPORT(uint8_t location)
{
  uint16_t tmp = EEPROM.read(location);
  tmp += EEPROM.read(location+1)*256;
  
  return tmp;
}

void setMQTTPort(uint16_t port)
{
   setPORT(16, port);
}

uint16_t readMQTTPort()
{
   return readPORT(16); 
}

void setCLOUDPort(uint16_t port)
{
   setPORT(22, port);
}

uint16_t readCLOUDPort()
{
   return readPORT(22); 
}

void store16BIT(uint8_t location, uint16_t value)
{
  uint8_t tmp = value&0xff;
  EEPROM.write(location, tmp);
  tmp = value>>8;
  EEPROM.write((location+1), tmp);
}

void setString(uint16_t recordLocation, uint16_t location, String input)
{
  /* Take record of where to store the string */
  store16BIT(recordLocation, location);
  uint8_t length = input.length();
  LENGTH = length;
  EEPROM.write((recordLocation+2), length);
  storeString(length, location, input);
}

void storeString(uint8_t length, uint16_t location, String input)
{
  uint8_t tmp;
  for(int i = location; i<(location+length); i++)
  {
    tmp = input[i-location];
    EEPROM.write(i, tmp);
  }
}

String readSTRING(uint8_t location)
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

void setSSID(String input)
{
  setString(1, 24, input);
}

String readSSID()
{
  return readSTRING(1); 
}

void setPWD(String input)
{
 setString(4, (24+LENGTH), input);
}

String readPWD()
{
   return readSTRING(4); 
}
