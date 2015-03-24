/**
 * @file    scheduler.cpp
 * @brief   Configure a scheduler to manage multiple threads
 * @author  Yang Wu
 * @version 1.0
 * @see     
 *
 * Copyright (c) 2015
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "scheduler.h"

SCHEDULER::SCHEDULER(int threadNum, int duration)
{
  offset = 1;
  Duration = duration;
  MaxPosition = threadNum; 
  currentPosition = -1;
  TimeLimit = new int[MaxPosition];
  CurrentTime = new int[MaxPosition];
  Address = new ptrFunction[MaxPosition];
}

boolean SCHEDULER::addThread(const int limit, void (*func)(void))
{
  currentPosition ++; /* Once if the new thread is added, the position moves to the nxt position */
  
  if(currentPosition >= MaxPosition)
  {
    Serial.println("[ERROR] Out of index ");
    return false; 
  }
  
  TimeLimit[currentPosition] = limit;
  CurrentTime[currentPosition] = limit;
  Address[currentPosition] = func; /* Store the function ptr to the array */
  //func();
  return true;
}

void SCHEDULER::Display() /* Only used for the debugging purpose */
{
  for(int i=0; i<=currentPosition; i++)
  {
     Serial.println(TimeLimit[i]);
     //Serial.println((int)Address[i]); /* Print the address of the function */
     Address[i](); /* Call functions in the array */
  }
}

void SCHEDULER::RoundRobin()
{
  int diff;
  long deadline = millis()+Duration;
  

    if(!stack.isEmpty())
    {
      (stack.pop())();
    }
     
    for(int i=0; i<=currentPosition-1; i++) 
    {
       CurrentTime[i]-=offset;
       
       if(CurrentTime[i] <= 1){
         stack.push(Address[i]);
       }
       
       if(CurrentTime[i] <= 0){
         CurrentTime[i] = TimeLimit[i];
       } 
    }
   
   /* keep the last index as highest priority so that the last index keeps running until meet a stack*/
   if(stack.isEmpty())
   {
     while(millis()<=deadline)
     {
       Address[currentPosition]();
     }
     return;
   }
   
   diff = deadline-millis();
   if(diff>0)
   {
     offset = 1;
     delay(diff); 
   }
   else
   {
     offset = 1 - (int)(diff/Duration);
   }
  
}



