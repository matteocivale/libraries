// MSGEQ7.h
// Library for controlling MSGEQ7 Audio Spectrum Analyzer Chip (https://www.sparkfun.com/products/10468)
// Credit to J Skoba (http://nuewire.com/info-archive/msgeq7-by-j-skoba/) for the original implementation


#ifndef __MSGEQ7_H
#define __MSGEQ7_H


#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <thePins_arduino.h>
#endif

//#define 63HZ 0

typedef enum {
  HZ_63,
  HZ_160,
  HZ_400,
  HZ_1000,
  HZ_2500,
  HZ_6250,
  HZ_16000
} FREQ;

class MSGEQ7
{
	public:
		MSGEQ7();
		void init(int analogPin, int strobePin, int resetPin);

		void poll();
		void calc_Med();
		byte getAnalog_Value(FREQ freqency,byte passi, bool silenziato);
		
		int getValue(int index);
		int getValue(FREQ freq);
		bool checkBleEnable(FREQ freq);
		byte med;
		

	private:
		int analogPin; // read from multiplexer using analog input 0
		int strobePin; // strobe is attached to digital pin 2
		int resetPin; // reset is attached to digital pin 3
		

		int spectrumValues[7]; // to hold a2d values
		int scaler;
};

#endif