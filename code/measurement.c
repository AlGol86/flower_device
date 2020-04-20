#include "iostm8s103f3.h"
#include "measurement.h"
#include "wh1604a.h"


void init_ADC_56(void)
{
  PD_DDR&=~BIT_MASK_ch56 ;
  PD_CR1&=~BIT_MASK_ch56 ;
  PD_CR2&=~BIT_MASK_ch56 ;
  ADC_CR1_CONT=0;
  //ADC_CSR_CH=4;
  //ADC_CR2_ALIGN=1;
  //ADC_CR1_ADON=1;  
}
 
unsigned int get_ADC(char ch)
{

char h=0;
char l=0;
  ADC_CSR_CH=ch;
  ADC_CR1_ADON=1; 
  ADC_CR1_ADON=1; //dubble 'ADON" for switch on ADC
while(ADC_CSR_EOC==0)
  {asm("nop");}

ADC_CSR_EOC=0;
ADC_CR1_ADON=0;
h=ADC_DRH;
l=ADC_DRL;
return ((h<<2)|l); 
}

unsigned char get_ADC_8(char ch)
{

char h=0;
  ADC_CSR_CH=ch;
  ADC_CR1_ADON=1; 
  ADC_CR1_ADON=1; //dubble 'ADON" for switch on ADC
while(ADC_CSR_EOC==0)
  {asm("nop");}

ADC_CSR_EOC=0;
ADC_CR1_ADON=0;
h=ADC_DRH;
return (h); 
}

void write_eeprom(unsigned char* addr_eeprom,char data)///save to eeprom
{ asm("sim");
  	if (!((FLASH_IAPSR) & (MASK_FLASH_IAPSR_DUL)))
    {  // unlock EEPROM
       FLASH_DUKR = 0xAE;
       FLASH_DUKR = 0x56;
    }
  asm("rim");
       while (!((FLASH_IAPSR) & (MASK_FLASH_IAPSR_DUL))) {;}
       *addr_eeprom=data;
       FLASH_IAPSR &= ~(MASK_FLASH_IAPSR_DUL); // lock EEPROM
}
