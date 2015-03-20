#ifndef STRINGMODULE_h
#define STRINGMODULE_h

#include <Arduino.h>

class StringModule
{
  public:
	  StringModule(String base);
	  boolean Contains(String search);
          void ReplaceBase(String base);
          void StoreKey(String input);
          String Delimitation(char delimiter);
  private:
	  String s;
          String key;
};

#endif
