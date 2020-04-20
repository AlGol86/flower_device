#include "iostm8s103f3.h"

//#define PORT_EN_DDR  PA_DDR
//#define PORT_EN_ODR  PA_ODR
//#define PORT_EN_CR1  PA_CR1
//#define PIN_en       3 
//#define BIT_en (1<<PIN_en)


#define N_F_code   2 //n:2-line f:5*11font
#define SC_RL_code 2 //sc:setcursor/display moving rl:right/left
#define D_C_B_code 4 //display/cursor/blinking
#define ID_SH_code 2 //cursor mov/display shift

void write_wh(char rs,char db7_db4);
void write_wh_byte(char rs,char db7_db0);
void write_wh_digit(char n);
void del_WH(unsigned int del);
void init1604(void);
void print_string(char* pointer); 
void clear(void);
void set_cursor(char x,char y);
void print_char(char ch);
char print_n_01(unsigned int n, char x, char y ); //n=0..65535 right align x-right position
void print_n_00(unsigned int n, char nn, char x, char y ); //n=0..65535 fixed format - nn diges x-left pos


