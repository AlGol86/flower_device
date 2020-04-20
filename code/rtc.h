#define YEAR_10_TIME   ((YEAR_TIME%100)/10)
#define YEAR_1_TIME    (YEAR_TIME%10)
#define MN_TH_ST        WEEK_TIME
#define MONTH_10_TIME   (MONTH_TIME/10)
#define MONTH_1_TIME    (MONTH_TIME%10)
#define DATE_10_TIME    (DATE_TIME/10)
#define DATE_1_TIME     (DATE_TIME%10)
#define HR_10_TIME      (HOURS_TIME/10)
#define HR_1_TIME       (HOURS_TIME%10)
#define MIN_10_TIME     (MINUTES/10) 
#define MIN_1_TIME      (MINUTES%10) 

#define PIN_dat_rtc     4   //port D
#define PIN_clk_rtc     3   //port D
#define PIN_rst_rtc     1   //port A
#define PORT_rtc_dat_clk_IDR PD_IDR
#define PORT_rtc_dat_clk_ODR PD_ODR
#define PORT_rtc_dat_clk_DDR PD_DDR
#define PORT_rtc_dat_clk_CR1 PD_CR1
#define PORT_rtc_dat_clk_CR2 PD_CR2
#define PORT_rtc_rst_IDR PA_IDR
#define PORT_rtc_rst_ODR PA_ODR
#define PORT_rtc_rst_DDR PA_DDR
#define PORT_rtc_rst_CR1 PA_CR1
#define PORT_rtc_rst_CR2 PA_CR2
#define BIT_dat_rtc    (1<<PIN_dat_rtc)
#define BIT_clk_rtc    (1<<PIN_clk_rtc)
#define BIT_rst_rtc    (1<<PIN_rst_rtc)


#define SEC   0
#define MIN   1
#define HR    2
#define DATE  3
#define MONTH 4
#define DAY   5
#define YEAR  6
#define CONTR 7
#define TR_CH 8
#define WHOLE 3

#ifndef RTC
#define RTC
typedef struct
{
  unsigned char hr     ;
  unsigned char min    ;
  unsigned char sec    ;
  unsigned char day_week ;
  unsigned char day    ;
  unsigned char month ; 
  unsigned char year   ;//0..99

} rtc_struct;
#endif

void delay_rtc(char del );
void rtc_set_time(rtc_struct* rtc_s);
void init_rtc(void);
void send_byte_rtc(char data);
char receive_byte_rtc(void);
void send_rtc(char reg_adr, char data);
char receive_rtc(char reg_adr);
void get_time (rtc_struct* s_rtc);
void get_date (rtc_struct* s_rtc);
void print_time_wh1604(char x,char y);
void print_date_wh1604(char x,char y);
void print_day_of_week(char day_week);
void rtc_adjust_time(char x,char y);
