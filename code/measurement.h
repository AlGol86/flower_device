
#include "iostm8s103f3.h"

#define PIN_ADC_chanal_5     5  //port D(ch#5)
#define ch_adc_foto          5
#define BIT_ADC_chanal_5    (1<<PIN_ADC_chanal_5 )
#define PIN_ADC_chanal_6     6  //port D(ch#6)
#define ch_adc_wl            6
#define BIT_ADC_chanal_6    (1<<PIN_ADC_chanal_6 )
#define BIT_MASK_ch56      (BIT_ADC_chanal_5|BIT_ADC_chanal_6)

#ifndef MEASUREMENT
#define MEASUREMENT
typedef struct
{
 unsigned char complit_time_correction_f;
 unsigned char lamp_ON[3] ;
 unsigned char lamp_day_ON[3] ;
 unsigned char wl_adc[3]     ;
 unsigned char wl_cnt[3]     ;
 unsigned char foto_level   ;
} sensor_struct;
#endif


void init_ADC_56(void);
unsigned char get_ADC_8(char ch);
unsigned int get_ADC(char ch);
void write_eeprom(unsigned char* addr_eeprom,char data);///save to eeprom
