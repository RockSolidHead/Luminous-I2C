/*
  時間超過確認関数
  by RockSolidHead
*/


//  2^32-1
#define   COUNT_LIMIT             4294967295

//時刻超過確認用関数

boolean   IsOverTime( unsigned long  last_time, unsigned long diff_limit_time){
  unsigned long sss,ttt=0;
  boolean       ret,overflow_flag;

  overflow_flag=false;
  sss=millis();
  if(  ((signed long)(sss-last_time)) <0 ) {
    ttt=COUNT_LIMIT-last_time;
    overflow_flag=true;
  }
  if (overflow_flag) {
    if( (sss+ttt)>diff_limit_time )  {
      ret=true;
    } else {
      ret=false;
    }
  } else {
    if ((sss-last_time)>diff_limit_time)  {
      ret=true;
    } else {
      ret=false;
    }
  }
  return(ret);
}
