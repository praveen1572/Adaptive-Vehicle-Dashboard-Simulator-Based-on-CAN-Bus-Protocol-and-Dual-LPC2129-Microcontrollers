#include"header.h"
void adc_init(void){
PINSEL1|=0x15400000;
ADCR=0x00200400;
}

u32 adc_read(u32 channel){
ADCR|=1<<channel;
ADCR|=1<<24;
while(((ADDR>>31)&1)==0);
ADCR^=1<<24;
ADCR^=1<<channel;
return ((ADDR>>6)&0X3ff);
}

float adc_to_temp(u32 adc){
	float temp=(adc*3.3)/1023;
	return temp/0.01;
}

float adc_to_speed(u32 adc){
	float temp=(adc*120)/1023;
	return temp;
}

int dis_to_fuel(u32 adc){//1cm=10 lit
	int fuel=50-adc*10;
	if(fuel<0)
        fuel=0;
	return fuel;
}

