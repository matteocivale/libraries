  
  #include "Arduino.h"
  #include "Adafruit_CAP1188.h"
  #include "PCA9633.h"
  #include "command.h"
  #include "MSGEQ7.h"
  #include "EPROM24LC256.h" 
  
  #define BOARD_BUTTON_REDLED_PIN    9    
  #define BOARD_BUTTON_GREENLED_PIN  10
  #define BOARD_BUTTON_BLUELED_PIN   11
  
  #define CAP1188_RESET   2
  #define CAP1188_ADDRESS 0x28
  
  #define CAP1188_IRQ_LINE A0
  
  #define BUTTON_TIME_SHORT  50 
  #define BUTTON_TIME_LONG   300
  #define BUTTON_TIME_SLONG  800
  
  
  #define MSGEQT_PIN_OUTPUT  A6
  #define MSGEQT_PIN_STROBE  12
  #define MSGEQT_PIN_RESET   13
  
  #define BOARD_MAX_PETAL_NUM    7 
  #define BOARD_INSTALLED_PETALS 7
  
  /* system register mask */
  #define SYSTEM_BLE_ON  0x0
  #define SYSTEM_BLE_OFF 0x01
  
  #define SYSTEM_COMMAND_MODE   0x0
  #define SYSTEM_ANIMATION_MODE 0x1
  
  /* Audio pins */
  #define AMPLIFY_SD 4
  /* Eprom define */
  
  #define EPROM_BANK0_ADDRESS 0x50
  
  typedef struct
  {
	  byte bleState;
	  
	  byte sysStatus;
	  
  }SysRegType;
  
  typedef struct 
  {
	  bool iWork;
	  byte address;
	  byte lastCmdpParam[PARAM_LEN];
	  unsigned long int lastTime;
	  unsigned long int deltaT;
	  float ramp;
	  float function_red;
	  bool genState;
	  unsigned int period;
	  byte color[4];
   }petalType;
  
  typedef struct 
  {
	  byte color[3];
	  bool writeColor;
	  bool touchState;
	  long int time;
	  long int deltaT;
	  
	  Adafruit_CAP1188 CAP1188;//Adafruit_CAP1188(CAP1188_RESET);
	  
  }buttonType;
  
  #define MEMORY_DESCRIPTOR_LEN 10 
  typedef struct 
  {
	  byte animationID[MEMORY_DESCRIPTOR_LEN];
	  unsigned int animationAddress[MEMORY_DESCRIPTOR_LEN];
	  unsigned int animationLen[MEMORY_DESCRIPTOR_LEN]; 
	  unsigned int currentPoint;
	  
  }memoryDescriptorType;
  
  extern memoryDescriptorType memoryDesc;
  
  
  
  
  typedef enum
  {
	  
	  BUTTON_SONG_PLAY  =0x0,
	  BUTTON_SONG_NEXT  =0x1,
	  BUTTON_SONG_PAUSE =0x2,
	  BUTTON_SONG_STOP  =0x3,
	  BUTTON_LAUD_UP    =0x4,
	  BUTTON_LAUD_DOWN  =0x5,
	  
  }mCmdType;
  
  extern petalType petals[];
  
  extern  MSGEQ7 msgeq7;
  
  extern SysRegType sysReg;
  
  void boardInit(void);
  
  bool buttonInit(void);
  
  void setButtonColor(byte *color);
  
  byte buttonMonitor(void);
  
  void touched(void);
  void petalsMonitor(void);
  
  void memoryHandler(byte* command, byte commandLen);