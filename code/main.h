#include "iostm8s103f3.h"
#include "rtc.h"

//lamps
#define PORT_L1_ODR  PC_ODR
#define PORT_L1_DDR  PC_DDR
#define PORT_L1_CR1  PC_CR1
#define PIN_L1   7 
#define BIT_L1 (1<<PIN_L1)
#define PORT_L2_ODR  PD_ODR
#define PORT_L2_DDR  PD_DDR
#define PORT_L2_CR1  PD_CR1
#define PIN_L2   1 
#define BIT_L2 (1<<PIN_L2)
#define PORT_L3_ODR  PD_ODR
#define PORT_L3_DDR  PD_DDR
#define PORT_L3_CR1  PD_CR1
#define PIN_L3   2 
#define BIT_L3 (1<<PIN_L3)

//buttons+fotosensor
#define PORT_buttons_foto_DDR PD_DDR
#define PORT_buttons_foto_ODR PD_ODR
#define PORT_buttons_foto_IDR PD_IDR
#define PORT_buttons_foto_CR1 PD_CR1
#define PIN_buttons_foto  5 
#define BIT_buttons_foto (1<<PIN_buttons_foto)
//water level sensors

#define PORT_wls_DDR PD_DDR
#define PORT_wls_IDR PD_IDR
#define PORT_wls_CR1 PD_CR1
#define PIN_wls     6 
#define BIT_wls (1<<PIN_wls)


#ifndef MAIN
#define MAIN

typedef enum  {main_screen,pump_1,pump_2,pump_3,pump_4,pump_5,pump_6,lamp_adjust,time_adjust} statement_t;


typedef struct
{
 unsigned char state_lamp ;
 unsigned char state_pump ;
 unsigned char lamp_AM_ON[3];
 unsigned char lamp_AM_OFF[3];
 unsigned char lamp_AM_OFF_lux_f[3] ;
 unsigned char lamp_PM_ON[3]; 
 unsigned char lamp_PM_ON_lux_f[3]  ;
 unsigned char lamp_PM_OFF[3];

 
 unsigned char pump_1[7];
 unsigned char pump_2[7];
 unsigned char pump_3[7];
 unsigned char pump_1s[7];
 unsigned char pump_2s[7];
 unsigned char pump_3s[7];
 unsigned char time_correction;
 unsigned char last_day_correction; 
 
} settings_struct;

typedef struct
{
 unsigned char EOT       :1; //end of transmission
  
  
} serial_SR_struct;

typedef struct
{
 unsigned char tik_n_button   ; //left-1 right-2 up-3 down-4
 unsigned char n_button       ; //left-1 right-2 up-3 down-4
 unsigned char f_zero_button_cnt     :1; //down
 unsigned char button_cnt     ;
 unsigned char switch_cnt     ;
 unsigned int return_to_main_screen;
} buttons_struct;
#endif

#pragma vector = 25
__interrupt void time4(void);

void init_tim4(void);
void print_n_xx_adjustable_00(unsigned char* n, char x, char y, char marker, char* cursor , char lim, char* print_f );
void print_n_ab_adjustable(unsigned char* n,char x, char y, char marker, char* cursor, char a, char b, char* print_f );
void print_n___ab_adjustable(unsigned char* n,char x, char y, char marker, char* cursor,  char a, char b,  char* print_f );
void print_lux_adjustable(unsigned char* n,char x, char y, char marker, char* cursor,  char* print_f );
void print_day_of_week_adjustable(unsigned char* n,char x, char y, char marker, char* cursor, char* print_f );
void print_off_auto_on_adjustable(unsigned char* n,char x, char y, char marker, char* cursor, char lim, char* print_f );
char buttons_handler(unsigned char* n, char marker, char* cursor,  char preset ,  char lim  );

statement_t main_screen_handler(void) ;
void print_off_auto_on(char n);

statement_t lamp_adjust_handler(void) ;
void line_LAMP_menu(settings_struct* set_s,char n,char cursor1,char* cursor,char* print_f );

statement_t pump_adjust_handler(char n);

statement_t time_adjust_handler(void);

void functional(rtc_struct* s_rtc);
