

  #include "board.h"
  //#include "Adafruit_CAP1188.h"
  
  byte zeros[]={0,0,0};
  
  /* msgeq7 definition*/
  
  SysRegType sysReg;
  
  MSGEQ7 msgeq7;
  
  buttonType button={{0,0,0}, false, false, 0, 0, 0, 0, 0, Adafruit_CAP1188(CAP1188_RESET)};
 // button.CAP1188=Adafruit_CAP1188(CAP1188_RESET);
  petalType petals[BOARD_MAX_PETAL_NUM];
  
  memoryDescriptorType memoryDesc;
  
  

  
  void boardInit(void)
  {
	 byte i;
	 
	 /* Initialize button */
	 buttonInit();
	 
	 /* Inizialize MSGEQ7 */
     msgeq7.init(MSGEQT_PIN_OUTPUT, MSGEQT_PIN_STROBE, MSGEQT_PIN_RESET);

	 
     /* Reset all petals */	
     for(i=1; i<= 0x07; i++ )	 
	 {
        Inizilize_PCA9633(i<<2);
		Set_Selected_PCA9633_AllOut(i<<2, zeros);
		petals[i-1].address=(i<<2);
		delay(1);
	 }
	 
	 /* Enable Amplify */
	 pinMode(AMPLIFY_SD,OUTPUT);
	 digitalWrite(AMPLIFY_SD,HIGH);
	 
	 /* Test EPROM */
	 // EpromWriteString(EPROM_BANK0_ADDRESS,0,(unsigned char*)str, sizeof(str));
     // EpromReadString((byte*)buf,EPROM_BANK0_ADDRESS, 0,sizeof(str));
	 
	
 
	 /* Initialize system reg status */
	 sysReg={SYSTEM_BLE_OFF, SL_STARTUP, SYSTEM_COMMAND_MODE};
     
  }
  
  void setButtonColor(byte *color)
  {
	  
	 button.color[0]=color[0];
	 button.color[1]=color[1];
	 button.color[2]=color[2];
	 button.writeColor=true;
  }
  
  
  
  byte buttonMonitor(void)
  {
     static long int HTime=0;
	 static long int LTime=0;
	 static byte tapN=0;
	 static byte ltapN=0;
	 static byte sltapN=0;
	 static bool dvalid=false;
	 static float ramp;
	 static byte sign=0;
	 //static float rise =255/
	 
	 long int actTime=millis();
	 button.deltaT=actTime-button.time;
	 
	 if(button.writeColor)
	 {	  
	    
		if(!sign)
		{
		  ramp+=button.deltaT*0.005;
		  if(ramp>1)
		  {
			 ramp=1;
			 sign=1;
		  }
		}
		else
		{
			ramp-=button.deltaT*0.0008;
			if(ramp<0) 
			{
				sign=0;
				ramp=0;
		    }
		}
		//Serial.println(ramp);
		analogWrite(BOARD_BUTTON_REDLED_PIN, ramp*button.color[0]);
        analogWrite(BOARD_BUTTON_GREENLED_PIN,ramp*button.color[1]);
        analogWrite(BOARD_BUTTON_BLUELED_PIN, ramp*button.color[2]); 
	    //button.writeColor=false;
		
	 }
	 /* Check the touch state  */
	 
	 
	 if((button.CAP1188.touched()&0x01)) 
	 {
		 HTime+=button.deltaT;
		 LTime=0;
		 dvalid=false;
		 //Serial.println(deltaT);
		 if(HTime>10000)
		 { 
	        Serial.println(tapN);
		    Serial.println(ltapN);
			Serial.println(sltapN);
			HTime=0;
		 }
	 }
	 else
	 {
		 if((HTime>BUTTON_TIME_SHORT)&&(HTime<BUTTON_TIME_LONG))
		 {
			 tapN++;
			// Serial.println("one tap");
	     }
		 else
	    	 if((HTime>BUTTON_TIME_LONG)&&(HTime<BUTTON_TIME_SLONG))
		     {
			    ltapN++;
				//Serial.println("one ltap");
		     }			 
		     else
				 if(HTime>BUTTON_TIME_SLONG)
				 {
                    sltapN++;			 
				   // Serial.println("long sltap");
				 }
		 LTime+=button.deltaT;
		 if(LTime>500)//900 ok
		 {
			// tapN=0;
			// ltapN=0;
			// sltapN=0;
			 dvalid=true;
		 }
		 HTime=0;
	 }
	 
	 button.time=actTime;
	 
	 if(dvalid)
	 {
		 button.ntap=tapN;
	     button.nltap=ltapN;
	     button.nsltap=sltapN;
		 /* Reset all counts */
		 ltapN=0;
		 tapN=0;
		 sltapN=0;
		 
		 
		 
		
		 
	 }
	 return 0;
  }
  
  bool buttonInit(void)
  {
	  setButtonColor(zeros);
	  return !button.CAP1188.begin(CAP1188_ADDRESS);
  }
  
  void touched(void)
  {
	  analogWrite(BOARD_BUTTON_REDLED_PIN, 0xff);
  }
/*index */

  #define PETALS_BLINK               0x01
  #define PETALS_PULSE               0x02
  #define PETALS_CHANGE_COLOR        0x03
  #define PETALS_RHYTM_SYNC          0x04
  #define PETALS_RHYTM_ASYNC         0x05
  #define PETALS_EXTEND_BAND_SYNC    0x06
  #define PETALS_FADE                0x07


  void petalsMonitor(void)
  {
	  byte petalIndex;
	  unsigned long int actTime;
	  
	  
	  for(petalIndex=0;petalIndex<BOARD_INSTALLED_PETALS;petalIndex++)
	  {
		 
		  
		  if(petals[petalIndex].iWork)
		  {
			  actTime=millis();
			  switch(petals[petalIndex].lastCmdpParam[0])
			  {
				  
				  case PETALS_BLINK:
				  
				  //Serial.print("ci sono");
				  petals[petalIndex].deltaT=actTime-petals[petalIndex].lastTime;
				  if(petals[petalIndex].lastTime==0)  
				  {
					
					 petals[petalIndex].genState=true;
					 petals[petalIndex].color[0]=petals[petalIndex].lastCmdpParam[3];
					 petals[petalIndex].color[1]=petals[petalIndex].lastCmdpParam[4];
					 petals[petalIndex].color[2]=petals[petalIndex].lastCmdpParam[5];
					 petals[petalIndex].color[3]=petals[petalIndex].lastCmdpParam[6];
				     petals[petalIndex].period=(petals[petalIndex].lastCmdpParam[1]<<8)+petals[petalIndex].lastCmdpParam[2];
					 Set_Selected_PCA9633_AllOut(petals[petalIndex].address,petals[petalIndex].color);
					 petals[petalIndex].lastTime=actTime;
				  }
				  if(petals[petalIndex].deltaT>petals[petalIndex].period)
				  {
					  if(petals[petalIndex].genState)
					     Set_Selected_PCA9633_AllOff(petals[petalIndex].address);
					  else
						 Set_Selected_PCA9633_AllOut(petals[petalIndex].address,petals[petalIndex].color);
			          petals[petalIndex].genState=!petals[petalIndex].genState;
					  petals[petalIndex].lastTime=actTime;
				  
				  }
				     
					 
				  break;
				  
				  case PETALS_PULSE:
				  
				  petals[petalIndex].deltaT=actTime-petals[petalIndex].lastTime;
				  if(petals[petalIndex].genState)
		          {
		             petals[petalIndex].ramp+=petals[petalIndex].deltaT/((float)((petals[petalIndex].lastCmdpParam[1]<<8)+petals[petalIndex].lastCmdpParam[2]));
					 
		             if(petals[petalIndex].ramp>1)
		             {
			            petals[petalIndex].ramp=1;
			            petals[petalIndex].genState=false;
		             }
		          }
		          else
		          {
			         petals[petalIndex].ramp-=petals[petalIndex].deltaT/((float)((petals[petalIndex].lastCmdpParam[3]<<8)+petals[petalIndex].lastCmdpParam[4]));
			         if(petals[petalIndex].ramp<0) 
			         {
				        petals[petalIndex].genState=true;
				        petals[petalIndex].ramp=0;
		             }
		          }
				  petals[petalIndex].color[0]=petals[petalIndex].lastCmdpParam[5]*petals[petalIndex].ramp;
			      petals[petalIndex].color[1]=petals[petalIndex].lastCmdpParam[6]*petals[petalIndex].ramp;
				  petals[petalIndex].color[2]=petals[petalIndex].lastCmdpParam[7]*petals[petalIndex].ramp;
				  petals[petalIndex].color[3]=petals[petalIndex].lastCmdpParam[8]*petals[petalIndex].ramp;
				  Set_Selected_PCA9633_AllOut(petals[petalIndex].address,petals[petalIndex].color);
				  
			      petals[petalIndex].lastTime=actTime;
				  
				  
				  break;
				   
				  case PETALS_CHANGE_COLOR:
				 	 //Serial.print("+++");
					 petals[petalIndex].color[0]=petals[petalIndex].lastCmdpParam[1];
					 petals[petalIndex].color[1]=petals[petalIndex].lastCmdpParam[2];
					 petals[petalIndex].color[2]=petals[petalIndex].lastCmdpParam[3];
					 petals[petalIndex].color[3]=petals[petalIndex].lastCmdpParam[4];
				     Set_Selected_PCA9633_AllOut(petals[petalIndex].address,petals[petalIndex].color);
				     petals[petalIndex].iWork=false;
				  
				  break;
				  
				  case PETALS_RHYTM_ASYNC:
				     msgeq7.poll();
				  
				  
				  case PETALS_RHYTM_SYNC:
				      
				     petals[petalIndex].color[1]=(msgeq7.getAnalog_Value(HZ_63,4,1)+msgeq7.getAnalog_Value(HZ_160,4,1)+msgeq7.getAnalog_Value(HZ_400,4,1))/3;
					 petals[petalIndex].color[2]=(msgeq7.getAnalog_Value(HZ_1000,4,1)+msgeq7.getAnalog_Value(HZ_2500,4,1))>>1;
					 petals[petalIndex].color[3]=(msgeq7.getAnalog_Value(HZ_6250,4,1)+msgeq7.getAnalog_Value(HZ_16000,4,1))>>1;
				     Set_Selected_PCA9633_AllOut(petals[petalIndex].address,petals[petalIndex].color);
				  break;
				  
				  case PETALS_EXTEND_BAND_SYNC:
				     
					 petals[petalIndex].color[1]=(msgeq7.getAnalog_Value(HZ_63,4,1)+msgeq7.getAnalog_Value(HZ_160,4,1))>>1;
					 petals[petalIndex].color[2]=msgeq7.getAnalog_Value(HZ_400,4,1);
					 petals[petalIndex].color[3]=(msgeq7.getAnalog_Value(HZ_2500,4,1)+msgeq7.getAnalog_Value(HZ_1000,4,1))>>1;
				     petals[petalIndex].color[0]=(msgeq7.getAnalog_Value(HZ_16000,4,1)+msgeq7.getAnalog_Value(HZ_6250,4,1))>>1;
					 Set_Selected_PCA9633_AllOut(petals[petalIndex].address,petals[petalIndex].color);
				  break;
				  
				  case PETALS_FADE:
				     byte i;
					 byte app;
					 bool rotate=false;

					 if(petals[petalIndex].lastTime==0)
					 {
						 petals[petalIndex].color[0]=petals[petalIndex].lastCmdpParam[3];
						 petals[petalIndex].color[1]=petals[petalIndex].lastCmdpParam[4];
						 petals[petalIndex].color[2]=petals[petalIndex].lastCmdpParam[5];
						 petals[petalIndex].color[3]=petals[petalIndex].lastCmdpParam[6];
						 petals[petalIndex].lastTime=actTime;
					 }
					 else
					{	 
					 petals[petalIndex].deltaT=actTime-petals[petalIndex].lastTime;
					 //petals[petalIndex].lastTime=actTime;
					 for (i=0;i<4;i++)
					 {
					    petals[petalIndex].ramp=petals[petalIndex].deltaT/((float)((petals[petalIndex].lastCmdpParam[1]<<8)+petals[petalIndex].lastCmdpParam[2]));
					    if(petals[petalIndex].ramp<0)
						{
							petals[petalIndex].ramp=0;
						    petals[petalIndex].lastTime=0;
							rotate=true;
							
						}
						if(petals[petalIndex].ramp>1)
						{
							petals[petalIndex].ramp=1;
							petals[petalIndex].lastTime=0;
							rotate=true;
							
						}
						petals[petalIndex].color[i]=(petals[petalIndex].lastCmdpParam[7+i]-petals[petalIndex].lastCmdpParam[3+i])*petals[petalIndex].ramp+petals[petalIndex].lastCmdpParam[3+i];
					 }
				    } 
					 if(rotate)
					 {
						 rotate=false;
						 for(i=0;i<4;i++)
						 {
							 app=petals[petalIndex].lastCmdpParam[7+i];
							 petals[petalIndex].lastCmdpParam[7+i]=petals[petalIndex].lastCmdpParam[3+i];
							 petals[petalIndex].lastCmdpParam[3+i]=app;
							 
						 }
				     }
					 Set_Selected_PCA9633_AllOut(petals[petalIndex].address,petals[petalIndex].color);
				  
				  break;
				  
				  
			  }
			  
			  
			  
			  //petals[petalIndex].deltaT=actTime-petals[petalIndex].lastTime;
			 // petals[petalIndex].lastTime=actTime;
		  }
	  }
  }
  
/**
 *Memory command type definition 
 */
#define MEMORY_CMD_DESCRIPTOR       0x00
#define MEMORY_CMD_NEW_ANIM   		0x01
#define MEMORY_CMD_STORE_ANI        0x02
#define MEMORY_CMD_FRAME_TYME       0x03
  
  
  
  void memoryHandler(byte* command, byte commandLen)
  {
	  
	  
	  switch (command[0])
	  {
		  case MEMORY_CMD_DESCRIPTOR:
		   // memoryDesc.currentPoint=command[1]<<8+command[2];
		  break;
		  
		  case MEMORY_CMD_NEW_ANIM:
		    
			memoryDesc.flags|=MEMORY_STATE_SAVE_NEW_ANIM_MASK;
			memoryDesc.animDescriptor.animParameter.animId = command[1];
			memoryDesc.animDescriptor.animParameter.adrStart= command[2]<<8+command[3];
			memoryDesc.animDescriptor.animParameter.numFrame=command[4];
		  
		  break;
		  
		  case MEMORY_CMD_FRAME_TYME:
		    memoryDesc.animDescriptor.animParameter.frameTimeLen[memoryDesc.frameIndex]=command[1]<<8+command[2];
			memoryDesc.animDescriptor.animParameter.numberCmd[memoryDesc.frameIndex]=command[3];
			memoryDesc.frameIndex++;
		  break;
		  
		 // default:
		  //  EpromWriteString(EPROM_BANK0_ADDRESS, memoryDesc.currentPoint, command, PARAM_LEN);
		  
	  }
  }
  
  byte animationDescriptor_save(animDescriptorType* descriptor, unsigned int adr)
  {
	  EpromWriteString(EPROM_BANK0_ADDRESS, adr, descriptor->raw, ANIMATION_DESCRIPTOR_LEN);
      return 0;
  }
  
  byte animationDescriptor_load(animDescriptorType* descriptor, unsigned int adr)
  {
	  EpromReadString(EPROM_BANK0_ADDRESS, adr, descriptor->raw, ANIMATION_DESCRIPTOR_LEN);
	  return 0;
  }
  
  byte storedCommand_save(comType* command, unsigned int adr)
  {
	  EpromWriteString(EPROM_BANK0_ADDRESS, adr, command->rawData, ANIMATION_DESCRIPTOR_LEN);
      return 0;
  }
  
  byte storedCommand_load(comType* command, unsigned int adr)
  {
	  EpromReadString(EPROM_BANK0_ADDRESS, adr, command->rawData, ANIMATION_DESCRIPTOR_LEN);
      return 0;
  }
  
  
  void memoryManagare(void)
  {
	if(memoryDesc.flags&MEMORY_FLAGS_SAVE_NEW_ANIM_MASK)
	{
		memoryDesc.status&=~MEMORY_FLAGS_SAVE_NEW_ANIM_MASK;
		memoryDesc.frameIndex=0;
		memoryDesc.status|=MEMORY_FLAGS_SAVE_NEW_ANIM_MASK;
	}  

  }
  
  
  
  #define TIME_DELAY_BETWEEN 100//80
  void animationManager(systemBufType* myBuffer)
  {
	  
	  static unsigned long int lastTime=0;
	  unsigned long int actTime;
	  static byte actPointer  = 0;
	  static byte lastPointer = 0;
	  
	  
	  static byte command[]={2,0,3,0,0,0,0};
	  static int level=16;
	  static int level1=4;
	  static byte scelta = 3;
	  static int inc = 1;//uguale a 1 nel caso di scelta=2 altrimenti 7
 	  
	  actTime=millis();
	  
	  switch (scelta)
	  {
		  case 1:
			  if((actTime-lastTime)>TIME_DELAY_BETWEEN)
			  {
				  command[1]=lastPointer;
				  command[3]=0;
				  command[4]=0;
				  command[5]=0;
				  command[6]=0;
				  push_command(myBuffer, command, 7);  
				  
				  lastPointer=actPointer;
				 
				  actPointer = random(0,7);
				  
				  command[1]=actPointer;
				  command[3]=255;
				  command[4]=0;//level1;
				  command[5]=0;
				  command[6]=0;//level1;
				  
				  push_command(myBuffer, command, 7); 
				  
				  // level+=2;
				  // level1+=4;
				  // if(level>255) level=2;		  
				  // if(level1>255)level1=4;
				  // actPointer++;
				  // actPointer%=7;
				  
				  
				  lastTime=actTime;
				  
			  }
	  break;
	  
	  case 2:
		if((actTime-lastTime)>TIME_DELAY_BETWEEN)
        {
		     command[1] = lastPointer;
		     command[3] = 0;
		     command[4] = 0;
		     command[5] = 0;
		     command[6] = 0;
		     push_command(myBuffer, command, 7);  
		  
		     command[1] = actPointer;
		     command[3] = 0;
		     command[4] = 255;//level1;
		     command[5] = 0;
		     command[6] = 0;//level1;
		  
		     push_command(myBuffer, command, 7); 
			 
			 lastPointer=actPointer;
			 if(actPointer == 6) inc=-1;
			 if(actPointer == 0) inc=+1;
			 actPointer += inc;
			 //actPointer %= 7;
		     lastTime=actTime;
				  
	    }
	  
	  break;
	  case 3:
		if((actTime-lastTime)>TIME_DELAY_BETWEEN)
        {
		  command[1]=lastPointer;
		  command[3]=0;
		  command[4]=0;
		  command[5]=0;
		  command[6]=0;
		//  push_command(myBuffer, command, 7);  
		  
		  lastPointer=actPointer;
		 
		  
		  
		  
		  
		  command[1]=actPointer;
		  command[3]=0;
		  command[4]=0;//level1;
		  command[5]=0;
		  command[6]=level1;
		  
		  push_command(myBuffer, command, 7); 
          
		  level+=2;
		  level1+=4;
		  if(level>255) level=2;		  
		  if(level1>255)level1=4;
		  actPointer++;
		  actPointer%=7;
		  
		  lastTime=actTime;
		}
	  break;
	  
	  case 4:
		  if((actTime-lastTime)>TIME_DELAY_BETWEEN)
			{
				 
				if(actPointer==7)
				{
					 command[1] = 0xFF;
					 command[3] = 0;
					 command[4] = 0;
					 command[5] = 0;
					 command[6] = 0;
					 push_command(myBuffer, command, 7);  
					 actPointer=0;
				}  
				else
				{
					 command[1] = actPointer;
					 command[3] = 0;
					 command[4] = 255;//level1;
					 command[5] = 0;
					 command[6] = 0;//level1;
					 push_command(myBuffer, command, 7); 
					 actPointer++;
				}
				lastTime=actTime;
					  
			}
		
	  break;
	  case 5:
		  if((actTime-lastTime)>TIME_DELAY_BETWEEN)
			{
			
				
				
				command[1] = lastPointer;
		        command[3] = 0;
		        command[4] = 0;
		        command[5] = 0;
		        command[6] = 0;
		        push_command(myBuffer, command, 7);  
		  
		        command[1] = actPointer;
		        command[3] = 0;
		        command[4] = 255;//level1;
		        command[5] = 0;
		        command[6] = 0;//level1;
		  
		        push_command(myBuffer, command, 7); 
			 
			    
				lastPointer=actPointer;
				actPointer++;
				if(actPointer == inc)
				{
					inc--;
					actPointer = 0;
					lastPointer = 0;
					if(inc == 0)
					{
						inc = 7;
					    lastPointer=0xFF;
					}
				}
				actPointer %= inc;
				
			    //actPointer %= 7;
		        lastTime=actTime;
					  
			}
	  
	  break;
	  }
	  
  }
  
  