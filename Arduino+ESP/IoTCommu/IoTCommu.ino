#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 12); //RX,TX

void setup()
{
  Serial.begin(9600);    // Arduino serial Monitor
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  mySerial.begin(9600);   // The ESP8826 has a baud rate of 115200

    // Enable esp8266 
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  CommLaunch("AT\r", 1000, true);
  CommLaunch("AT+RST\r", 4*1000, true);
  CommLaunch("AT+CIOBAUD?\r", 4*1000, true);
  
  CommLaunch("AT+CWMODE=3\r", 1000, true);
  CommLaunch("AT+CIFSR\r", 4*1000, true);
  CommLaunch("AT+CWLAP\r", 4*1000, true);
  
  
 /* String tmp;
  
  tmp="The Seeed tutorial is writen for the version 1 module with a 57600 baud\n";
  tmp+= "rate serial link. The version 2 module that is currently shipping runs\n";
  tmp+="at 115200 bps Also, the version 2 module requires 5 connections rather\n";
  tmp+="than the 4 shown in the tutorial. You need to pull the chip-enable (CH_PD)\n";
  tmp+="pin high - by connecting it to a 3.3V source via a (say) 10k resistor.\n";
  
  Serial.print(tmp);*/
}

void loop() {
  // Serial.println("Hello");
   
}

boolean CommLaunch(String cmd,unsigned int duration, boolean res)
{
  mySerial.println(cmd);    //send command to ESP8266
  //mySerial.flush();  //wait for the command finshes the sending
  /* Display the Command on the Moniter */
    Serial.println(cmd);
  if(res)
    CommResponse("sth", duration);
}

void CommResponse(String keyword, unsigned int duration)
{
  String tmp ="";
  long deadline = millis()+duration;
  while(millis() < deadline)
  {
    while (mySerial.available()>0)  {
      //tmp += char(mySerial.read());
      //delay(15);
      Serial.print(char(mySerial.read()));
    }

    //if(tmp.length() > 0) {
    //  Serial.print(tmp);
    //  tmp = "";
    //}
  }
  Serial.println("\n------------------------------");
}

