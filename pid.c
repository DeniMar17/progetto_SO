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

    long t_precedente = 0;
    int pos_precedente = 0;

    long t_precedente_i = 0;
    int pos_i = 0;
    float velocita_i = 0;
    

    //1 giro motore 12
    //1 giro ruota 170
    if(rpm_attivo){
        // pid
        /*
        sprintf(printbuffer, "sto nel pid \n");
        usart_TransmitString(printbuffer);

        int pos = 0;
        float velocita = 0;
        pos = pos_i; // pos_i è il conteggio dell'interrupt
        float eintegral = 0;

        uint32_t t = restituisciMicrosecondi();
        float deltaT = ((float) (t-t_precedente))/1.0e6;
        float velocita = (pos - pos_precedente)/deltaT;
        pos_precedente = pos;
        t_precedente = t;

        // Convert count/s to RPM
        float v = velocita/170.0*60.0;

        float v_des = num_giri*100; //target

        float kp = 5;
        float ki = 10;
        float e = v_des-v;
        eintegral = eintegral + e*deltaT;
  
        float u = kp*e + ki*eintegral;
*/
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
 