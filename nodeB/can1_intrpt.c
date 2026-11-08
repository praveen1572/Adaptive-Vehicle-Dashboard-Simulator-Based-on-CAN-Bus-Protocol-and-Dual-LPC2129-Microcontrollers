#include "header.h"
volatile u32 flag=0;
volatile u32 left_indi=0,right_indi=0,head_light=0;
void CAN1_Rx_Handler(void) __irq{
	flag=1;
	r1.id=C1RID;
	r1.dlc=(C1RFS>>16)&0xF;
	r1.rtr=(C1RFS>>30)&1;
	if(r1.rtr==0){
		r1.byteA=C1RDA;
		r1.byteB=C1RDB;
		left_indi=(r1.byteB>>19)&1;
    	right_indi=(r1.byteB>>20)&1;
   		head_light=(r1.byteB>>21)&1;
		if(left_indi)
            uart0_tx_string("left indic ON\r\n");
		if(right_indi)
            uart0_tx_string("right indic ON\r\n");	  
		if(head_light)
            uart0_tx_string("head light ON\r\n");	}
	C1CMR=0x4;
	VICVectAddr=0;
}

void config_vic_for_can1(void){
	VICIntSelect=0;
	VICVectAddr5=(u32)CAN1_Rx_Handler;
	VICVectCntl5=26|(1<<5);
	VICIntEnable=(1<<26);
	C1IER=1;
}
