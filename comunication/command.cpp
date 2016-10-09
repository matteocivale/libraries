// command.cpp
// Library for command handling
// Credit to Matteo Civale (matteo.civale@gmail.com) for the original implementation


 #include "command.h"
 #include "board.h"
 
  systemBufType sysBuffer;

  byte preamble[PREAMBLE_LEN]={'#','#','#'};
  
  /* End definition of in coming message */
  byte endMessage='!';

 
  comState CheckNewCommand(systemBufType* myBuffer,SoftwareSerial *myCom)
  {
     static byte i=0;
     static byte messageLen=0;
     static bool recivPreamble=true;
     byte numberByte;
  
  
     numberByte=myCom->available();
  
     myBuffer->cmdBuffer[myBuffer->cmdPointer].command.message.len=0;
  
     if((i<PREAMBLE_LEN)&&numberByte)
     {

	   if(myCom->read()==preamble[i])
       {
          
		  i++;
          return COMMAND_IN_COMING;
       }
       else
       {  
          i=0;
          return COMMAND_ERROR;
       }  
     }
     else
      
       if((i==PREAMBLE_LEN)&&(numberByte))
       {
          messageLen=myCom->read();
          i++;
		  //Serial.print("ho letto il preambolo ");
          return COMMAND_IN_COMING;
       }
       else
       {
          if(numberByte>=(messageLen+1))
          {   
             
			 
			 i=0;
             myCom->readBytes(myBuffer->cmdBuffer[myBuffer->cmdPointer].command.rawData,messageLen+1);
             if (myBuffer->cmdBuffer[myBuffer->cmdPointer].command.rawData[messageLen]==endMessage)
             {
                //Serial.print("il messaggio è corretto");
				myBuffer->cmdBuffer[myBuffer->cmdPointer].command.message.len=messageLen;
                myBuffer->cmdBuffer[myBuffer->cmdPointer].noProcess=1;
                messageLen=0;
                myBuffer->cmdPointer++;
                myBuffer->cmdPointer%=CMD_BUFFER_LEN;
                if(myBuffer->cmdPointer==myBuffer->headQueue) return COMMANND_QUEUE_FULL;
                return COMMAND_NEW;
             }
             else
             {
                //Serial.print("il messaggio è sbagliato");
				return COMMAND_ERROR;
             }  
          }
       
        }
     return COMMAND_IDLE;      
      
  }


  byte printRecivedCommand(systemBufType* myBuffer)
  {
     byte i;
     // Serial.print(myBuffer->cmdPointer);
     // Serial.print(myBuffer->cmdBuffer[myBuffer->cmdPointer-1].noProcess);
	 
     for(i=0;i<myBuffer->cmdPointer;i++)
       if(myBuffer->cmdBuffer[i].noProcess)
       {
          Serial.print("Order: ");
          Serial.println(i);
          Serial.print("Id:");
          Serial.println(myBuffer->cmdBuffer[i].command.message.perId);
          Serial.print("Len:");
          Serial.print(myBuffer->cmdBuffer[i].command.message.len-1);
          Serial.print("\nParamiter:");
          for(byte j=0;j<(myBuffer->cmdBuffer[i].command.message.len-1);j++)
          {
             Serial.println(myBuffer->cmdBuffer[i].command.message.parameters[j]);
          }
          Serial.print("\n");
       }
 
     return i-1;
  }
  
  
  byte commandProcessor(systemBufType* myBuffer)
  {
    byte i=0,j;
	byte endCycle;
	while(myBuffer->cmdPointer!=myBuffer->headQueue)
    {
     
      switch(myBuffer->cmdBuffer[myBuffer->headQueue].command.message.perId)
      {
            case PERIPHERAL_BC_127:
    
            break;
    
            case PERIPHERAL_PETAL:
				
				/* Copy parameter in all petals */
				if(myBuffer->cmdBuffer[myBuffer->headQueue].command.message.parameters[0]==0xFF) 
				{
					j=0;
					endCycle=BOARD_INSTALLED_PETALS;
				}
				else
					j=endCycle=myBuffer->cmdBuffer[myBuffer->headQueue].command.message.parameters[0];
				   
				do 
				{			
				  for(i=0;i<PARAM_LEN;i++)
				  {
					  petals[j].lastCmdpParam[i]=
												myBuffer->cmdBuffer[myBuffer->headQueue].command.message.parameters[i+1];
				  }
				  petals[j].iWork=true;
				  petals[j].deltaT=0;
				  petals[j].lastTime=0;
				  j++;
				}while(j<endCycle);
			  
            break;
			
			case PERIPHERAL_BUTTON:
			  
			    byte app[3];
			    app[0]=myBuffer->cmdBuffer[myBuffer->headQueue].command.message.parameters[0];
			    app[1]=myBuffer->cmdBuffer[myBuffer->headQueue].command.message.parameters[1];
			    app[2]=myBuffer->cmdBuffer[myBuffer->headQueue].command.message.parameters[2];
			    setButtonColor(app);
			
			break;
			
			case PERIPHERAL_MEMORY:
			    memoryHandler(myBuffer->cmdBuffer[myBuffer->headQueue].command.rawData, myBuffer->cmdBuffer[myBuffer->headQueue].command.message.len);
			break;
			
			case PERIPHERAL_ANIMATION_MANAGER:
			
			    /* Animation Id */
			    sysReg.animInfo.animId = 
			     myBuffer->cmdBuffer[myBuffer->headQueue].command.message.parameters[0];
			
			    /* Animation Type */			
			    sysReg.animInfo.animType =
			     myBuffer->cmdBuffer[myBuffer->headQueue].command.message.parameters[1];
			    
				/* Start/Stop */
			    if(myBuffer->cmdBuffer[myBuffer->headQueue].command.message.parameters[2])
			     sysReg.animInfo.command.start = true;
		        else
				 sysReg.animInfo.command.stop = true;	
			 
			    /* Copy command parameter to animation memory */
				for(i=0; i< PARAM_LEN-3; i++)
					animation_memory[i] = myBuffer->cmdBuffer[myBuffer->headQueue].command.message.parameters[3 +i];
			 
			break;
      }
      myBuffer->headQueue++;
	  myBuffer->headQueue%=CMD_BUFFER_LEN;
      
    }
    return 0;
  }

  void resetBuffer(systemBufType* buffer)
  {
	  buffer->cmdPointer=0;
      buffer->headQueue=0;
  }
  
  void push_command(systemBufType* myBuffer, byte* command, byte len)
  {
	  byte i;
	  for (i=0; i<len; i++)
	  {
	     myBuffer->cmdBuffer[myBuffer->cmdPointer].command.rawData[i]=command[i];
	  }
	  myBuffer->cmdBuffer[myBuffer->cmdPointer].command.message.len=len;
	  myBuffer->cmdBuffer[myBuffer->cmdPointer].noProcess=1;
	  
	  /* Increment header pointer */
	  myBuffer->cmdPointer++;
	  myBuffer->cmdPointer%=CMD_BUFFER_LEN;
  }
  void pop_command(systemBufType* myBuffer)
  {}
  