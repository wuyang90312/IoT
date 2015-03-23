#include <StackList.h>
#include "scheduler.h"

SCHEDULER sch(5, 500);


void setup()
{
  Serial.begin(9600);

  sch.addThread(50, &sample1);   
  sch.addThread(150,&sample2);
  sch.addThread(10, &sample3);
}

void loop()
{
  Serial.println("--------------->");
  sch.RoundRobin();
  Serial.println("<---------------");  
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
