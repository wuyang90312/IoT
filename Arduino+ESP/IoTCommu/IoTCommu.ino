#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 12); //RX,TX

String tmp; 

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

}

void loop() {
  // Serial.println("Hello");
  CommLaunch("AT+CWMODE=1\r", 4000, true);
  CommLaunch("AT+CIFSR\r", 6*1000, true);

}

boolean CommLaunch(String cmd,unsigned int duration, boolean res)
{
  mySerial.println(cmd);    //send command to ESP8266

  /* Display the Command on the Moniter */
  Serial.print("CMD: ");
  Serial.println(cmd);

  if(res)
    CommResponse("sth", duration);
}

void CommResponse(String keyword, unsigned int duration)
{
  int record = 0;
  String response ="";
  Serial.println("RES: ");

  long deadline = millis()+duration;
  while(millis() < deadline)
  {
    while (mySerial.available() > 0)  {
      tmp += char(mySerial.read());
      delay(2);
    }

    if(tmp.length() > 0) {
      Serial.println(tmp);
      tmp = "";
    }

    if (Serial.available()) {
      mySerial.write(Serial.read());
    }
  }

  /* while (mySerial.available() > 0)  {
   response += char(mySerial.read());
   delay(15);
   }
   Serial.println(response);
   delay(duration);*/
}

