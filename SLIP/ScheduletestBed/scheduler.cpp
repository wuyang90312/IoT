/**
 * @file    FP.cpp
 * @brief   Core Utility - Templated Function Pointer Class
 * @author  sam grove
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

SCHEDULER::SCHEDULER(int threadNum)
{
  MaxPosition = threadNum; 
  currentPosition = -1;
  TimeLimit = new int[MaxPosition];
  CurrentTime = new int[MaxPosition];
}

boolean SCHEDULER::addThread(int limit)//, void* func)
{
  currentPosition ++; /* Once if the new thread is added, the position moves to the nxt position */
  
  if(currentPosition >= MaxPosition)
  {
    Serial.println("[ERROR] Out of index ");
    return false; 
  }
  
  TimeLimit[currentPosition] = limit;
  CurrentTime[currentPosition] = limit;
  
  return true;
}

void SCHEDULER::Display()
{
  for(int i=0; i<=currentPosition; i++)
  {
     Serial.println(TimeLimit[i]);
  }
}



