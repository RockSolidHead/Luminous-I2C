//******************* LED Blink mode *************
// LEDの点灯モード
//LED消灯
#define   LED_OFF           0x00
//LED点灯
#define   LED_ON            0x10
//LED点滅
#define   LED_BLINK         0x20
//LED 長く消灯１回点灯
#define   LED_BLINK_OFFON   0x30
//LED 長く消灯２回点灯
#define   LED_BLINK_OFFONON 0x40
//LED だんだん明るくだんだん暗く
#define   LED_BLINK_STUD    0x50
//LED 急に明るく急に暗く
#define   LED_BLINK_ST2UD   0x60
//LED だんだん明るく消灯
#define   LED_BLINK_STU     0x70
//点灯モードは上位4ビット
#define MODE_MASK   0xf0

//LEDの点滅スピード
#define LED_SPEED_LEVEL0  0 
#define LED_SPEED_LEVEL1  1
#define LED_SPEED_LEVEL2  2
#define LED_SPEED_LEVEL3  3
#define LED_SPEED_LEVEL4  4
#define LED_SPEED_LEVEL5  5
#define LED_SPEED_LEVEL6  6
#define LED_SPEED_LEVEL7  7
#define LED_SPEED_LEVEL8  8 
#define LED_SPEED_LEVEL9  9
#define LED_SPEED_LEVEL10 10
#define LED_SPEED_LEVEL11 11
#define LED_SPEED_LEVEL12 12
#define LED_SPEED_LEVEL13 13
#define LED_SPEED_LEVEL14 14
#define LED_SPEED_LEVEL15 15

#define LED_SPEED_SLOW    0
#define LED_SPEED_NORMAL  4
#define LED_SPEED_FAST    5
#define LED_SPEED_VFAST   7

#define LED_SPEED_ANALOG_SLOW 12
#define LED_SPEED_ANALOG_NORMAL 14
#define LED_SPEED_ANALOG_FAST 15

//LED点滅等の間隔設定は下位4ビット
#define SPEED_MASK   0x0f

// LEDの点滅スピード用のタイミング（ｍSec)  
#define LED_BLINK_WAIT0   2000
#define LED_BLINK_WAIT1   1600
#define LED_BLINK_WAIT2   1200
#define LED_BLINK_WAIT3   1000
#define LED_BLINK_WAIT4   800
#define LED_BLINK_WAIT5   600
#define LED_BLINK_WAIT6   400
#define LED_BLINK_WAIT7   200
#define LED_BLINK_WAIT8   8
#define LED_BLINK_WAIT9   7
#define LED_BLINK_WAIT10  6
#define LED_BLINK_WAIT11  5

#define LED_BLINK_WAIT12  4

#define LED_BLINK_WAIT13  2
#define LED_BLINK_WAIT14  1
#define LED_BLINK_WAIT15  0

// I2C関係
// 本機のSLAVEアドレス
#define LUM_ADDRESS         0x11
//フルカラーLED１つに点滅モード・カラーを設定
#define LUM_FCL_SET         0x01
//フルカラーLEDの全体の明るさ設定
#define LUM_FCL_BRIGHT      0x02
//フルカラーLEDに個別にRGBカラーを設定
#define LUM_FCE_COLOR_SET   0x04
//フルカラーLEDすべてに同じ点滅モード・カラーを設定
#define LUM_FCL_ALL_SET     0x11
//モノカラーLED１つに点灯モードを設定
#define LUM_MCL_SET         0x21
//モノカラーLEDすべてに同じ点滅モード・カラーを設定
#define LUM_MCL_ALL_SET     0x31
//本機を初期化
#define LUM_INIT            0x7f

// モノカラーLEDの数
#define LED_MONO_COLOR_NUM  8

// フルカラーLEDの数
#define LED_FULL_COLOR_NUM 24
// フルカラーLEDの制御用のPIN
#define LED_FULL_COLOR_CONT_PIN 2

//======================================== LED COLOR
#define LED_RED       0
#define LED_BLUE      1
#define LED_GREEN     2
#define LED_ORANGE    3
#define LED_PINK      4
#define LED_YELLOW    5
#define LED_WHITE     6
#define LED_SAKURA    9
#define LED_CYAN      7
#define LED_BLACK     8
#define LED_LIGHTGREEN  10
#define LED_LEMON     11
#define LED_VIOLET    12
#define LED_PURPLE    13

//========================================== LED FRONT or BACK
#define FRONT 1
#define BACK  2

#ifndef LED_DEF
#define LED_DEF 1

// LED点滅速度設定用
unsigned long   ledBlinkDiff[]={
  LED_BLINK_WAIT0,LED_BLINK_WAIT1,LED_BLINK_WAIT2,LED_BLINK_WAIT3,
  LED_BLINK_WAIT4,LED_BLINK_WAIT5,LED_BLINK_WAIT6,LED_BLINK_WAIT7,
  LED_BLINK_WAIT8,LED_BLINK_WAIT9,LED_BLINK_WAIT10,LED_BLINK_WAIT11,
  LED_BLINK_WAIT12,LED_BLINK_WAIT13,LED_BLINK_WAIT14,LED_BLINK_WAIT15 };
// 長く消灯、１回点灯時の制御用配列関係
byte  BlinkOFFON[]={0, 0, 0, 0,0,0, 1};
#define BlinkOFFON_NUM   7
// 長く消灯、２回点灯時の制御用配列関係
byte  BlinkOFFONON[]={0, 0, 0, 0,0,0, 1, 0, 1};
#define BlinkOFFONON_NUM   9

//-----------------------------------For Mono Color LEDs
struct  LEDARRAY_MONO_COLOR {
  byte           ModeNo,Count;
  unsigned  long  LastUpDateTime;
};
class LED_CONTROL_MONO_COLOR {
public:
  struct  LEDARRAY_MONO_COLOR LEDMC[LED_MONO_COLOR_NUM];

  void  defaultSetting(void);
  void  showLED(byte);  //LED No
  void  showLEDall(void);
  void  LEDOff(byte);  //LED No
  void  LEDOn(byte);  //LED No
  void  setLED(byte,byte);   //LED No,Mode( STYLE | SPEED
  void  LEDBlink(byte);   //LED No
  void  blinkSTUD(byte);  // Step UP->DOWN
  void  blinkST2UD(byte); // Step UP2 -> DOWN2
  void  blinkOffOn(byte);  //LED No
  void  blinkOffOnOn(byte);  //LED No
  void  blinkSTU( byte );  //LED No
};

//========================================For Full Color  LEDs
struct  LEDARRAY_FULL_COLOR {
  byte          ModeNo,Count;
  unsigned long LastUpDateTime;
  unsigned int  r,g,b,rb,gb,bb; // FRONT RGB &  BACK RGB
};
class LED_CONTROL_FULL_COLOR {
public:
  struct  LEDARRAY_FULL_COLOR LEDFC[LED_FULL_COLOR_NUM];
  int   LEDBright;

  void  defaultsetting(void);
  void  showLEDall(void);
  void  initLED(byte);
  void  BrightUpDown(byte);
  void  LEDOff(byte);
  void  LEDOn(byte);
  void  LEDBlink(byte);
  void  BlinkOffOn( byte ) ;
  void  BlinkOffOnOn( byte  ) ;
  void  blinkSTUD( byte );
  void  blinkST2UD(byte);
  void  blinkSTU(byte);
  void  setLEDColorMode( byte, byte ,byte  ,byte );
  void  setLEDMode(byte,byte);
  void  setLEDColor( byte , byte ,byte ) ;
  void  setLEDColorFB( byte , byte ,byte ,byte ,byte );
  void  setLEDColorFB( byte , byte  , byte );
  void  SetDiscomffortIndex(byte , byte);
};

#endif
