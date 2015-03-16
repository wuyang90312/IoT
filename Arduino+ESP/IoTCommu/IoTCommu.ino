#include <SoftwareSerial.h>
#include "StringModule.h"

String Storage;  /* Store the important information from ESP reponse */
String Command;  /* Keep track of the command */
int    Decision; /* Decide which kind of behavior of ESP should have */
int   Count;

int dummyData = 0;
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
  
  /* Define the global variables */
  Storage = "";
  Command = "";
  Decision = -1;
  Count=0;
  
  ESPWIFIsetup(); /* set up the configuration of ESP chip */
  ESPWIFIassociate();
  ESPTCPconnection();

}

void loop() {
    char character;
    if (Serial.available())
    {
      character = Serial.read();
      Command += character;
    }
    else if(Command.length())
    {
      Serial.println(Command);
      if(Command.equals("FINISH"))
      {
         ESPFinish(); /* Break the TCP and association*/
      }else if(Command.equals("UPLOAD"))
      {
         Decision = 0;
      }else if(Command.equals("DOWNLOAD"))
      {
         Decision = 1;
      }else if(Command.equals("PAUSE"))
      {
         Decision = -1;
      }else{
         Serial.println("[ERROR0] : no matching command!!");
      }
      Command =""; /* Clean the memory of the command */
    }
    else
    {
      switch(Decision)
      {
       case 0:
         if(Count==0)
         {
           dummyData +=5;
           ESPUpload(dummyData);
         }
         else
           Count = (Count+1)%60;
         delay(1000);
         break;
       case 1:
         Decision = -1;
         ESPDownload();
         break;
       default:
         delay(1000);
         break;
      } 
    }
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
  String usr, pw, asso;
  usr = "QoS";
  pw = "23456";
  
  STR.ReplaceBase(Storage);
  Storage = ""; // Clear the memory for nxt time

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
    CommLaunch("AT+CIPCLOSE", 2*1000, true, 0);
    CommLaunch("AT+CWQAP", 2*1000, true, 0);
}

void CommLaunch(String cmd,unsigned int duration, boolean res, int keyword)
{
  Serial1.println(cmd);    //send command to ESP8266

  if(res)
    CommResponse( keyword, duration);
}

void CommResponse(int keyword, unsigned int duration)
{
  Serial.println("ESP RESPONSE:");
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

boolean ESPCommandSucess(String KEY, int index)
{
  int iteration;
  STR.ReplaceBase(Storage);
  Storage = "";
  
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
  
  return result;
}

