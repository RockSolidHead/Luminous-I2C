/*-------------------------------------------------------------

  Project : 状態表示用LEDシステム（Luminous）
  Arduino : Nano
  Interface: I2C

  by RockSolidHead
-------------------------------------------------------------*/
#include  <Wire.h>
#include  <Adafruit_NeoPixel.h>

#include  "LuminousCommon.h"

byte  i2cBuffer[33],i2cNum;
boolean i2cRecFlag;

#define DEBUG 1

//  LED Control
class LED_CONTROL_MONO_COLOR LED_MC;  // for Mono color LED(standard LED)
class LED_CONTROL_FULL_COLOR LED_FC;  // for Full color LED
// for Full color LED
Adafruit_NeoPixel RGBLED = Adafruit_NeoPixel(LED_FULL_COLOR_NUM, LED_FULL_COLOR_CONT_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Wire.begin(LUM_ADDRESS);  //I2c Slave mode
  i2cRecFlag=false;
  pinMode(13,OUTPUT);
  Wire.onReceive(recieveEvent); //I2C 受信処理
  Wire.onRequest(requestEvent); //I2C 要求処理
#ifdef  DEBUG
  Serial.begin(19200);
#endif
  LED_MC.defaultSetting();
  LED_FC.defaultsetting();
}
void  recieveEvent(int num ) {
  if( i2cRecFlag==true ) {
    digitalWrite( 13 , HIGH );
  }
  i2cNum=0;
  while(Wire.available()){
    i2cBuffer[i2cNum++]=Wire.read();
  }
  i2cRecFlag=true;
  digitalWrite( 13 , LOW );
}
void requestEvent() {
  Wire.write(255);
}
//---------------------------------------------------
//  i2cで受信したデータに合わせて動作する部分
//---------------------------------------------------
void  i2cparser() {
  int i,cnt;

  switch( i2cBuffer[0] ) {
    case  LUM_FCL_SET: // １つのフルカラーLEDを設定する
      LED_FC.setLEDColorMode(i2cBuffer[1],i2cBuffer[2],i2cBuffer[3],i2cBuffer[4]);
      break;
    case  LUM_FCL_ALL_SET: // すべてのフルカラーLEDを同一に設定する
      for( i=0 ; i<LED_FULL_COLOR_NUM ; i++ ) {
        LED_FC.setLEDColorMode(i,i2cBuffer[1],i2cBuffer[2],i2cBuffer[3]);
      }
      break;
    case  LUM_FCL_BRIGHT: // フルカラーLEDの明るさを変更する
      LED_FC.BrightUpDown(i2cBuffer[1]);
      break;
    case  LUM_FCE_COLOR_SET:  // フルカラーLEDに個別にRGBカラーを設定
      LED_FC.setLEDColorFB( i2cBuffer[1], i2cBuffer[2],i2cBuffer[3],i2cBuffer[4],i2cBuffer[5] );
      break;
    case  LUM_MCL_SET:   // １つのモノカラーLEDを設定する
      LED_MC.setLED( i2cBuffer[1] , i2cBuffer[2] );
      break;
    case  LUM_MCL_ALL_SET:   // すべてのモノカラーLEDを設定する
      for( i=0; i<LED_MONO_COLOR_NUM ; i++ ) {
        LED_MC.setLED( i, i2cBuffer[i+1]);
      }
      break;
    case  LUM_INIT:// 再起動させる
      for( i=0 ; i<LED_FULL_COLOR_NUM; i++ ) {  // モノカラーLED初期化
        LED_FC.initLED(i);
      }
      for( i=0 ; i<LED_MONO_COLOR_NUM; i++ ) {  // フルカラーLED初期化
        LED_MC.setLED(i,LED_OFF);
      }
      software_reset();
      break;
    default:
#ifdef  DEBUG
      Serial.println("I2C data error");
      Serial.print("Top Code:");Serial.println( i2cBuffer[0] );
#endif
      break;
  }
  i2cRecFlag=false;
}
//  再起動関数
void software_reset() {
  asm volatile ("  jmp 0");  
} 

void loop() {
  if( i2cRecFlag ) {  //I2C受信データがある場合
    i2cparser();      //受信データを解析して処理する  
  }
  LED_MC.showLEDall();//モノカラーLEDを表示する処理
  LED_FC.showLEDall();//フルカラーLEDを表示する処理
}

