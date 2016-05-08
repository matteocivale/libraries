// BC127.h
// Library for controlling BC127 of BlueCreation
// Credit to Matteo Civale(matteo.civale@gmail.com) for the original implementation


#ifndef __BC127_H
#define __BC127_H

  #include "Arduino.h"
  #include "SoftwareSerial.h"
  


 /*
  *                                 Constant definition
  */

  #define BT_RX 8
  #define BT_TX 7
  #define BC127_COM_BAUD 9600
  #define BC127_RESET_PIN 3
  
  
 
 
 #define RESPONS_BUFFER_LEN 5
 
  /* Serial port for bt comunication */
  byte BC127_ComStart(void);

 /*
  *                    FUNCTION INTERFACE
  */
  
  
 
   
  /*
   *@ Return the serial pointer of BT comunication
   */
   SoftwareSerial *BC127_GetCom(void);
  
  /*
   *@ Turn device in BLE mode
   *@ Return 0 if no error occured 1 otherwise
   */
 
   byte BC127_BleON(void);
   
  /*
   *@ Return true if BC127 is in BLE mode, false otherwise
   */   
   
   bool BC127_IsBleOn(void);
   
   /*
    *@ Return true if the BC127 return "OK"	
	*/
   bool BC127_sendCmd(char *command, byte cmdlen);
  
  #endif
  