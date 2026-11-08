#include "header.h"

void can1_init(void){
	VPBDIV=1;//pclk=60MHz
	PINSEL1|=0x00040000;//P0.25--> RD1
	C1MOD=1;//select reset mode
	AFMR=2;//accept all receiving msgs
	C1BTR=0x001C001D;//baudrate=125Kbps
	C1MOD=0;//release reset mode
}

#define RBS (C1GSR&1)
void can1_rx(CAN1 *ptr){
	uart0_tx_string("rx fun start\r\n");
	while(RBS==0);
	ptr->id=C1RID;
	ptr->dlc=(C1RFS>>16)&0xF;
	ptr->rtr=(C1RFS>>30)&1;
	ptr->ff=(C1RFS>>31)&1;
	if(ptr->rtr==0){
		ptr->byteA=C1RDA;
		ptr->byteB=C1RDB;
	}
	C1CMR=0x4;
	uart0_tx_string("rx fun end\r\n");
}

#define TCS ((C1GSR>>3)&1)
void can1_tx(CAN1 v){
	uart0_tx_string("tx fun start\r\n");
	C1TID1=v.id;
	C1TFI1=(v.dlc<<16);
	if(v.rtr==0){
		C1TDA1=v.byteA;
		C1TDB1=v.byteB;
	}
	else 
		C1TFI1|=(1<<30);
	C1CMR=0x21;
	while(TCS==0);
	uart0_tx_string("tx fun end\r\n");
}
