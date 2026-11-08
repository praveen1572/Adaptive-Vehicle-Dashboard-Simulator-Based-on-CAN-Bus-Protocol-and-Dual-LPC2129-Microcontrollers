#include"header.h"
void delay_us(u32 us){
T0PR=60-1;
T0TC=0;
T0PC=0;
T0TCR=2;
T0TCR=1;
while(T0TC<us);
T0TCR=0;
}

void delay_ms(u32 ms){
T0PR=60000-1;
T0TC=0;
T0PC=0;
T0TCR=2;
T0TCR=1;
while(T0TC<ms);
T0TCR=0;
}

void delay_sec(u32 sec){
T0PR=60000000-1;
T0TC=0;
T0PC=0;
T0TCR=2;
T0TCR=1;
while(T0TC<sec);
T0TCR=0;
}
