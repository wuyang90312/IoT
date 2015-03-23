#include "scheduler.h"

 SCHEDULER sch(5);

void setup()
{
  Serial.begin(9600);
  Serial.println("Start Setup");
  sch.addThread(50, &sample1);   
  sch.addThread(150,&sample2);
  sch.addThread(10, &sample3);
}

void loop()
{
  Serial.println("--------------->");
  sch.Display();
  Serial.println("<---------------");  
  delay(30*1000);
}

void sample1()
{
  Serial.println("sample  1");
}

void sample2()
{
  Serial.println("sample  2");
}

void sample3()
{
  Serial.println("sample  3");
}
