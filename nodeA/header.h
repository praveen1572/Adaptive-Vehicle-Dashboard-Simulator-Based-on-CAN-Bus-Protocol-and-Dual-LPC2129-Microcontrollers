#include <lpc21xx.h>
#include <stdio.h>

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;
typedef signed int s32;

typedef struct CAN1_MSG {
    u32 id;
    u32 byteA;
    u32 byteB;
    u8 rtr;
    u8 dlc;
    u8 ff;
} CAN1;

extern volatile u32 right_flag,left_flag,head_flag,flag;
extern volatile u32 left_indi,right_indi,head_light;
extern CAN1 r1;

void delay_sec(unsigned int sec);
void delay_ms(unsigned int ms);
void delay_us(u32 us);

void adc_init(void);
u32 adc_read(u32 channel);
float adc_to_temp(u32 adc);
float adc_to_speed(u32 adc);
int dis_to_fuel(u32 adc);

void uart0_init(unsigned int baud);
void uart0_tx(unsigned char data);
unsigned char uart0_rx(void);
void uart0_tx_string(char *ptr);
void uart0_binary(int num);
void uart0_integer(int num);
void uart0_float(float num);

void can1_rx(CAN1 *ptr);
void can1_tx(CAN1 v);
void can1_init(void);
void config_vic_for_can1(void);

void config_ext_intrpt0(void);
void config_vic_for_ext_intrpt0(void);
void config_ext_intrpt1(void);
void config_vic_for_ext_intrpt1(void);
void config_ext_intrpt2(void);
void config_vic_for_ext_intrpt2(void);

