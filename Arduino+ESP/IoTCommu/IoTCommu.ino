#include <SoftwareSerial.h>
#include "StringModule.h"

String Storage;

void setup()
{
  Serial.begin(115200);    // Arduino serial Monitor
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial1.begin(9600);   // The ESP8826 has a baud rate of 115200

  // Enable esp8266 
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  ESPWIFIsetup(); /* set up the configuration of ESP chip */
  ESPWIFIassociate();


}

void loop() {
  // Serial.println("Hello");
   
}

void ESPWIFIsetup()
{
  CommLaunch("AT+CIOBAUD?\r", 2*1000, true, 0);
  CommLaunch("AT+RST\r", 2*1000, true, 0);
  CommLaunch("AT+CWMODE=1\r", 1000, true, 0);
  CommLaunch("AT+CIFSR\r", 2*1000, true, 0);
  CommLaunch("AT+CWLAP\r", 6*1000, true, 1);
}

void ESPWIFIassociate()
{
  String usr,pw;
  usr = "QoS";
  pw = "23456";
  
  StringModule STR(Storage);
  Storage = "";

  if(STR.Contains(usr))
  {
    String asso = "AT+CWJAP=\""+usr+"\",\""+pw+"\"";
    while(true)
    {
      CommLaunch(asso, 20*1000, true, 1);

      STR.ReplaceBase(Storage);
      Storage = "";
      
      if(STR.Contains("OK"))
        break;
    }
  }
  else
    Serial.println("[ERROR0] : no matching result!!");
    
   Serial.println("FINISH ASSOCIATION!!");
  CommLaunch("AT+CIFSR\r", 2*1000, true, 0); 
}

boolean CommLaunch(String cmd,unsigned int duration, boolean res, int keyword)
{
  Serial1.println(cmd);    //send command to ESP8266

  if(res)
    CommResponse( keyword, duration);
}

void CommResponse(int keyword, unsigned int duration)
{
  String tmp ="";
  long deadline = millis()+duration;
  
  while(millis() < deadline)
  {
    while (Serial1.available()>0)  {
      tmp += char(Serial1.read());
    }

    if(tmp.length() > 0) {
      if(keyword==1)
        Storage += tmp; // if the keyword is triggered, copy the string

      Serial.print(tmp);
      tmp = "";
    }
  }
  Serial.println("\n------------------------------");
}

