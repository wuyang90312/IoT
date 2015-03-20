#include "scheduler.h"

 SCHEDULER sch(5);

void setup()
{
  Serial.begin(9600);
  Serial.println("Start Setup");
  sch.addThread(50);   
  sch.addThread(150);
  sch.addThread(10);
}

void loop()
{
  Serial.println("--------------->");
  sch.Display();
  sch.addThread(21);
  Serial.println("<---------------");  
  delay(30*1000);
}
