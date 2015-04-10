#include <EEPROM.h>
#include <Wire.h>
#include "PROM.h"
#include "StringModule.h"



/* 1 KB (ATmega32u4)*/
String resp;
String Command;  /* Keep track of the command */
int    Decision; /* Decide which kind of behavior of ESP should have */
char   ChannelID; /*Channel is in scale of 0-6, No need to worry about more than 2 chars */
int    tmp102Address = 0x48; /* Base address of thermometer */
bool   reset = false;
 
StringModule STR("");
PROM prom;

void setup()
{
  Serial.begin(115200);    // Arduino serial Monitor
  Serial1.begin(9600);   // The ESP8826 has a baud rate of 115200
  
  // Enable esp8266 
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  UIweb();
  
  delay(3*1000); /* Give a time break b/t two critical commands */
  EEPROMconfiguration();
}

void ESPTCPconnection()
{
  String URL;
  
  URL = "AT+CIPSTART=0,\"TCP\",\"184.106.153.149\",80";
  CommLaunch(URL, 8*1000, true, 2);
   
  //CommLaunch("AT+CIPSTATUS\r", 2*1000, true, 0); 
}

void EEPROMconfiguration()
{
  String msg;
 
  CommLaunch("AT+RST\r", 8*1000, true, 0);
   
  CommLaunch("AT+CIPSTATUS\r", 2*1000, true, 0); 
   
  msg = "AT+CWMODE=";
  msg += prom.readMode();
  msg +="\r";
  CommLaunch(msg, 1000, true, 0);
  
  CommLaunch("AT+CIPMUX=1\r", 1000, true, 0);
  
  msg = "AT+CWJAP=\"";
  msg += prom.readSSID();
  msg += "\",\"";
  msg += prom.readPWD();
  msg += "\"\r";
  CommLaunch(msg, 8*1000, true, 2);
  
}

void loop()
{
  long time = millis();
  float celsius = getTemperature();
  
  if(reset)
  {
   celsius =0;
   reset = false; 
  }
  
  ESPTCPconnection();
  ESPUpload(celsius);
  
  if(!STR.Contains("CLOSED"))
  {
      reset = true;
        EEPROMconfiguration();
  }
  
  time = prom.readFREQUENCY()*1000 + time - millis();
  if(time > 0)
    delay(time);
}

void ESPTCPsend(int length)
{
  String message = "AT+CIPSEND=0,"; 
  message+=length;
  CommLaunch(message, 2*1000, true, 0); 
}

void ESPUpload(float input)
{
  String content;
  int SIZE;
  content = "GET /update?key=";
  content += prom.readAPI();
  content += "&field1=";
  content += input;
  SIZE = content.length()+1;

  ESPTCPsend(SIZE);
  CommLaunch(content, 8*1000, true, 1); 
  
}

void UIweb()
{
  long deadline = 600000+millis(); /* If the EEPROM is already configured, wait for 10 min */
  //Serial.println(deadline);
  Configuration();

  while(true)
  {
    CommResponse(1, 100); // Wait for the HTTP access
    if(STR.Storage.length()>0)
    {
      if(STR.Contains("+IPD,"))
      {
       // Serial.println(F("------------------GET----------------------"));
        ChannelID = STR.Storage.charAt(STR.readPosition()); 
        STR.Storage = ""; // Clear the memory for nxt time
        CommResponse(0, 5*1000); // Wait for HTTP response to finish
        uploadUI();
        break;
      }
    }

    STR.Storage = "";// Clear the memory for nxt time
    if(millis() > deadline && prom.readConfig())
    {
      //Serial.println(deadline);
      //Serial.println(millis());
      //Serial.println("Wait for 10 min");
      break;
    }
  }
}

boolean uploadUI()
{
  String msg,result;
  resp = "<h1> CONFIGURATION </h1>\n";
  resp +="<form  method=\"get\">";
  resp += "<p>SSID &nbsp; <input type=\"text\" name=\"SSID\"><br>";   /* Always name the name with 4 character word, easy to parse in the later step */
  resp += "PASSWORD &nbsp; <input type=\"text\" name=\"PAWD\"><br>";
  resp += "API KEY &nbsp; <input type=\"text\" name=\"APIK\"><br>";
  resp += "Update Duration(Sec) &nbsp;<input type=\"text\" name=\"TIME\"><br></p>";
  resp += "<a type=\"submit\" value=\"submit\"><button>SUBMIT</button></a>";
  resp += "</form>\r";
  msg = "AT+CIPSEND=";
  msg +=ChannelID;
  msg += ",";
  msg += resp.length();
  msg +="\r";
 
  CommLaunch(msg, 1000, true, 0);
  CommLaunch("resp",1.5*1000, true, 0);
  resp=""; // clean the string which is occupying space
  msg = "AT+CIPCLOSE=";
  msg +=ChannelID;
  msg +="\r";
  CommLaunch(msg, 0, true, 0);
  
  result = Waitresponse();
  Serial.println(result);
  /*deliminate the extracted information*/
  STR.StoreKey(result);
  /*****************Information stored in the URL*************************/
  String ssid, pwd,api;
  uint8_t IP[3][4];
  uint16_t PORT[2], duration;
  /*****************Information stored in the URL*************************/
  ssid = STR.Delimitation('&').substring(5);
  pwd = STR.Delimitation('&').substring(5);
  api = STR.Delimitation('&').substring(5);
  duration = (uint16_t) converToInt(STR.Delimitation('&').substring(5));
  
  prom.Flash(1, IP[0], IP[1], IP[2], PORT[0],PORT[1], ssid, pwd, api, duration);  
  
  Serial.println("....................................");
  Serial.println(prom.readAPI());
  
  String tmp = "<h1>COMPLETE</h1>\r";
  msg = "AT+CIPSEND=";
  msg +=ChannelID;
  msg +=",";
  msg += tmp.length();
  msg += "\r";
  CommLaunch(msg, 1000, true, 0);
  CommLaunch(tmp, 5*1000, true, 0);
  tmp="";
  msg = "AT+CIPCLOSE=";
  msg +=ChannelID;
  msg +="\r";
  CommLaunch(msg, 1000, true, 0);
  return true;
}

String Waitresponse()
{
  String result="";
  int startP=0, endP=0;
  while(true)
  {
    CommResponse(1, 200); // Wait for the HTTP access
    if(STR.Storage.length()>0)
    {
      if(STR.Contains("GET /?"))
      {
        startP =STR.readPosition();
        if(STR.Contains("HTTP/"))
        {
          Serial.println(F("-----------------GET&&HTTP--------------------"));
          endP= STR.readPosition()-6;
          result = STR.Storage.substring(startP,endP);
          STR.Storage = "";
          break;
        }else
        {
          Serial.println(F("-----------------GET--------------------"));
         result = STR.Storage.substring(startP);
        }
      }else if(startP && STR.Contains("HTTP/"))
      {
        Serial.println(F("---------------------------HTTP----------------------"));
        endP= STR.readPosition()-6;
        result.concat(STR.Storage.substring(0,endP));
        STR.Storage = "";
        break;
      }
       STR.Storage="";
    }
  }
  
  return result;
}

int converToInt(String input)
{
   return input.toInt();
}

void Configuration()
{
  CommLaunch(F("AT+RST\r"), 2*1000, true, 0);
  CommLaunch(F("AT+CWMODE=2\r"), 1000, true, 0);
  CommLaunch(F("AT+CIPMUX=1\r"), 2*1000, true, 0);
  CommLaunch(F("AT+CIPSERVER=1,80\r"), 2*1000, true, 0);
  CommLaunch(F("AT+CWSAP=\"AI-THINKER\",\"1234567890\",5,4\r"), 5*1000, true, 0);
}

void CommLaunch(String cmd,unsigned int duration, boolean res, int keyword)
{
  if(cmd.equals("resp"))
    Serial1.println(resp);
    
  for(int Loop = 0; Loop <5 ; Loop ++)
  {
    STR.Storage = ""; /* clean every times after 1 iteration of for loop */
    Serial1.println(cmd);    //send command to ESP8266
  
   if(res)
    CommResponse( keyword, duration);

   if(keyword==2 && !STR.Contains("OK"))
     Loop ++;
   else
     Loop = 5; 
  }
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
      if(keyword==1 || keyword == 2)
      {
        STR.Storage += tmp; // if the keyword is triggered, copy the string
      }

      Serial.print(tmp);
      tmp = "";
    }
  }
}


float getTemperature(){
  Wire.requestFrom(tmp102Address,2); 

  byte MSB = Wire.read();
  byte LSB = Wire.read();

  //it's a 12bit int, using two's compliment for negative
  int TemperatureSum = ((MSB << 8) | LSB) >> 4; 

  float celsius = TemperatureSum*0.0625;
  return celsius;
}
