#include <EEPROM.h>
#include "PROM.h"
#include "StringModule.h"

/* 1 KB (ATmega32u4)*/
String resp;
String Command;  /* Keep track of the command */
int    Decision; /* Decide which kind of behavior of ESP should have */
int   Count;

int dummyData = 40;
const String  API_KEY = "GPVP0E6QQVWU47LZ";
StringModule STR("");

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
  UIweb();
 // ESPWIFIsetup(); /* set up the configuration of ESP chip */
 // ESPWIFIassociate();
 // ESPTCPconnection();
  
}

void loop()
{
}

void UIweb()
{
  char ChannelID; /*Channel is in scale of 0-6, No need to worry about more than 2 chars */
  Configuration();
  
  while(true)
  {
    CommResponse(1, 100); // Wait for the HTTP access
    if(STR.Storage.length()>0)
    {
    //  STR.ReplaceBase(Storage);
      if(STR.Contains("+IPD,"))
      {
        Serial.println("---------------CHECK----------------");
        ChannelID = STR.Storage.charAt(STR.readPosition()); 
        STR.Storage = ""; // Clear the memory for nxt time
        CommResponse(0, 5000); // Wait for HTTP response to finish
        uploadUI(ChannelID);
        break;
      }
    }

    STR.Storage = "";// Clear the memory for nxt time
  }
}

boolean uploadUI(char ID)
{
  
  String msg = "AT+CIPSEND=";
  resp = "<h1> ESP8266 Web Server</h1>\n";
  resp +="<form  method=\"get\">";
  resp += "<p>SSID &nbsp; <input type=\"text\" name=\"SSID\" value=\"SSID\"><br>";
  resp += "PWD &nbsp; <input type=\"text\" name=\"PWD\" value=\"PASSWORD\"><br>";
  resp += "STA IP &nbsp; <input type=\"text\" name=\"STAIP\" value=\"STA IP\"><br>";
  resp += "MQTT IP &nbsp; <input type=\"text\" name=\"MQTTIP\" value=\"MQTT IP\"><br>";
  resp += "CLOUD IP &nbsp; <input type=\"text\" name=\"CLOUDIP\" value=\"CLOUD IP\"><br></p>";
  resp += "<a type=\"submit\" value=\"submit\"><button>SUBMIT</button></a>";
  resp += "</form>\r";
  msg +=ID;
  msg += ",";
  msg += resp.length();
  msg +="\r";
 
  CommLaunch(msg, 2*1000, true, 0);
  CommLaunch("resp", 4*1000, true, 0);
  msg = "AT+CIPCLOSE=";
  msg +=ID;
  msg +="\r";
  CommLaunch(msg, 0, true, 0);
  
  return Waitresponse();
}

boolean Waitresponse()
{
  String result;
  int startP=0, endP=0;
  while(true)
  {
    CommResponse(1, 100); // Wait for the HTTP access
    if(STR.Storage.length()>0)
    {
      
       Serial.println(">>>>>>");
      Serial.println(STR.Storage);
      Serial.println(">>>>>>");
      
      Serial.println(startP);
      Serial.println(endP);
      Serial.println("<<<<<<<");
      if(STR.Contains("GET"))
      {
        Serial.println("---------------GET----------------");
        startP =STR.readPosition()+3;
        Serial.println(startP);
        if(STR.Contains("HTTP"))
        {
          Serial.println("---------------HTTP----------------");
          endP= STR.readPosition()-5;
          result = STR.Storage.substring(startP,endP);
          STR.Storage = "";
          break;
        }else
        {
         result = STR.Storage.substring(startP);
         Serial.println(result); 
        }
      }else if(startP && STR.Contains("HTTP"))
      {
        Serial.println("---------------HTTP----------------");
        endP= STR.readPosition()-5;
         Serial.println(endP);
        result += STR.Storage.substring(0,endP);
        STR.Storage = "";
        break;
      }
       STR.Storage="";
    }
  }
  Serial.println("---------------RESULT----------------");
  Serial.println(result);
  
  return true;
}

void Configuration()
{
  CommLaunch("AT+RST\r", 2*1000, true, 0);
  CommLaunch("AT+CWMODE=2\r", 1000, true, 0);
  CommLaunch("AT+CIPMUX=1\r", 2*1000, true, 0);
  CommLaunch("AT+CIPSERVER=1,80\r", 2*1000, true, 0);
  CommLaunch("AT+CWSAP=\"AI-THINKER\",\"\",5,0\r", 5*1000, true, 0);
}

void ESPWIFIsetup()
{
  CommLaunch("AT+CIOBAUD?\r", 2*1000, true, 0);
  CommLaunch("AT+RST\r", 2*1000, true, 0);
  CommLaunch("AT+CWMODE=1\r", 1000, true, 0);
  CommLaunch("AT+CIFSR\r", 2*1000, true, 0);
  CommLaunch("AT+CWLAP\r", 6*1000, true, 1);
}

void ESPWIFIassociate(String usr, String pw)
{
  String asso;
  
 // STR.ReplaceBase(Storage);
 // Storage = ""; // Clear the memory for nxt time

  if(STR.Contains(usr))
  {
    asso = "AT+CWJAP=\""+usr+"\",\""+pw+"\"";
    while(true)
    {
      CommLaunch(asso, 15*1000, true, 1);
      
      if(ESPCommandSucess("OK",1))
        break;
    }
  }
  else
    Serial.println("[ERROR0] : no matching result!!");
    
  Serial.println("FINISH ASSOCIATION!!");
  CommLaunch("AT+CIFSR\r", 2*1000, true, 0); 
}

void ESPTCPconnection()
{
  String URL;
  
  URL = "AT+CIPSTART=\"TCP\",\"184.106.153.149\",80";
  
  while(true)
  {
    CommLaunch(URL, 15*1000, true, 1);
    
    if(ESPCommandSucess("OK",1))
        break;
  }
  
  Serial.println("FINISH TCP CONNECTION!!");
  CommLaunch("AT+CIPSTATUS\r", 2*1000, true, 0); 
}

void ESPTCPsend(int length)
{
  String message = "AT+CIPSEND="; 
  message+=length;
  CommLaunch(message, 2*1000, true, 0); 
}

void ESPUpload(int input)
{
  String content;
  int SIZE;
  content = "GET /update?key=";
  content += API_KEY;
  content += "&field1=";
  content += input;
  SIZE = content.length()+1;
  
  ESPTCPsend(SIZE);
  CommLaunch(content, 15*1000, true, 0); 
  
  ESPTCPcheck();
}

void ESPTCPcheck()
{
  while(true)
  {
    CommLaunch("AT+CIPCLOSE", 5*1000, true, 1);
    if(ESPCommandSucess("OK+ERROR",2))
        break;
  }
  ESPTCPconnection();
}

void ESPDownload()
{
  String content;
  int SIZE;
  content = "GET /channels/26355/feeds.json?results=1\r\n";
  SIZE = content.length()-1;

  ESPTCPsend(SIZE);
  CommLaunch(content, 15*1000, true, 0); 
  
  ESPTCPcheck(); 
}

void ESPFinish()
{
    CommLaunch("AT+CIPCLOSE", 6*1000, true, 0);
    CommLaunch("AT+CWQAP", 2*1000, true, 0);
}

void CommLaunch(String cmd,unsigned int duration, boolean res, int keyword)
{
  if(cmd.equals("resp"))
    Serial1.println(resp);
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
        STR.Storage += tmp; // if the keyword is triggered, copy the string

      Serial.print(tmp);
      tmp = "";
    }
  }
}

boolean ESPCommandSucess(String KEY, int index)
{
  int iteration;
 // STR.ReplaceBase(Storage);
//  Storage = "";
  
  if(index==1)
    return STR.Contains(KEY);
  
  boolean result = false;
  String  component;
  STR.StoreKey(KEY);
  for(iteration = 0; iteration < index; iteration++)
  {
    component = STR.Delimitation('+');

    result = result || STR.Contains(component);
  }
  
  if(STR.Contains("busy"))
  {
    Serial.println("ENTER REST");
    delay(1000);
    Serial.println("EXIT REST");  
}
  
  return result;
}
