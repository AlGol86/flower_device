#include "iostm8s103f3.h"
#include "wh1604a.h"
#include "serial24.h"
#include "main.h"

void del_WH(unsigned int del)
{
  for(unsigned int j=0;j<=del;j++){}
}

void init1604(void)
{
  extern unsigned long int serial_data;
  //PORT_EN_ODR&=~BIT_en;
 // PORT_EN_DDR|=BIT_en;
 // PORT_EN_CR1|=BIT_en;
  serial_data&=~(b_wh_rs|b_wh_en);
  serial_data|=b_wh_light;
  serial_data&=~((unsigned long int)0xf<<wh_offset);
  send_serial (serial_data);
  del_WH(30000);
  write_wh(0,0x30);
  del_WH(10000);
  write_wh(0,0x30);
  del_WH(100);
  write_wh(0,0x30);
  write_wh(0,0x20);
  write_wh_byte(0,0x20|N_F_code<<2);
  write_wh_byte(0,8|(D_C_B_code));
  clear();
  write_wh_byte(0,4|ID_SH_code);
  clear();
  set_cursor(1,1);
}

void clear(void)
{
  write_wh_byte(0,1);
  del_WH(2000);
}

void set_cursor(char x,char y)
{
  x--;
  switch(y)
{
case 1: {write_wh_byte(0,0x80|0x0|x);break;} 
case 2: {write_wh_byte(0,0x80|0x40|x);break;}
case 3: {write_wh_byte(0,0x80|(0x14+x));break;}
case 4: {write_wh_byte(0,0x80|(0x54+x));break;}
}
}

void write_wh(char rs,char db7_db4)// rs=0-instruction; rs=1-data (write half of byte)
{
  extern unsigned long int serial_data;
  extern serial_SR_struct serial_SR;
  db7_db4>>=4;
  serial_data&=~((unsigned long int)0xf<<wh_offset);
  serial_data|=(unsigned long int)db7_db4<<wh_offset;
  if (rs) serial_data|=b_wh_rs;
  else serial_data&=~b_wh_rs;
  serial_data|=b_wh_en;
  serial_SR.EOT=0;
  while(!serial_SR.EOT){}
  serial_data&=~b_wh_en;
  serial_SR.EOT=0;
  while(!serial_SR.EOT){}
 // PORT_EN_ODR|=BIT_en;
 // asm("nop");
 // PORT_EN_ODR&=~BIT_en;
}

void write_wh_byte(char rs,char db7_db0)
{
  write_wh(rs,db7_db0);
  write_wh(rs,db7_db0<<4);
  del_WH(20);
}

void print_wh_digit(char n)
{
write_wh_byte(1,0x30+n);
}

void print_char(char ch) 
{
  write_wh_byte(1,ch);
}

void print_string(char* pointer) //print eng_litters string
{
while (*pointer!=0)
{
write_wh_byte(1,*pointer);
pointer++;
}
}

char print_n_01(unsigned int n, char x, char y ) //n=0..65535 right align x-right position
{ 
do
{
set_cursor(x,y);
print_wh_digit(n%10); 
n/=10;
x--;
}
while(n>0); 
return (x);
}

void print_n_00(unsigned int n, char nn, char x, char y ) //n=0..65535 fixed format - nn diges x-left pos
{
char pos; 
pos=print_n_01(n,x+nn-1,y);
while(pos>=x) { set_cursor(pos,y); print_char('0');pos--;}
}
