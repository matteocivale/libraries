#include "EPROM24LC256.h" 

 
 
/* FUNCTION IMPLEMENTATION */
  
  
  
   
  
  
  
  
  
  //---------------------------------------------------------FUNZIONI DI SCRITTURA INSERITE DA MATTEO-------------------------------------------------------------------------
    void EpromWriteString(int deviceaddress, unsigned int eprom_address, unsigned char * string, unsigned int string_len)
	{

      unsigned int page_num=(unsigned int)eprom_address/EPROM_PAG_SIZE;
      unsigned int pag_offset=(unsigned int)eprom_address%EPROM_PAG_SIZE;
      unsigned short int byte_cont=0;

      Wire.beginTransmission(deviceaddress);
      Wire.write((int)(eprom_address >> 8));   // MSB
      Wire.write((int)(eprom_address & 0xFF)); // LSB
      Wire.write(string[0]);
      pag_offset++;
      byte_cont=3;
      for (unsigned int i=1;i<string_len;i++)
	  {
        if ((pag_offset==EPROM_PAG_SIZE)||(byte_cont==BUFFER_WIRE_LEN))
		{
                                                    if(pag_offset==EPROM_PAG_SIZE)
													{
                                                       pag_offset=0;
                                                       page_num++;
                                                    }
                          
                                                    Wire.endTransmission();
                                                    delay(5);
                                                    Wire.beginTransmission(deviceaddress);
                                                    Wire.write((int)((page_num*EPROM_PAG_SIZE+pag_offset) >> 8));   // MSB
                                                    Wire.write((int)((page_num*EPROM_PAG_SIZE+pag_offset) & 0xFF)); // LSB
                                                    byte_cont=2;
                                                    
        }
      Wire.write((byte)string[i]);
      pag_offset++;
      byte_cont++;
    }

    Wire.endTransmission();
    delay(5);
  }
  
  
  byte readEEPROM(int deviceaddress, unsigned int eeaddress )
  {
    byte rdata = 0xFF;
   
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8));   // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
   
    if (Wire.available()) rdata = Wire.read();
   
    return rdata;
  }
  
  
  
  
  void EpromReadString(byte* string,int deviceAddress, unsigned int startingEepromAddress,unsigned int memorySize)
  {
     
    for(int i=0;i<memorySize;i++)
	{
      string[i]=(byte)readEEPROM(deviceAddress,startingEepromAddress+i); 
    }
   
  } 
   
  // void load_stored_command(int device_address, unsigned int memory_location,unsigned char* command){
  // for (int i=0;i<MEMORY_CMD_LENGHT;i++){
  // command[i]=(unsigned char)readEEPROM(device_address,memory_location+i);
  // }
  // }

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data )
{
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8));   // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(data);
    Wire.endTransmission();
   
    delay(5);
}
  