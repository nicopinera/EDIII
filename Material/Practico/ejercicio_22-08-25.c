/*
===============================================================================
 Name        : Ejercicio 1
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <cr_section_macros.h>

#define FALSE 0
#define MAX_VALUE 10000000




// TODO: insert other include files here

// TODO: insert other definitions and
void retardo(int tiempo);
void retardo_secuencial(void);

void check_pin( uint8_t *pin);

int main(void) {
	SystemInit();
	uint32_t verClk;
	verClk = SystemCoreClock;
	LPC_GPIO0->FIODIR |= ((1<<22)&(~(1<<19)));//output port0 pin 22
	LPC_GPIO0->FIOMASK = ~(1<<22);//
	LPC_GPIO0->FIOMASK &= ~(1<<19);//


	while(1){

//		LPC_GPIO0->FIOSET |= (1<<22);
		LPC_GPIO0->FIOPIN = (1<<22)& ( ~3 << 19 );//
		retardo_secuencial();
		LPC_GPIO0->FIOCLR |= (1<<22);
		retardo_secuencial();



	}
	 return 0 ;
}

void check_pin(uint8_t* pin){
	*pin =(LPC_GPIO0->FIOPIN2 >>3)&1;
	if(pin){
			signal_o =signal_1;

		}
	else{
			signal_o =signal_2;

		}
	if (status!=pin){
		rotate=0;
	}
}
static uint8_t status =0; //status 0 status 1;

void output_data(){//main()
	uint8_t array_bytes={0xff,0x0F,0xA0,0xAA};
	static rotate=0;
	static uint16_t signal_1 = 0x256;
	static uint16_t signal_2;
	static uint16_t signal_o;
	static uint8_t pin;
	static uint8_t status =0;
	check_pin(&pin);
	status=pin;
	while(1){
	LPC_GPIO0->FIOPIN = ((signal_o>>rotate) & 1 )<< 22;//
	//DELAY
	rotate++;
	check_pin(&pin);
	if(rotate ==11)
			rotate=0;
	}

}









}


void retardo (int tiempo){
	uint32_t counter;
	uint32_t *counter_address = &counter;

	printf("La direccion es: %x",counter_address);
	for(counter = 0; counter < tiempo; counter ++ ){}
	return;
}


void retardo_secuencial (void){

	static uint32_t UP_FADE = FALSE;
	static uint32_t counter=100000;

	if(!UP_FADE){
		counter+= 5000;
		if (counter== MAX_VALUE){
			UP_FADE = ~UP_FADE;
		}
	}
	else {
		counter -=5000;
		if (counter == 0){
			UP_FADE = ~UP_FADE;
		}
	}

	for (int i =0; i<counter;i++){}

	return;
}
