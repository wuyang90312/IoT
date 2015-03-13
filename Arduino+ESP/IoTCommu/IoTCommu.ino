#include <SoftwareSerial.h>

//SoftwareSerial mySerial(11, 12); //RX,TX

void setup()
{
  Serial.begin(115200);    // Arduino serial Monitor
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial1.begin(57600);   // The ESP8826 has a baud rate of 115200

  // Enable esp8266 
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

 // CommLaunch("AT\r", 1000, true);
  CommLaunch("AT+RST\r", 4*1000, true);
 // CommLaunch("AT+CIOBAUD=57600\r", 4*1000, true);
  delay(5*1000);
 // Serial1.begin(57600);   // The ESP8826 has a baud rate of 115200
 // CommLaunch("AT+RST\r", 4*1000, true);
  
  CommLaunch("AT+CWMODE=3\r", 1000, true);
  CommLaunch("AT+CIFSR\r", 4*1000, true);
  CommLaunch("AT+CWLAP\r", 4*1000, true);

}

void loop() {
  // Serial.println("Hello");
   
}

boolean CommLaunch(String cmd,unsigned int duration, boolean res)
{
  Serial1.println(cmd);    //send command to ESP8266
  //mySerial.flush();  //wait for the command finshes the sending
  /* Display the Command on the Moniter */
    Serial.println(cmd);
  if(res)
    CommResponse("sth", duration);
}

void setUp()
{
  
}

void CommResponse(String keyword, unsigned int duration)
{
  String tmp ="";
  long deadline = millis()+duration;
  while(millis() < deadline)
  {
    while (Serial1.available()>0)  {
      //tmp += char(mySerial.read());
      //delay(15);
      Serial.print(char(Serial1.read()));
    }

    //if(tmp.length() > 0) {
    //  Serial.print(tmp);
    //  tmp = "";
    //}
  }
  Serial.println("\n------------------------------");
}

