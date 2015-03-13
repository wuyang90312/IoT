#ifndef STRINGMODULE_h
#define STRINGMODULE_h

#include "Arduino.h"

class StringModule
{
  public:
	  StringModule(String base);
	  boolean Contains(String search);
          void ReplaceBase(String base);
  private:
	  String s;
};

#endif
