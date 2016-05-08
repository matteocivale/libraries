/*
 * BC127.cpp
 * Library for controlling BC127 of BlueCreation
 * Credit to Matteo Civale(matteo.civale@gmail.com) for the original implementation
*/

  #include "BC127.h"
  

 

/********************************************bluetooth function **********************************************/

  SoftwareSerial BC127Com(BT_RX,BT_TX); // RX, TX
  bool isBleOn=false;
 
  byte btBuffer[RESPONS_BUFFER_LEN];


  byte BC127_ComStart(void)
  {
	  // pinMode(BC127_RESET_PIN,OUTPUT);
	  // digitalWrite(BC127_RESET_PIN, LOW);
	  // delay(50);
	  // digitalWrite(BC127_RESET_PIN, HIGH);
	  // delay(1000);
	  BC127Com.begin(BC127_COM_BAUD);
	  return 0;
  }
  
  SoftwareSerial *BC127_GetCom(void)
  {
    return &BC127Com;
  }
  
  

  byte BC127_BleON(void)
  {
    
	
	BC127Com.flush();
    BC127Com.println("ADVERTISING ON");
    BC127Com.print("\r");
    while(BC127Com.available()<2){;};
    BC127Com.readBytes(btBuffer,2);
    if((btBuffer[0]=='O')&&(btBuffer[1]=='K')) 
	{
      isBleOn=true;
	  return 0;
    }
	else
      return 1;
  }
  
  bool BC127_IsBleOn(void)
  {
	  return isBleOn;
  }
  
  bool BC127_sendCmd(char *command, byte cmdlen)
  {
	BC127Com.flush();
    BC127Com.write(command,cmdlen);
    BC127Com.print("\r");
    while(BC127Com.available()<2){;};
    BC127Com.readBytes(btBuffer,2);
    if((btBuffer[0]=='O')&&(btBuffer[1]=='K')) 
	{
      return true;
    }
	else
      return false;
  }


