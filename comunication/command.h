// command.h
// Library for command handling
// Credit to Matteo Civale(matteo.civale@gmail.com) for the original implementation

#ifndef __COMMAND_H
  #define __COMMAND_H
  
  #include "Arduino.h"
  #include "SoftwareSerial.h"

 /*
  *                                 Constant definition
  */


  /* Preamble definition of in coming message */
  #define PREAMBLE_LEN 3
  
  /* Definition of received command function */

  typedef enum
  {
    COMMAND_ERROR,
    COMMAND_IN_COMING,
    COMMAND_NEW,
    COMMAND_IDLE,
    COMMANND_QUEUE_FULL,
  }comState; 
  
  /* Received command structure definition */
  #define PARAM_LEN 20

  typedef union
  {
     struct
     {
       byte perId;
       byte parameters[PARAM_LEN];
       byte len;
     }message;
  
     byte rawData[PARAM_LEN+1];

  }comType;
  
  
  /* Command storage queue definition */
  typedef struct 
  {
    bool noProcess;
    comType command;
  
  }cmdBufType;
  
  
  /* Buffer type definition */
  
  #define CMD_BUFFER_LEN 5 //Len of command buffer
  
  typedef struct
  {
    cmdBufType cmdBuffer[CMD_BUFFER_LEN];
    byte cmdPointer;
    byte headQueue;
  
  }systemBufType;
  
  
  
  /* Peripheral id  Type*/
  typedef enum
  {
    PERIPHERAL_BC_127 =0x01,
    PERIPHERAL_PETAL  =0x02,
	PERIPHERAL_BUTTON =0x03,
	PERIPHERAL_MEMORY =0x04,
  
  }peripheralIdType;
  
  
  /* Definition of system commands buffer */
  extern systemBufType sysBuffer;
  
  /*
   *
   *                                Function Interface
   *
   */
  
  comState CheckNewCommand(systemBufType* myBuffer,SoftwareSerial *myCom);
  
  byte printRecivedCommand(systemBufType* myBuffer);
  
  byte commandProcessor(systemBufType* myBuffer);
  
  void resetBuffer(systemBufType* buffer);
  void push_command(systemBufType* myBuffer, byte* command, byte len);
  void pop_command(systemBufType* myBuffer);

  
  
#endif