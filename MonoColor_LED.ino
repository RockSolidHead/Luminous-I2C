/*
  Luminous-I2C: モノカラーLED制御
  by RockSolidHead
*/

#include  "LuminousCommon.h"

byte pins[]={7,8,3,5,6,9,10,11};
byte LEDType[]={0,0,1,1,1,1,1,1};// 0 -> ON/OFF only , 1-> ON/OFF and PWM

#define DIV 30

//-------------------------------------------------------
//      モノカラーLEDクラスの初期化処理
//-------------------------------------------------------
void  LED_CONTROL_MONO_COLOR::defaultSetting() {
  int i;
// Digital I/O port Setting
  pinMode( 13, OUTPUT);digitalWrite( 13 , LOW );

  for( i=0 ; i<LED_MONO_COLOR_NUM ; i++ ) {
    if( LEDType[i]==0 ) {
      pinMode( pins[i] , OUTPUT );
      digitalWrite( pins[i],LOW );
    } else {
      analogWrite( pins[i],0 );
    }
    LEDMC[i].ModeNo=LED_OFF;
    LEDMC[i].Count=0;
    LEDMC[i].LastUpDateTime=0;
  }
}
//-------------------------------------------------------
//    １つのモノカラーLEDの動作を設定する
//    no:モノカラーLEDの番号、Mode:点灯モード｜点灯スピード
//-------------------------------------------------------
void  LED_CONTROL_MONO_COLOR::setLED( byte no, byte Mode ) { // Mode is STYLE | SPEED
  byte  mm,bk;

  bk=Mode;
  mm=Mode & MODE_MASK; // Blink Mode Number

  if( LEDType[no]==0 ) {  // 0 is ON/OFF only
    switch( mm ) {
      case  LED_OFF:
      case  LED_ON:
      case  LED_BLINK:
      case  LED_BLINK_OFFON:
      case  LED_BLINK_OFFONON:
        break;
      default:
        bk=LED_OFF;
        break;
    }
  } else {
  }
  LEDMC[no].ModeNo=bk;
  LEDMC[no].Count=0;
  LEDMC[no].LastUpDateTime=millis();
}
//-------------------------------------------------------
//    すべてのモノカラーLEDを処理する
//-------------------------------------------------------
void  LED_CONTROL_MONO_COLOR::showLEDall() {
  int i;
  for( i=0 ; i<LED_MONO_COLOR_NUM; i++ ) {
    showLED(i);
  }
}
//-------------------------------------------------------
//    １つのモノカラーLEDを処理する
//-------------------------------------------------------
void  LED_CONTROL_MONO_COLOR::showLED( byte no ) {
  int mm,ss;

  mm=LEDMC[no].ModeNo & MODE_MASK; // 点灯モードは上位４ビット（下位４ビットはオール０）
  ss=LEDMC[no].ModeNo & SPEED_MASK; // 点灯スピードは下位４ビット
  if( !IsOverTime(LEDMC[no].LastUpDateTime,ledBlinkDiff[ss]) ) {  //  点灯（点滅等）の状態変化するタイミングか？
    return;//タイミングまできていないならリターン
  }
  switch( mm ) {
    case  LED_OFF:  //LEDを消灯
      LEDOff( no );
      break;
    case  LED_ON:   //LEDを点灯
      LEDOn( no );
      break;
    case  LED_BLINK:  //LEDを点滅させる
      LEDBlink( no );
      break;
    case  LED_BLINK_OFFON:  //LEDを長く消灯→短く１回点灯するモード
      blinkOffOn( no );
      break;
    case  LED_BLINK_OFFONON: //LEDを長く消灯→短く２回点灯するモード
      blinkOffOnOn( no );
      break;
    case  LED_BLINK_STUD://LEDをだんだん明るくしてだんだん暗くするモード
      blinkSTUD( no );
      break;
    case  LED_BLINK_ST2UD://LEDを急に明るくして急に暗くするモード
      blinkST2UD( no );
      break;
    case  LED_BLINK_STU://LEDをだんだん明るくしてすぐ消灯するモード
      blinkSTU( no ) ;
      break;
    default:
      break;
  }
}
//-----------------------------------------------------------------
//    LEDを長く消灯→短く１回点灯するモード
//-----------------------------------------------------------------
void  LED_CONTROL_MONO_COLOR::blinkOffOn( byte no ) {
  int cc,cnt;

  cnt=LEDMC[no].Count;
  cc= BlinkOFFON[cnt];

  if( cc==0 ) {
    LED_CONTROL_MONO_COLOR::LEDOff(no);
  }else {
    LED_CONTROL_MONO_COLOR::LEDOn(no);
  }
  cnt++;
  if( cnt==BlinkOFFON_NUM ) cnt=0;
  LEDMC[no].Count=cnt;
  LEDMC[no].LastUpDateTime=millis();
}
//-----------------------------------------------------------------
//    LEDを長く消灯→短く２回点灯するモード
//-----------------------------------------------------------------
void  LED_CONTROL_MONO_COLOR::blinkOffOnOn( byte no ) {
  int cc,cnt;

  cnt=LEDMC[no].Count;
  cc= BlinkOFFONON[cnt];

  if( cc==0 ) {
    LED_CONTROL_MONO_COLOR::LEDOff(no);
  }else {
    LED_CONTROL_MONO_COLOR::LEDOn(no);
  }
  cnt++;
  if( cnt==BlinkOFFONON_NUM ) cnt=0;
  LEDMC[no].Count=cnt;
  LEDMC[no].LastUpDateTime=millis();
}

//----------------------------------------------------------------
//  LEDを急に明るくして急に暗くするモード
//----------------------------------------------------------------
void  LED_CONTROL_MONO_COLOR::blinkST2UD( byte no ) {
  byte pwm,cnt;

  cnt=LEDMC[no].Count;
  if( cnt < DIV ) {
    pwm=255/(DIV*DIV)*cnt*cnt;
  } else if( cnt == DIV){
    pwm=255;
  } else {
    pwm=-255/(DIV*DIV)*((cnt-DIV)*(cnt-DIV))+255;
  }
  cnt++;
  if( cnt==(DIV+2) ) cnt=0;
  
  LEDMC[no].Count=cnt;
  analogWrite( pins[no] , pwm);
  LEDMC[no].LastUpDateTime=millis();
}
//----------------------------------------------------------------
//    //LEDをだんだん明るくしてだんだん暗くするモード
//----------------------------------------------------------------
void  LED_CONTROL_MONO_COLOR::blinkSTUD( byte no ) {
  byte pwm,cnt;

  cnt=LEDMC[no].Count;
  if( cnt < DIV ) {
    pwm=255.0/DIV*cnt;
  } else if( cnt == DIV){
    pwm=255;
  } else if( cnt>DIV && cnt<(DIV*2+1) ) {
    pwm=-255/DIV*(cnt-DIV)+255;
  } else {
    pwm=0;
  }
  cnt++;
  if( cnt==(DIV*2+2) ) cnt=0;
  
  LEDMC[no].Count=cnt;
  analogWrite( pins[no] , pwm);
  LEDMC[no].LastUpDateTime=millis();
}
//----------------------------------------------------------------
//    LEDをだんだん明るくしてすぐ消灯するモード
//----------------------------------------------------------------
void  LED_CONTROL_MONO_COLOR::blinkSTU( byte no ) {
  byte pwm,cnt;

  cnt=LEDMC[no].Count;
  if( cnt < DIV ) {
    pwm=255/DIV*cnt;
  } else if( cnt == DIV){
    pwm=-255;
  }
  cnt++;
  if( cnt==DIV+1 ) cnt=0;
  
  LEDMC[no].Count=cnt;
  analogWrite( pins[no],pwm);
  LEDMC[no].LastUpDateTime=millis();
}
//----------------------------------------------------------------
//　LEDを点滅させるモード 
//----------------------------------------------------------------
void  LED_CONTROL_MONO_COLOR::LEDBlink( byte no  ) {

  LEDMC[no].Count=1-LEDMC[no].Count;
  if( LEDMC[no].Count==1 ) {
    LEDOn( no ) ;
  }else {
    LEDOff( no );
  }
  LEDMC[no].LastUpDateTime=millis();
}
//----------------------------------------------------------------
//    LEDを消灯させるモード
//----------------------------------------------------------------
void  LED_CONTROL_MONO_COLOR::LEDOff(byte no ) {

  if( LEDType[no]==0 ) {
    digitalWrite( pins[no],LOW );
  } else {
    analogWrite( pins[no],0);
  }
  LEDMC[no].LastUpDateTime=millis();
}
//----------------------------------------------------------------
//    LEDを点灯させるモード
//----------------------------------------------------------------
void  LED_CONTROL_MONO_COLOR::LEDOn( byte no ) {

  if( LEDType[no]==0 ) {
    digitalWrite( pins[no],HIGH );
  } else {
    analogWrite( pins[no],255);
  }
  LEDMC[no].LastUpDateTime=millis();
}

