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

#include <StackList.h>
#include <Arduino.h>

class SCHEDULER {

private:
  int MaxPosition;
  int currentPosition;
  int *TimeLimit;
  int *CurrentTime;
  typedef void (*ptrFunction)(void); /* Define a type of the function */
  ptrFunction *Address;

public:
  SCHEDULER(int threadNum);
  boolean addThread(int limit, void (*func)(void)); 
  void Display();

};

#endif

