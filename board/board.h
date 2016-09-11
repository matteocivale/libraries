  
  #include "Arduino.h"
  #include "Adafruit_CAP1188.h"
  #include "PCA9633.h"
  #include "command.h"
  #include "MSGEQ7.h"
  #include "EPROM24LC256.h" 
  #include "BC127.h"
  
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
  
  typedef enum
  {
	 SL_STARTUP,
	 SL_OFF,
	 SL_DISCONNECTED,
	 SL_CONNECTED,
  }stateType;
  
  #define ANIM_TYPE_CUSTOM 0x00
  #define ANIM_TYPE_USER   0x01
  
  typedef struct
  {
	  byte bleState;
	  stateType state;
	  byte sysStatus;
	  struct
	  {
		byte animId;
		byte animType;
		struct
		{
			bool start;
			bool stop;
		}command;
	  }animInfo;
	  
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
	  /* Number of different tap */
	  byte ntap;
	  byte nltap;
	  byte nsltap;
	  
	  Adafruit_CAP1188 CAP1188;//Adafruit_CAP1188(CAP1188_RESET);
	  
  }buttonType;
  extern buttonType button;
  extern byte animation_memory[];
  /**
  * Structure for memory handling
  */
  
  
  #define ANIMATION_DESCRIPTOR_LEN        91
  #define ANIMATION_MAX_FRAME_NUMBER      20
  #define ANIMATION_MEMORY_ADDRESS_OFFSET 0x0
  
  typedef union
  {
	struct
	{
		byte animId;
		unsigned int  adrStart;
		byte numFrame;
		unsigned int frameTimeLen[ANIMATION_MAX_FRAME_NUMBER];
		byte numberCmd[ANIMATION_MAX_FRAME_NUMBER];
	}animParameter;
	
	byte raw[ANIMATION_DESCRIPTOR_LEN];
	
  }animDescriptorType;
  
  
  
 /* This is the list of memory command  */ 
#define MEMORY_FLAGS_SAVE_NEW_ANIM_MASK  0x01
#define MEMORY_FLAGS_SAVE_NEW_ANIM_SHIFT 0x00

#define MEMORY_STATE_SAVE_NEW_ANIM_MASK  0x02
#define MEMORY_STATE_SAVE_NEW_ANIM_SHIFT 0x01

  typedef struct 
  {
      byte status;	  
	  byte flags;
	  animDescriptorType animDescriptor;
	  unsigned int commandCont;
	  byte frameIndex;
	  
  }memoryDescriptorType;
  
  extern memoryDescriptorType memoryDesc;
  
  /*************************************************************/
  
  
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
  void memoryManagare(void);
  
  void animationManager(systemBufType* myBuffer);
  
  byte animationDescriptor_save(animDescriptorType* descriptor, unsigned int adr);
  byte storedCommand_save(byte* command, unsigned int adr);