#include "iostm8s103f3.h"
#include "wh1604a.h"
#include "rtc.h"
#include "main.h"
#include "serial24.h"
#include "measurement.h"

cyr_eeprom_struct s_cyr_eeprom[8] @0x4010; //store setting variables (in EEPROM)()
settings_struct set_s0            @0x4100; //store setting variables (in EEPROM)()
unsigned long int serial_data;
serial_SR_struct serial_SR;
sensor_struct sen_s;
buttons_struct but_s;


int main()
{
statement_t statement=main_screen;
init_serial ();
init_out ();
init_ADC_56();
init_tim4(); 
init1604();
init_rtc();
//init_aht10();
//get_aht10( &sen_s );




 while(1)
 {
  switch(statement){
    case   main_screen     :{statement=main_screen_handler();break;}
    case   pump_1   :{statement=pump_adjust_handler(0);break;}
    case   pump_2   :{statement=pump_adjust_handler(1);break;}
    case   pump_3   :{statement=pump_adjust_handler(2);break;}
    case   pump_4   :{statement=pump_adjust_handler(3);break;}
    case   pump_5   :{statement=pump_adjust_handler(4);break;}
    case   pump_6   :{statement=pump_adjust_handler(5);break;}
    case   lamp_adjust     :{statement=lamp_adjust_handler();break;}
    case   time_adjust     :{statement=time_adjust_handler();break;}
                   }
 if(but_s.return_to_main_screen==0) statement= main_screen;

 }
}
///////////////////////////////////////////////////////////////////////////////////////
/********************main screen*******************************************************/
////////////////////////////////////////////////////////////////////////////////////////
statement_t main_screen_handler(void) 
{
  extern settings_struct set_s0;
  settings_struct set_s;
  rtc_struct s_rtc;
  unsigned char lamp_enter=0;
  unsigned char pump_enter=0;
  unsigned char time_enter=0;
    
  char cursor=1;
  char print_f=15;
  clear();
  set_s=set_s0;
  but_s.return_to_main_screen=0;
  while(1)
  {
  
  if((but_s.return_to_main_screen==0)||(but_s.return_to_main_screen>2000))
  { write_wh_byte(0,0x0c);//off blink kursor
    //get_aht10( &sen_s );
    print_time_wh1604(2,1);print_date_wh1604(8,1);
    
   // print_n_00(sen_s.aht10_temp%100,2,2,2);set_cursor(4,2); print_char(223);//print_symb(&s_cyr_eeprom[0].deg,4,2,0);
   // print_n_00(sen_s.aht10_humid%100,2,6,2);set_cursor(8,2); print_char('%');
    set_cursor(9,2);
    print_string("  LUX="); print_n_00(sen_s.foto_level,2,15,2);set_cursor(17,2); print_char('%');
    set_cursor(2,3); print_string("LAMP:");
    set_cursor(12,3); print_string("PUMP:");
    set_cursor(2,4); print_string("SET:LAMP/PUMP/TIME");
    get_date(&s_rtc);
    get_time(&s_rtc);
    /////////////*******time correction***********//////////////
    if ((s_rtc.hr==0)&&(sen_s.complit_time_correction_f==0)) 
    {
      while(1)
      { get_time(&s_rtc);
        if(s_rtc.sec==set_s0.time_correction) break;
      }
    rtc_set_time(&s_rtc); 
    sen_s.complit_time_correction_f=1;
    }
    if (s_rtc.hr==1) sen_s.complit_time_correction_f=0;
       
    functional(&s_rtc);//assert condinions for manage electric equipment
    
  }
  else write_wh_byte(0,0x0d);//on  kursor
  ///********select params on main screen********************************////////////
  print_off_auto_on_adjustable(&set_s.state_lamp,7, 3,1,&cursor,2, &print_f );
  print_off_auto_on_adjustable(&set_s.state_pump,17,3,2,&cursor,1, &print_f );
  print_off_auto_on_adjustable(&lamp_enter,6,4,3,&cursor,1, &print_f );
  print_off_auto_on_adjustable(&pump_enter,11,4,4,&cursor,1, &print_f );
  print_off_auto_on_adjustable(&time_enter,16,4,5,&cursor,1, &print_f );
  if(cursor==0) cursor=5; if(cursor==6) cursor=1;
  if(set_s.state_lamp!=set_s0.state_lamp) write_eeprom(&set_s0.state_lamp,set_s.state_lamp);
  if(set_s.state_pump!=set_s0.state_pump) write_eeprom(&set_s0.state_pump,set_s.state_pump);
  if(lamp_enter)       
    return lamp_adjust; 
  else if (pump_enter) 
    return  pump_1; 
  else if (time_enter) 
    return  time_adjust; 
  }
  
}

void print_off_auto_on(char n) // indication for main screen
{
     switch(n)
   {
   case 0: {print_string("OFF ");break;}
   case 1: {print_string("AUTO");break;}
   case 2: {print_string("ON  ");break;}
   }
}
///////////////////////////////////////////////////////////////////////////////////////
/********************lamp_adjust screen************************************************/
////////////////////////////////////////////////////////////////////////////////////////
statement_t lamp_adjust_handler(void) 
{
  extern settings_struct set_s0;
  settings_struct set_s;
  char cursor=1;
  char print_f=30;
  clear();
  set_s=set_s0;
  

 
  write_wh_byte(0,0x0d);//on  kursor
  set_cursor(2,1); 
  print_string("LAMP ON-OFF TIME");
  for(char i=2;i<5;i++){  set_cursor(2,i); print_char(0x30+(i-1)); print_string(":   -          -");}
  
  while(1)
  {
   line_LAMP_menu(&set_s,0,1,&cursor,&print_f );
   line_LAMP_menu(&set_s,1,5,&cursor,&print_f );
   line_LAMP_menu(&set_s,2,9,&cursor,&print_f );

  
  
  if((cursor==0)||(cursor==13)) 
  {
    for(unsigned char* p=&set_s0.lamp_AM_ON[0];p<=&set_s0.lamp_PM_OFF[2];p++)
    {
    if(*p!=*(p-&set_s0.lamp_AM_ON[0]+&set_s.lamp_AM_ON[0])) 
      write_eeprom(p,*(p-&set_s0.lamp_AM_ON[0]+&set_s.lamp_AM_ON[0]));
    
    }
return (main_screen);
  }
  }
  
}

 void line_LAMP_menu(settings_struct* set_s,char n,char cursor1,char* cursor,char* print_f )
    {

  print_n_xx_adjustable_00(&set_s->lamp_AM_ON[n],5, n+2,cursor1,cursor,12, print_f );
  if (set_s->lamp_AM_OFF_lux_f[n]) {
    print_lux_adjustable (&set_s->lamp_AM_OFF[n],8,n+2,cursor1+1,cursor, print_f );
    if ((set_s->lamp_AM_OFF[n]==4)||(set_s->lamp_AM_OFF[n]==96)) {set_s->lamp_AM_OFF_lux_f[n]=0;set_s->lamp_AM_OFF[n]=10;}
  }
  else {
    print_n___ab_adjustable (&set_s->lamp_AM_OFF[n],8,n+2,cursor1+1,cursor,0,13, print_f );
    if ((set_s->lamp_AM_OFF[n]==0)||(set_s->lamp_AM_OFF[n]==13)) {set_s->lamp_AM_OFF_lux_f[n]=1;set_s->lamp_AM_OFF[n]=20;}
  }
  if (set_s->lamp_PM_ON_lux_f[n])  { 
    print_lux_adjustable (&set_s->lamp_PM_ON[n],14, n+2,cursor1+2,cursor, print_f );
    if ((set_s->lamp_PM_ON[n]==4)||(set_s->lamp_PM_ON[n]==96)) {set_s->lamp_PM_ON_lux_f[n]=0;set_s->lamp_PM_ON[n]=20;}
  }
  else {
    print_n___ab_adjustable (&set_s->lamp_PM_ON[n],14,n+2,cursor1+2,cursor,11,24, print_f );
    if ((set_s->lamp_PM_ON[n]==11)||(set_s->lamp_PM_ON[n]==24)) {set_s->lamp_PM_ON_lux_f[n]=1;set_s->lamp_PM_ON[n]=10;}
  }
  print_n_ab_adjustable (&set_s->lamp_PM_OFF[n],19,n+2,cursor1+3,cursor,12,24, print_f );
    }

///////////////////////////////////////////////////////////////////////////////////////
/********************pump_adjust screen************************************************/
////////////////////////////////////////////////////////////////////////////////////////
statement_t pump_adjust_handler(char n)
{
  extern settings_struct set_s0;
  settings_struct set_s;
  char cursor=1;
  char print_f=15;
  char f_2=n%2;
  clear();
  set_s=set_s0;
  set_cursor(2,1);  print_string("PUMP: ");print_char(0x31+n/2);
  for(char i=4;i<=(19-f_2);i+=5) {set_cursor(i,3);print_char(223);print_char(223);set_cursor(i,4);print_char('s');}
  set_cursor(2,2);
  if((f_2)==0)  for(char i=1;i<5;i++) {print_day_of_week(i);print_char(' ');print_char(' ');}
   else for(char i=5;i<8;i++) {print_day_of_week(i);print_char(' ');print_char(' ');}
  
  while(1)
  {
    if((f_2)==0) 
      for(char i=0;i<4;i++) {
       print_n_xx_adjustable_00(&set_s.pump_1[i]+7*(n/2),2+i*5, 3, i+1,&cursor,23,&print_f );
       print_n_xx_adjustable_00(&set_s.pump_1s[i]+7*(n/2),2+i*5, 4, i+5,&cursor,99,&print_f );  
       }
    else 
      for(char i=4;i<7;i++) {
       print_n_xx_adjustable_00(&set_s.pump_1[i]+7*(n/2),2+(i-4)*5, 3, i-3,&cursor,23,&print_f );
       print_n_xx_adjustable_00(&set_s.pump_1s[i]+7*(n/2),2+(i-4)*5, 4, i,&cursor,99,&print_f );  
       }
    
    if((f_2==0)&&((cursor==0)||(cursor==9)))  {
              
        for(char i=0;i<4;i++){
         if(*(&set_s.pump_1[i]+7*(n/2))!=*(&set_s0.pump_1[i]+7*(n/2))) 
           write_eeprom(&set_s0.pump_1[i]+7*(n/2),*(&set_s.pump_1[i]+7*(n/2)));
         if(*(&set_s.pump_1s[i]+7*(n/2))!=*(&set_s0.pump_1s[i]+7*(n/2))) 
           write_eeprom(&set_s0.pump_1s[i]+7*(n/2),*(&set_s.pump_1s[i]+7*(n/2)));
        }
        if(cursor==0) return (pump_1+n-1); 
           else return (pump_1+n+1);
        }
  
   
     if((f_2==1)&&((cursor==0)||(cursor==7))){

         for(char i=4;i<7;i++){
         if(*(&set_s.pump_1[i]+7*(n/2))!=*(&set_s0.pump_1[i]+7*(n/2))) 
           write_eeprom(&set_s0.pump_1[i]+7*(n/2),*(&set_s.pump_1[i]+7*(n/2)));
         if(*(&set_s.pump_1s[i]+7*(n/2))!=*(&set_s0.pump_1s[i]+7*(n/2))) 
           write_eeprom(&set_s0.pump_1s[i]+7*(n/2),*(&set_s.pump_1s[i]+7*(n/2)));
        }
       if(cursor==0) return (pump_1+n-1); 
           else if (n!=5) return (pump_1+n+1); else return (main_screen);
      }
  
  }
}


///////////////////////////////////////////////////////////////////////////////////////
/********************time_adjust screen************************************************/
////////////////////////////////////////////////////////////////////////////////////////

statement_t time_adjust_handler(void)
{ clear();
  rtc_adjust_time(2,1);
  return (main_screen);
}

///////////////////////////////////////////////////////////////////////////////////////
/********************functional*******************************************************/
////////////////////////////////////////////////////////////////////////////////////////

void functional(rtc_struct* s_rtc)
{
 extern unsigned long int serial_data;
 extern settings_struct set_s0;
 extern sensor_struct sen_s;
 extern serial_SR_struct serial_SR;
 int over_sec=(s_rtc->min*60)+s_rtc->sec;

 /**********LAMPS****************************/
 if(set_s0.state_lamp==0) serial_data&=~(unsigned long int)(b_l1|b_l2|b_l3);
 else if(set_s0.state_lamp==2) serial_data|=b_l1|b_l2|b_l3;
 else  
   for (char i=0;i<3;i++) {
     if((s_rtc->hr >= set_s0.lamp_AM_ON[i])&&(s_rtc->hr < set_s0.lamp_PM_OFF[i]))    sen_s.lamp_ON[i]=1; 
     else                                                                            sen_s.lamp_ON[i]=0; 
     
     if ((set_s0.lamp_AM_OFF_lux_f[i]==1)&&(sen_s.foto_level>set_s0.lamp_AM_OFF[i]))  sen_s.lamp_day_ON[i]=0;
     if ((set_s0.lamp_AM_OFF_lux_f[i]==0)&&(s_rtc->hr >= set_s0.lamp_AM_OFF[i]))      sen_s.lamp_day_ON[i]=0;     

   
     
     if ((set_s0.lamp_PM_ON_lux_f[i]==1)&&(sen_s.foto_level<set_s0.lamp_PM_ON[i]))    sen_s.lamp_day_ON[i]=1;
     if ((set_s0.lamp_PM_ON_lux_f[i]==0)&&(s_rtc->hr  >= set_s0.lamp_PM_ON[i]))       sen_s.lamp_day_ON[i]=1;

     if (sen_s.lamp_ON[i]&&sen_s.lamp_day_ON[i])   serial_data|=b_l1>>i;  ///LAMP ON
     else                                       serial_data&=~(unsigned long int)(b_l1>>i);  ///LAMP OFF
    }   
 
 if(serial_data & b_l1)  PORT_L1_ODR |=BIT_L1;
 else                    PORT_L1_ODR &=~BIT_L1;
 if(serial_data & b_l2)  PORT_L2_ODR |=BIT_L2;
 else                    PORT_L2_ODR &=~BIT_L2;
 if(serial_data & b_l3)  PORT_L3_ODR |=BIT_L3;
 else                    PORT_L3_ODR &=~BIT_L3;
  /**********PUMPS****************************/
 if(s_rtc->hr==24) s_rtc->hr=0;
   for (char i=0;i<3;i++) {
     serial_data|=b_ws1>>i;
     serial_SR.EOT=0;
     while(!serial_SR.EOT){}
     sen_s.wl_adc[i]=get_ADC_8(ch_adc_wl);
     serial_data&=~(unsigned long int)(b_ws1>>i);
     if ((sen_s.wl_adc[i]<20)&&(sen_s.wl_cnt[i]<20)) sen_s.wl_cnt[i]++;
     else if ((sen_s.wl_adc[i]>25)&&(sen_s.wl_cnt[i]>0)) sen_s.wl_cnt[i]--;
     }
     
  if(set_s0.state_pump==0) serial_data&=~(unsigned long int)(b_p1|b_p2|b_p3);
  else 
    for (char i=0;i<3;i++) {
      if(sen_s.wl_cnt[i]<10) { 
        serial_data&=~(unsigned long int)(b_il1>>i);
        if(((*(&set_s0.pump_1[s_rtc->day_week-1]+i*7))==s_rtc->hr)&&(over_sec<(*(&set_s0.pump_1s[s_rtc->day_week-1]+i*7)))) serial_data|=b_p1>>i;
        else serial_data&=~(unsigned long int)(b_p1>>i);
       }
      else {
      serial_data|=b_il1>>i;
      serial_data&=~(unsigned long int)(b_p1>>i);//PUMP OFF(iow level of water)
      }
    }
}
