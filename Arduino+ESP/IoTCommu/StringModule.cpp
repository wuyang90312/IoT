#include "Arduino.h"
#include "StringModule.h"

String s;

StringModule::StringModule(String base)
{
  s = base;
  base="";
}

boolean StringModule::Contains(String search) {
  int end = search.length();
  int max = s.length() - end;

  for (int i = 0; i <= max; i++) {
    if (search.equals(s.substring(i, end)))
      return true;

    end++;
  }

  return false;
}

void StringModule::ReplaceBase(String base)
{
  s = base;
  base="";
  Serial.println(s);
}
