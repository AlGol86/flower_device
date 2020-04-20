#include "iostm8s103f3.h"

//serial output
#define PORT_serial_ODR      PC_ODR
#define PORT_serial_DDR      PC_DDR
#define PORT_serial_CR1      PC_CR1
#define PIN_serial_data  3 
#define BIT_serial_data (1<<PIN_serial_data)
#define PIN_serial_latch 4 
#define BIT_serial_latch (1<<PIN_serial_latch)
#define PIN_serial_clk   5 
#define BIT_serial_clk (1<<PIN_serial_clk)
#define PIN_serial_reset 6 
#define BIT_serial_reset (1<<PIN_serial_reset)
#define BIT_serial_mask (BIT_serial_reset|BIT_serial_clk|BIT_serial_latch|BIT_serial_data)

//wh1604
#define b_wh_en    0x100000
#define b_wh_rs    0x200000
#define b_wh_light 0x800000
#define b_foto     0x400000
#define wh_offset   16
//lamps
#define b_l1       0x8000
#define b_l2       0x4000
#define b_l3       0x2000
//pumps
#define b_p1       0x1000
#define b_p2       0x800
#define b_p3       0x400
//water level indicators
#define b_il1       0x200
#define b_il2       0x100
#define b_il3       0x80
//buttons
#define b_button1       0x40
#define b_button2       0x20
#define b_button3       0x10
#define b_button4       0x8
#define b_button_foto_mask  (b_button1|b_button2|b_button3|b_button4|b_foto)
//water sensors
#define b_ws1       0x4
#define b_ws2       0x2
#define b_ws3       0x1

void init_out (void);
void init_serial (void);
void send_serial (unsigned long int data);
