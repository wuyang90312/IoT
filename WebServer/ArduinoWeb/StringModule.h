#ifndef STRINGMODULE_h
#define STRINGMODULE_h

#include <Arduino.h>

class StringModule
{
  public:
          String Storage;
	  StringModule(String base);
	  boolean Contains(String search);
          void ReplaceBase(String base);
          void StoreKey(String input);
          String Delimitation(char delimiter);
          int readPosition();
  private:
          String key;
          int END_POSITION;
};

#endif
