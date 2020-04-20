//this example is intended for 8 MHz clock frequency
#include "iostm8s103f3.h"
#include "rtc.h"
#include "wh1604a.h"
#include "main.h"
#include "measurement.h"

void delay_rtc(char del )
{
char i;
for(i=0;i<(del);i++)
{asm("nop");}
}

void rtc_set_time(rtc_struct* rtc_s)
{
 send_rtc(CONTR,0);  //WRITE-PROTECT: OFF
 send_rtc(TR_CH,0);    //TRICKLE CHARGER: OFF
 send_rtc(YEAR,  rtc_s->year); 
 send_rtc(DAY,   rtc_s->day_week);    //DAY OF WEEK
 send_rtc(MONTH, rtc_s->month); 
 send_rtc(DATE,  rtc_s->day); 
 send_rtc(HR,    rtc_s->hr); 
 send_rtc(MIN,   rtc_s->min); 
 send_rtc(SEC,0x00);
}


void init_rtc(void)
{
  PORT_rtc_dat_clk_CR2&=~BIT_clk_rtc ; // low speed (interrupt disabled)
  PORT_rtc_rst_CR2&=~BIT_rst_rtc ; // low speed (interrupt disabled)
  PORT_rtc_dat_clk_CR1|=BIT_clk_rtc ; // open drain /float in inp. direct
  PORT_rtc_rst_CR1|=BIT_rst_rtc ; // open drain /float in inp. direct
  PORT_rtc_dat_clk_ODR&=~BIT_clk_rtc ;  //clk_pin, rst_pin  - 0 
  PORT_rtc_rst_ODR&=~BIT_rst_rtc ;  //clk_pin, rst_pin  - 0 
  PORT_rtc_dat_clk_DDR|=BIT_clk_rtc ; 
  PORT_rtc_rst_DDR|=BIT_rst_rtc ; 
    
  PORT_rtc_dat_clk_CR2&=~BIT_dat_rtc; //dat_pin  - low speed (interrupt disabled)
  PORT_rtc_dat_clk_CR1|=BIT_dat_rtc; //dat_pin - open drain /float in inp. direct
  PORT_rtc_dat_clk_ODR|=BIT_dat_rtc;  //dat_pin  - 1 (SDA)
  PORT_rtc_dat_clk_DDR|=BIT_dat_rtc; 
 }

void send_byte_rtc(char data)
{
 char a=data; 
for(char i=1;i<=8;i++)
{
PORT_rtc_dat_clk_ODR&=((data&1)<<PIN_dat_rtc)|(~BIT_dat_rtc);
data>>=1;
PORT_rtc_dat_clk_ODR|=BIT_clk_rtc; // clk=1
asm("nop");
PORT_rtc_dat_clk_ODR|=BIT_dat_rtc; // sda=1
PORT_rtc_dat_clk_ODR&=~BIT_clk_rtc;// clk=0
for (char i=a;i<1;i++){};
}
}

char  receive_byte_rtc(void)
{
char data=0;
PORT_rtc_dat_clk_CR1&=~BIT_dat_rtc;
PORT_rtc_dat_clk_DDR&=~BIT_dat_rtc;
for(char i=0;i<=7;i++)
{
asm("nop");
PORT_rtc_dat_clk_ODR|=BIT_clk_rtc; // clk=1
data|=((PORT_rtc_dat_clk_IDR&BIT_dat_rtc)>>PIN_dat_rtc)<<i;
asm("nop");
PORT_rtc_dat_clk_ODR&=~BIT_clk_rtc;// clk=0
asm("nop");
}
PORT_rtc_dat_clk_CR1|=BIT_dat_rtc;
PORT_rtc_dat_clk_DDR|=BIT_dat_rtc;
return (data);
}

void send_rtc(char reg_adr, char data)
{ 
  delay_rtc(3); //9*0.5us 
  PORT_rtc_rst_ODR|=BIT_rst_rtc; //rst=1(>4us)
  delay_rtc(3); //9*0.5us 
  reg_adr<<=1;
  reg_adr+=0x80;
  send_byte_rtc(reg_adr);
  asm("nop");
  send_byte_rtc(((data/10)<<4)+(data%10));
  PORT_rtc_rst_ODR&=~BIT_rst_rtc; //rst=0
}

char receive_rtc(char reg_adr)
{
  char receive_data=0xff;
  delay_rtc(3); //9*0.5us 
  PORT_rtc_rst_ODR|=BIT_rst_rtc; //rst=1(>4us)
  delay_rtc(3); //9*0.5us 
  reg_adr<<=1;
  reg_adr+=0x81;
  send_byte_rtc(reg_adr);
  receive_data=receive_byte_rtc();
  PORT_rtc_rst_ODR&=~BIT_rst_rtc; //rst=0
  return (10*(receive_data>>4)+(receive_data&0x0f));
}

void get_date(rtc_struct* s_rtc)
{ 
  s_rtc->day=receive_rtc(DATE);
  s_rtc->day_week=receive_rtc(DAY);
  s_rtc->month=receive_rtc(MONTH);
  s_rtc->year=receive_rtc(YEAR);
 }

void get_time (rtc_struct* s_rtc)
{ 
  s_rtc->hr =receive_rtc(HR);
  s_rtc->min=receive_rtc(MIN);
  s_rtc->sec=receive_rtc(SEC);
}

void print_time_wh1604(char x,char y)
{
  rtc_struct s_rtc;
  get_time (&s_rtc);
  print_n_00(s_rtc.hr, 2,  x,  y );
  set_cursor(x+2,y);
  if(s_rtc.sec&1) print_char(':'); else print_char(' ');
  print_n_00(s_rtc.min, 2,  x+3,  y );
}

void print_date_wh1604(char x,char y)
{
  rtc_struct s_rtc;
  get_date (&s_rtc);
  print_n_00(s_rtc.day,  2,  x, y );
  set_cursor(x+2,y);  
  print_char('/');  
  print_n_00(s_rtc.month,2,x+3, y );  
  set_cursor(x+5,y);  
  print_char('/'); 
  print_n_00(s_rtc.year, 2,x+6, y );  
  set_cursor(x+8,y);  
  print_char(' '); 
  print_day_of_week(s_rtc.day_week);
  }

void print_day_of_week(char day_week)
 {
   switch(day_week)
   {
   case 1: {print_string("MON");break;}
   case 2: {print_string("TUE");break;}
   case 3: {print_string("WED");break;}
   case 4: {print_string("THU");break;}
   case 5: {print_string("FRI");break;}
   case 6: {print_string("SAT");break;}
   case 7: {print_string("SAN");break;}
   }
 }

void rtc_adjust_time(char x,char y)
{ rtc_struct s_rtc;
  extern settings_struct set_s0;
  unsigned char correction;
  char cursor=1;
  char print_f=15;
  get_time (&s_rtc);
  get_date (&s_rtc);
  correction=set_s0.time_correction;
  set_cursor(x+2,y);
  print_char(':');
  set_cursor(x+8,y);
  print_char('/');
  set_cursor(x+11,y);
  print_char('/');
  set_cursor(x,y+1);
  print_string("correction=  s/day");
  write_wh_byte(0,0x0e);//on blink kursor
  while(1)
  {
    if((cursor==0)||(cursor==8)) break;
        print_n_xx_adjustable_00(&s_rtc.hr,x,   y,1,&cursor,23,&print_f);
        print_n_xx_adjustable_00(&s_rtc.min,x+3,y,2,&cursor,59,&print_f);
        print_n_xx_adjustable_00(&s_rtc.day,x+6,y,3,&cursor,31,&print_f);
        print_n_xx_adjustable_00(&s_rtc.month,x+9,y,4,&cursor,12,&print_f);
        print_n_xx_adjustable_00(&s_rtc.year,x+12,y,5,&cursor,99,&print_f);
        print_day_of_week_adjustable(&s_rtc.day_week,x+15,y,6,&cursor,&print_f);
        print_n_xx_adjustable_00(&correction,x+11,y+1,7,&cursor,99,&print_f);
  }
  if(correction!=set_s0.time_correction)
           write_eeprom(&set_s0.time_correction,correction);
  write_eeprom(&set_s0.last_day_correction,s_rtc.day);
  write_wh_byte(0,0x0c);//off blink kursor
  rtc_set_time(&s_rtc);      
}
