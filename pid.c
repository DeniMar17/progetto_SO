#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "74HCT595N.h"
#include "uart.h"

unsigned int num_giri=0;
unsigned char rpm_attivo=0;

void PID(void){
    char printbuffer [256];


    


    if(rpm_attivo){
        // pid
        sprintf(printbuffer, "sto nel pid \n");
        usart_TransmitString(printbuffer);

       
        _delay_ms(1);

    }else{
        unsigned int count=counter;
        sprintf(printbuffer, "cnt: %u \n",count);
        usart_TransmitString(printbuffer); 
        //sprintf(printbuffer, "pid wait\n");
        //usart_TransmitString(printbuffer);
        _delay_ms(1000);
    }
}
 