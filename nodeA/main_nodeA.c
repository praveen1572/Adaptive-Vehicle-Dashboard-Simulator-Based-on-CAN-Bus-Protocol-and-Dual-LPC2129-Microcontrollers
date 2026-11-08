#include "header.h"
// temp p0.27
// pot p0.28
// trigger p0.17
// echo p0.18
// left indicator intrpt1 0.14
// right indicator intrpt2 p0.15
// head light intrpt0 p0.16
// left indicator 0.19
// right indicator p0.20
// head light p0.21

u32 ultrasonic_read(void) {
    u32 time;
    IOCLR0=(1<<17);
    delay_us(2);
    IOSET0=(1<<17);
    delay_us(10);
    IOCLR0=(1<<17);
    while(!((IOPIN0>>18)&1));
    T1PR=60-1;
    T1TCR=2;
    T1TCR=1;
    while(((IOPIN0>>18)&1));
    T1TCR=0;
    time=T1TC;
    return (u32)(time/58); // cm
}
CAN1 v1;

int main() {
	  float temp,speed,fuel;
    int left_indi,right_indi,head_light;
    IODIR0|=(1<<17)|(1<<19)|(1<<20)|(1<<21); // output pins
    IOSET0=(1<<19)|(1<<20)|(1<<21);
    can1_init();
    adc_init();
    uart0_init(9600);
    config_ext_intrpt0();
    config_vic_for_ext_intrpt0();
    config_ext_intrpt1();
    config_vic_for_ext_intrpt1();
    config_ext_intrpt2();
    config_vic_for_ext_intrpt2();
    v1.id=0x123;
    v1.rtr=0; // data-frame
    v1.dlc=8;
    while(1) {
        v1.byteA=0;
        v1.byteB=0;
        v1.byteA|=adc_read(0)&0x3FF; // temp sensor
        v1.byteA|=(adc_read(2)&0x3FF)<<10; // pot
        v1.byteA|=(ultrasonic_read()&0x3FF)<<20; // ultrasonic sensor
        v1.byteB|=head_flag<<21; // head light
        v1.byteB|=right_flag<<20; // right indi
        v1.byteB|=left_flag<<19; // left indi
        can1_tx(v1); // send data-frame

        // byteA
        temp=adc_to_temp(v1.byteA&0x3FF);
        speed=adc_to_speed((v1.byteA>>10)&0x3FF);
        fuel=dis_to_fuel((v1.byteA>>20)&0x3FF);

        // byteB
        left_indi=(v1.byteB>>19)&1;
        right_indi=(v1.byteB>>20)&1;
        head_light=(v1.byteB>>21)&1;

        // temp
        uart0_tx_string("TEMPERATURE: ");
        uart0_float(temp);
        uart0_tx_string("C\r\n");

        // speed
        uart0_tx_string("SPEED: ");
        uart0_float(speed);
        uart0_tx_string("km/h\r\n");

        // fuel
        uart0_tx_string("FUEL LEVEL: ");
        uart0_float(fuel);
        uart0_tx_string("L\r\n");

        // left indi
        if(left_indi)
            uart0_tx_string("<-");
        else
            uart0_tx_string("  ");
        delay_ms(20);

        // head light
        if(head_light)
            uart0_tx_string(" . ");
        else
            uart0_tx_string("   ");
        delay_ms(20);

        // right indi
        if(right_indi)
            uart0_tx_string("->");
        else
            uart0_tx_string("  ");

        uart0_tx_string("\r\n");
        delay_ms(3000);
    }
}
