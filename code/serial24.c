#include "iostm8s103f3.h"
#include "serial24.h"
#include "main.h"

void init_out (void)
{
    PORT_L1_ODR &=~BIT_L1;
  PORT_L1_DDR |=BIT_L1;
  PORT_L1_CR1 |=BIT_L1;
    PORT_L2_ODR &=~BIT_L2;
  PORT_L2_DDR |=BIT_L2;
  PORT_L2_CR1 |=BIT_L2;
    PORT_L3_ODR &=~BIT_L3;
  PORT_L3_DDR |=BIT_L3;
  PORT_L3_CR1 |=BIT_L3;
  
}
void init_serial (void)
{
  PORT_serial_ODR&=~BIT_serial_data;
  PORT_serial_ODR&=~BIT_serial_latch;
  PORT_serial_ODR|=BIT_serial_reset;
  PORT_serial_ODR|=BIT_serial_clk;
  
  PORT_serial_DDR|=BIT_serial_mask;
  PORT_serial_CR1|=BIT_serial_mask;
}

void send_serial (unsigned long int data)
{
  PORT_serial_ODR&=~BIT_serial_reset; 
  PORT_serial_ODR|=BIT_serial_reset; 
  for(char i=0;i<24;i++)
  {
  if(data&1) PORT_serial_ODR|=BIT_serial_data; 
  else PORT_serial_ODR&=~BIT_serial_data; 
  data>>=1;
  PORT_serial_ODR&=~BIT_serial_clk;
  PORT_serial_ODR|=BIT_serial_clk;
  }
  PORT_serial_ODR|=BIT_serial_latch;
  PORT_serial_ODR&=~BIT_serial_latch;
 }
