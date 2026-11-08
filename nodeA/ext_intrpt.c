#include"header.h"
extern CAN1	v1;
volatile u32 right_flag=0,left_flag=0,head_flag=0;

//head light intrpt0 p0.16
//head light p0.21
void config_ext_intrpt0(void){
PINSEL1|=1<<0;
EXTMODE|=0X7;
EXTPOLAR|=0;
}

void ext_intrpt0_handler(void)__irq{
head_flag^=1;
if(head_flag) IOCLR0=1<<21;
else IOSET0=1<<21;
EXTINT=1<<0;
VICVectAddr=0;
}

void config_vic_for_ext_intrpt0(){
VICIntSelect=0;
VICVectCntl0=14|1<<5;
VICVectAddr0=(unsigned int)ext_intrpt0_handler;
VICIntEnable=1<<14;
}

//left indicator intrpt1 0.14
//left indicator 0.19
void config_ext_intrpt1(void){
PINSEL0|=1<<29;
EXTMODE|=0X7;
EXTPOLAR|=0;
}

void ext_intrpt1_handler(void)__irq{
while((IODIR0>>14)&1);
IOSET0=1<<20;
right_flag=0;
left_flag^=1;
if(left_flag) IOCLR0=1<<19;
else IOSET0=1<<19;
EXTINT=1<<1;
VICVectAddr=0;
}

void config_vic_for_ext_intrpt1(){
VICIntSelect=0;
VICVectCntl1=15|1<<5;
VICVectAddr1=(unsigned int)ext_intrpt1_handler;
VICIntEnable=1<<15;
}

//right indicator intrpt2 p0.15
//right indicator p0.20
void config_ext_intrpt2(void){
PINSEL0|=(unsigned)1<<31;
EXTMODE|=0X7;
EXTPOLAR|=0;
}

void ext_intrpt2_handler(void)__irq{
IOSET0=1<<19;
left_flag=0;
right_flag^=1;
if(right_flag) IOCLR0=1<<20;
else IOSET0=1<<20;
EXTINT=1<<2;
VICVectAddr=0;
}

void config_vic_for_ext_intrpt2(){
VICIntSelect=0;
VICVectCntl2=16|1<<5;
VICVectAddr2=(unsigned int)ext_intrpt2_handler;
VICIntEnable=1<<16;
}
