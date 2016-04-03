// MSGEQ7.cpp
// Library for controlling MSGEQ7 Audio Spectrum Analyzer Chip (https://www.sparkfun.com/products/10468)
// Credit to J Skoba (http://nuewire.com/info-archive/msgeq7-by-j-skoba/) for the original implementation

#include "MSGEQ7.h"


MSGEQ7::MSGEQ7() 
{
	MSGEQ7::analogPin = A0;
	MSGEQ7::strobePin = 2;
	MSGEQ7::resetPin = 3;
}

void MSGEQ7::init(int analogPin, int strobePin, int resetPin)
{
	MSGEQ7::analogPin = analogPin;
	MSGEQ7::strobePin = strobePin;
	MSGEQ7::resetPin = resetPin;

	pinMode(MSGEQ7::analogPin, INPUT);
	pinMode(MSGEQ7::strobePin, OUTPUT);
	pinMode(MSGEQ7::resetPin, OUTPUT);
	analogReference(DEFAULT);
}

void MSGEQ7::poll()
{
 digitalWrite(MSGEQ7::resetPin, HIGH);
 digitalWrite(MSGEQ7::resetPin, LOW);

 for (int i = 0; i < 7; i++)
 {
   digitalWrite(MSGEQ7::strobePin, LOW);
   delayMicroseconds(30); // to allow the output to settle
   MSGEQ7::spectrumValues[i] = (analogRead(MSGEQ7::analogPin)+analogRead(MSGEQ7::analogPin))>>2;  
   digitalWrite(MSGEQ7::strobePin, HIGH);
 }

 digitalWrite(MSGEQ7::resetPin, LOW);
 digitalWrite(MSGEQ7::strobePin, HIGH);
}

int MSGEQ7::getValue(int index)
{
	return MSGEQ7::spectrumValues[index];
}

int MSGEQ7::getValue(FREQ freq)
{
	return MSGEQ7::spectrumValues[freq];
}


void MSGEQ7::calc_Med(){
int somma=0;  
for(int i=0;i<7;i++){
somma=somma+MSGEQ7::spectrumValues[i];
}
MSGEQ7::med = somma/7;
}

bool MSGEQ7::checkBleEnable(FREQ freq)
{
  byte i;
  static unsigned long int lastTime=0;
  static unsigned long int deltaT=0;
  unsigned long int actTime=0; 
  
 // MSGEQ7::calc_Med();
  i=0;
  while(i<7)
  {
	  if((i!=(byte)freq)&&(MSGEQ7::spectrumValues[i]<255))
	  {
		  i++;
	  }
	  else
		  if(i==(byte)freq) 
			 i++;
	      else
			 break;
  }
  
  if((i==7)&&(MSGEQ7::spectrumValues[freq]>300))
  {
	  actTime=millis();
	  deltaT+=actTime-lastTime;
	 
  }
  else
  {
	  deltaT=0;
	  lastTime=0;
  }
	  
  lastTime=actTime;
  if(deltaT>1000)
    return true;
  
  return  false;
}

byte MSGEQ7::getAnalog_Value(FREQ frequency,byte passi, bool silenziato){
byte valore=MSGEQ7::spectrumValues[frequency]<<2;
int soglia=256/passi;
while (valore<soglia){
soglia=soglia+256/passi;
}
//soglia shift
if (silenziato) soglia=soglia-256/passi;
return soglia; 
}