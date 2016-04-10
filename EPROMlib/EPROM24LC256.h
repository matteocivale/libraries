#ifndef __24LC256_H
#define __24LC256_H


#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <thePins_arduino.h>
#endif

#include "Wire.h"

#define BUFFER_WIRE_LEN 32  //bytes length of read/write function of wire library 
#define EPROM_PAG_SIZE  64  //bytes length of eprom page size 

/*                     FUNCTION DEFINITION                */
 
 void EpromWriteString(int deviceaddress, unsigned int eprom_address, unsigned char * string, unsigned int string_len);
 
 void EpromReadString(int deviceAddress, unsigned int startingEepromAddress, byte* string, unsigned int memorySize);
 
 void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data );
 byte readEEPROM(int deviceaddress, unsigned int eeaddress );
 
 
#endif