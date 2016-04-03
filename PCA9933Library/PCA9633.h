#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <Wire.h>

//register address
  #define  MODE1_regAdr   0x00
  #define  MODE2_regAdr   0x01
  #define  LEDOUT_regAdr  0x08
  #define  GRPFREQ_regAdr 0x07
  
//LEDOUT MASKS
  #define LED_OUT 0x08
  #define LED_IND_MASK 0x02
  #define LED_IND_SET(X)  (LED_IND_MASK<<(X<<1))&0xFF  
  #define RESET_ADDRESS 0x6
  #define PWMX_OFFSET   0x02
  #define NO_AUTO_INC_MASK 0x00
  #define ALL_LED_INC_MASK 0xE0

  
 /************************************************************************************************************/
 /*                                                                                                          */
 /*                                                Function definition                                       */
 /*                                                                                                          */
 /*                                                                                                          */
 /************************************************************************************************************/
 
  byte Set_Selected_PCA9633_AllOut(byte address,byte *dimming_value);
  byte Set_Selected_PCA9633_AllOff(byte address);
 
/*************************************************************************************************************/
/*     The PCA identify by address parameter will be configured as follow:                                   */
/*                                                                                                           */   
/*      1) disable auto increment                                                                            */
/*      2) enable normal mode                                                                                */
/*      3) not respond to any subaddress                                                                     */
/*      4) inverting output                                                                                  */
/*      5) output equal to 0 when driver disabled                                                            */
/*************************************************************************************************************/

  byte Inizilize_PCA9633(byte address);


