/**
 * @file    scheduler.h
 * @brief   Configure a scheduler to manage multiple threads
 * @author  Yang Wu
 * @version 1.0
 * @see     
 *
 * Copyright (c) 2015
 */


#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>
#include <StackList.h>

class SCHEDULER {

private:
  int offset;
  int Duration;
  int MaxPosition;
  int currentPosition;
  int *TimeLimit;
  int *CurrentTime;
  typedef void (*ptrFunction)(void); /* Define a type of the function */
  ptrFunction *Address;
  StackList<ptrFunction> stack; /* Every time store the function ptr in a order of priority */
  
public:
  SCHEDULER(int threadNum, int duration);
  boolean addThread(int limit, void (*func)(void)); 
  void Display();
  void RoundRobin();

};

#endif

