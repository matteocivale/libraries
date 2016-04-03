#include <PCA9633.h>


//All leds settings at same time 
byte Set_Selected_PCA9633_AllOut(byte address,byte *dimming_value)
{
  Wire.beginTransmission(0x80|address);
  Wire.write(((PWMX_OFFSET)&0x0F)|ALL_LED_INC_MASK);
  Wire.write(dimming_value,4);
  return Wire.endTransmission();
}
/*************************************************************************************************************/
/*     The PCA identfy by address paramiter will be configurated as follo:                                   */
/*                                                                                                           */   
/*      1) disable auto increment                                                                            */
/*      2) enable normal mode                                                                                */
/*      3) not respond to any subaddress                                                                     */
/*      4) inverting output                                                                                  */
/*      5) output egual to 0 when driver disabled                                                            */
/*************************************************************************************************************/

byte Inizilize_PCA9633(byte address)
{
  byte c;
  byte MODE1_reg;
  byte MODE2_reg;
  byte LEDOUT_reg;
  
  MODE1_reg=0x01;
  Wire.beginTransmission(address);
  Wire.write(MODE1_regAdr);
  Wire.write(MODE1_reg);
  c=Wire.endTransmission();
  delay(1);//delay al last 500us to oscillator power on 
//  
  MODE2_reg=0x14;//00010100
  Wire.beginTransmission(address);
  Wire.write(MODE2_regAdr);
  Wire.write(MODE2_reg);
  c|=Wire.endTransmission();
//  
  delay(1);
  LEDOUT_reg=0xAA;//enable all output
  Wire.beginTransmission(address);
  Wire.write(LEDOUT_regAdr);
  Wire.write(LEDOUT_reg);
  c|=Wire.endTransmission();
//  
  return c;
}

byte Set_Selected_PCA9633_AllOff(byte address)
{
	byte zeros[4]={0,0,0,0};
	return Set_Selected_PCA9633_AllOut(address,zeros);
}