/*
  Luminous-I2C: フルカラーLED制御
  by RockSolidHead
*/

#include  "LuminousCommon.h"

#include <Adafruit_NeoPixel.h>

//-------------------------------------------------------
//      initialized
//-------------------------------------------------------
void  LED_CONTROL_FULL_COLOR::defaultsetting() {
  int i;
  
  RGBLED.begin();
  LEDBright=16;
  RGBLED.setBrightness(LEDBright);
  for( i=0 ; i< LED_FULL_COLOR_NUM  ; i++ ) {
      LED_FC.initLED(i); 
  }
}
//-------------------------------------------------------
//      Bright UP/DOWN
//-------------------------------------------------------
void  LED_CONTROL_FULL_COLOR::BrightUpDown(byte v ) {
  if( v==0x01 ) {
    LEDBright+=8;
    if( LEDBright>=64 ) {
      LEDBright=64; 
    }
  }
  else if( v==0x02 ) {
    LEDBright-=8;
    if( LEDBright<=0 ) {
      LEDBright=1;
    }
  }else {
    return;
  }
  RGBLED.setBrightness(LEDBright);
}
//-------------------------------------------------------
//      Update:each LED status  -> show
//-------------------------------------------------------
void    LED_CONTROL_FULL_COLOR::showLEDall() {
  int i;
  byte  mm,ss;

  for( i=0 ; i< LED_FULL_COLOR_NUM ; i++ ) {
    mm= LED_FC.LEDFC[i].ModeNo;
    ss= mm& SPEED_MASK;
    mm= mm & MODE_MASK;
    if( IsOverTime( LEDFC[i].LastUpDateTime , ledBlinkDiff[ss] ) ) {
      switch( mm ) {
        case  LED_OFF:
          LEDOff( i );
          break;
        case  LED_ON:
          LEDOn( i );
          break;
        case  LED_BLINK:
          LEDBlink( i );
          break;
        case  LED_BLINK_OFFON:
          BlinkOffOn( i );
          break;
        case  LED_BLINK_OFFONON:
          BlinkOffOnOn( i );
          break;
        case  LED_BLINK_STUD:
          blinkSTUD( i );
          break;
        case  LED_BLINK_ST2UD:
          blinkST2UD( i );
          break;
        case  LED_BLINK_STU:
          blinkSTU( i );
          break;
      }
    } 
  }
  RGBLED.show();
}

#ifndef DIV
#define DIV 30.0
#endif
//----------------------------------------------------------------
//    LED Step UP -> DOWN
//----------------------------------------------------------------
void  LED_CONTROL_FULL_COLOR::blinkSTUD( byte no ) {
  byte cnt,pwm;

  cnt=LEDFC[no].Count;
  if( cnt < DIV ) {
      pwm=(float)cnt*255.0/DIV;
  } else if( cnt == DIV){
    pwm=255;
  } else if( cnt>DIV && cnt<(DIV*2+1) ) {
    pwm=-255.0/DIV*((float)cnt-DIV)+255.0;
  } else {
    pwm=0;
  }
  cnt++;
  if( cnt==(byte)(DIV*2+2) ) cnt=0;

  LEDFC[no].Count=cnt;
  RGBLED.setPixelColor(no,LEDFC[no].r*(pwm/255.0),LEDFC[no].g*pwm/255.0,LEDFC[no].b*pwm/255.0);
  LEDFC[no].LastUpDateTime=millis();
}

//----------------------------------------------------------------
//  LED Step UP cnt^2 -> DOWN cnt^2
//----------------------------------------------------------------
void  LED_CONTROL_FULL_COLOR::blinkST2UD( byte no ) {
  byte pwm,cnt;

  cnt=LEDFC[no].Count;
  if( cnt < DIV ) {
    pwm=255/(DIV*DIV)*cnt*cnt;
  } else if( cnt == DIV){
    pwm=255;
  } else {
    pwm=-255/(DIV*DIV)*((cnt-DIV)*(cnt-DIV))+255;
  }
  cnt++;
  if( cnt==(DIV+2) ) cnt=0;
  
  LEDFC[no].Count=cnt;
  RGBLED.setPixelColor(no,LEDFC[no].r*(pwm/255.0),LEDFC[no].g*pwm/255.0,LEDFC[no].b*pwm/255.0);
  LEDFC[no].LastUpDateTime=millis();
}

//----------------------------------------------------------------
//    LED Step UP -> Off
//----------------------------------------------------------------
void  LED_CONTROL_FULL_COLOR::blinkSTU( byte no ) {
  byte pwm,cnt;

  cnt=LEDFC[no].Count;
  if( cnt < DIV ) {
    pwm=255/DIV*cnt;
  } else if( cnt == DIV){
    pwm=-255;
  }
  cnt++;
  if( cnt==DIV+1 ) cnt=0;
  
  LEDFC[no].Count=cnt;
  RGBLED.setPixelColor(no,LEDFC[no].r*(pwm/255.0),LEDFC[no].g*pwm/255.0,LEDFC[no].b*pwm/255.0);
  LEDFC[no].LastUpDateTime=millis();
}

//----------------------------------------------------------------
//    LED ON
//---------------------------------------------------------------- 
void  LED_CONTROL_FULL_COLOR::LEDOn( byte no ) {
    LEDFC[no].LastUpDateTime=millis();
    RGBLED.setPixelColor(no,LEDFC[no].r,LEDFC[no].g,LEDFC[no].b);
}

//----------------------------------------------------------------
//    LED OFF
//----------------------------------------------------------------
void  LED_CONTROL_FULL_COLOR::LEDOff( byte no ) {
    LEDFC[no].LastUpDateTime=millis();
    LEDFC[no].r=0; LEDFC[no].g=0; LEDFC[no].b=0;
    RGBLED.setPixelColor(no,LEDFC[no].r,LEDFC[no].g,LEDFC[no].b);
}

//----------------------------------------------------------------
//    LED BLINK
//----------------------------------------------------------------
void  LED_CONTROL_FULL_COLOR::LEDBlink( byte no  ) {
  LEDFC[no].LastUpDateTime=millis();
  switch( LEDFC[no].Count) {
      case  0:
        RGBLED.setPixelColor(no,LEDFC[no].r,LEDFC[no].g,LEDFC[no].b);
        LEDFC[no].Count=1;
        break;
      case  1:
        RGBLED.setPixelColor(no,0,0,0);
        if( LEDFC[no].rb==0 && LEDFC[no].gb==0 && LEDFC[no].bb==0 ) {
          LEDFC[no].Count=0;
        }
        else {
          LEDFC[no].Count=2;
        }
        break;
      case  2:
        RGBLED.setPixelColor(no,LEDFC[no].rb,LEDFC[no].gb,LEDFC[no].bb);
        LEDFC[no].Count=3;
        break;
      case  3:
        RGBLED.setPixelColor(no,0,0,0);
        LEDFC[no].Count=0;
        break;
  }
}

//-----------------------------------------------------------------
//    off off off off off off on
//-----------------------------------------------------------------
void  LED_CONTROL_FULL_COLOR::BlinkOffOn( byte no ) {
  LEDFC[no].Count++;
  LEDFC[no].LastUpDateTime=millis();
  if( LEDFC[no].Count==BlinkOFFON_NUM ) LEDFC[no].Count=0;
  if( BlinkOFFON[LEDFC[no].Count]==1 ) {
      RGBLED.setPixelColor(no,LEDFC[no].r,LEDFC[no].g,LEDFC[no].b);
  } else {
      RGBLED.setPixelColor(no,LEDFC[no].rb,LEDFC[no].gb,LEDFC[no].bb);
  }
}

//-----------------------------------------------------------------
//    off off off off off off on off on
//-----------------------------------------------------------------
void  LED_CONTROL_FULL_COLOR::BlinkOffOnOn( byte no ) {
  LEDFC[no].Count++;
  LEDFC[no].LastUpDateTime=millis();
  if( LEDFC[no].Count==BlinkOFFONON_NUM ) LEDFC[no].Count=0;
  if( BlinkOFFONON[LEDFC[no].Count]==1 ) {
    RGBLED.setPixelColor(no,LEDFC[no].r,LEDFC[no].g,LEDFC[no].b);
  } else {
    RGBLED.setPixelColor(no,LEDFC[no].rb,LEDFC[no].gb,LEDFC[no].bb);
  }
}

//-----------------------------------------------------------------
//    Initialize FULL COLOR LEDs
//-----------------------------------------------------------------
void  LED_CONTROL_FULL_COLOR::initLED(byte no) {
  LEDFC[no].ModeNo=0;  LEDFC[no].Count=0;
  LEDFC[no].r=0;  LEDFC[no].g=0;  LEDFC[no].b=0;
  LEDFC[no].rb=0;  LEDFC[no].gb=0;  LEDFC[no].bb=0;
  LEDFC[no].LastUpDateTime=0;
}

//-----------------------------------------------------------------
//    set color & mode
//-----------------------------------------------------------------
void  LED_CONTROL_FULL_COLOR::setLEDColorMode( byte no, byte Mno,byte Ano,byte Bno ) {
  LEDFC[no].ModeNo=Mno;
  LEDFC[no].Count=0;
  LEDFC[no].LastUpDateTime=millis();
  setLEDColorFB( no, Ano,FRONT );
  setLEDColorFB( no, Bno,BACK );
}

//-----------------------------------------------------------------
//    set mode
//-----------------------------------------------------------------
void  LED_CONTROL_FULL_COLOR::setLEDMode( byte no, byte Mno ) {
  LEDFC[no].ModeNo=Mno;
  LEDFC[no].Count=0;
  LEDFC[no].LastUpDateTime=millis();
}

//-----------------------------------------------------------------
//    set front & back color
//-----------------------------------------------------------------
void  LED_CONTROL_FULL_COLOR::setLEDColor( byte no, byte Ano,byte Bno ) {
  setLEDColorFB( no, Ano,FRONT );
  setLEDColorFB( no, Bno,BACK );
}
void  LED_CONTROL_FULL_COLOR::setLEDColorFB( byte no, byte r,byte g,byte b,byte fb ) {
  if( fb==FRONT ) {
    LEDFC[no].r=(unsigned int)r;  LEDFC[no].g=(unsigned int)g;  LEDFC[no].b=(unsigned int)b;
  } else {
    LEDFC[no].rb=(unsigned int)r;  LEDFC[no].gb=(unsigned int)g;  LEDFC[no].bb=(unsigned int)b;
  }
}
//-----------------------------------------------------------------
void  LED_CONTROL_FULL_COLOR::setLEDColorFB( byte no, byte cno , byte fb) {
  unsigned int r,g,b;
  switch( cno ) {
    case  LED_RED:
      r=255;g=0;b=0;
      break;
    case  LED_GREEN:
      r=0;g=255;b=00;
      break;
    case  LED_BLUE:
      r=0;g=0;b=255;
      break;
    case  LED_ORANGE:
      r=255;g=050;b=0; //オレンジ
      break;
    case  LED_PINK:
      r=233;g=10;b=80;//ピンク
      break;
    case  LED_YELLOW:
      r=255;g=128;b=0;  //黄
      break;
    case  LED_WHITE:
      r=255;g=255;b=255;  //白
      break;
    case  LED_BLACK:
      r=0;g=0;b=0;
      break;
    case  LED_SAKURA:
      r=255;  g=020;  b=040;
      break;
    case  LED_CYAN:
      r=0;  g=128;  b=255;
      break;
    case  LED_VIOLET:
      r=255;  g=0;  b=255;
      break;
    case  LED_PURPLE:
      r=150;  g=0;  b=30;
      break;
    case  LED_LIGHTGREEN:
      r=20;  g=70;  b=20;
      break;
    case  LED_LEMON:
      r=255;  g=255;  b=0;
      break;
  }
  if( fb==FRONT ) {
    LEDFC[no].r=r;  LEDFC[no].g=g;  LEDFC[no].b=b;
  } else {
    LEDFC[no].rb=r;  LEDFC[no].gb=g;  LEDFC[no].bb=b;
  }
}


