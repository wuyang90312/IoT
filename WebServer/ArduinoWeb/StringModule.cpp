#include <Arduino.h>
#include "StringModule.h"

//String s;
//String key;
//int END_POSITION=0;

StringModule::StringModule(String base)
{
  Storage = base;
}

boolean StringModule::Contains(String search) {
  int endP = search.length();
  int maxP = Storage.length() - endP;

  for (int i = 0; i <= maxP; i++) {
    if (search.equalsIgnoreCase(Storage.substring(i, endP)))
    {
      END_POSITION = endP;
      Serial.println();
      Serial.println(Storage.substring(i, endP));
      return true;
    }

    endP++;
  }

  return false;
}

int StringModule::readPosition()
{
   return END_POSITION; 
}

/*void StringModule::ReplaceBase(String base)
{
  s = base;
  END_POSITION=0;
  Serial.println(s);
}*/

void StringModule::StoreKey(String input) /* Store the string need to be delimited */
{
  key=input;
}

String StringModule::Delimitation(char delimiter)
{
  int length = key.length();
  
  for(int i =0; i< length; i++)
  {
    if(delimiter == key.charAt(i))
    {
        String sth = key.substring(0,i);
        key = key.substring(i+1);
        return sth;
    }
  }
  
  return key;
}
