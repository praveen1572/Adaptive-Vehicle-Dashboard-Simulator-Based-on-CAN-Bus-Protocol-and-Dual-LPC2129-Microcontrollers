#include "header.h"

CAN1 r1;

int main() {
    float temp,speed,fuel;
    int left_indi,right_indi,head_light;
    can1_init();
    config_vic_for_can1();
    uart0_init(9600);
    IODIR0|=(1<<19)|(1<<20)|(1<<21);
    IOSET0=(1<<19)|(1<<20)|(1<<21);
    while(1) {
        //can1_rx(&r1);
        if(flag) {
            flag=0;
            // byteA
            temp=adc_to_temp(r1.byteA&0x3FF);
            speed=adc_to_speed((r1.byteA>>10)&0x3FF);
            fuel=dis_to_fuel((r1.byteA>>20)&0x3FF);
            if(fuel<0)
                fuel=0;

            // byteB
            left_indi=(r1.byteB>>19)&1;
            right_indi=(r1.byteB>>20)&1;
            head_light=(r1.byteB>>21)&1;

            // temp
            uart0_tx_string("TEMPERATURE: ");
            uart0_float(temp);
            uart0_tx_string("C\r\n");
            delay_ms(20);

            // speed
            uart0_tx_string("SPEED: ");
            uart0_float(speed);
            uart0_tx_string("km/h\r\n");
            delay_ms(20);

            // fuel
            uart0_tx_string("FUEL LEVEL: ");
            uart0_float(fuel);
            uart0_tx_string("L\r\n");
        
            // left indi
            if(left_indi)
                uart0_tx_string("<-");
            else
                uart0_tx_string("  ");
        
            // head light
            if(head_light)
                uart0_tx_string(" . ");
            else
                uart0_tx_string("   ");
        
            // right indi
            if(right_indi)
                uart0_tx_string("->");
            else
                uart0_tx_string("  ");
            uart0_tx_string("\r\n");
        }

        // headlight control
        if(head_light)
            IOCLR0=1<<21;
        else
            IOSET0=1<<21;

        // indi control
        if(left_indi) {
            IOSET0=1<<20;
            IOCLR0=1<<19;
            delay_ms(100);
            IOSET0=1<<19;
            delay_ms(100);
        } 
        else if(right_indi) {
            IOSET0=1<<19;
            IOCLR0=1<<20;
            delay_ms(100);
            IOSET0=1<<20;
            delay_ms(100);
        }
        else {
            IOSET0=1<<19;
            IOSET0=1<<20;
        }

        delay_ms(3000);
    }
}
