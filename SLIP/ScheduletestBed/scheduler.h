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

public:
  SCHEDULER(int threadNum);
  boolean addThread(int limit);//, void* func); 
  void Display();

};

#endif

