#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "74HCT595N.h"
#include "uart.h"
#include "timer4.h"

#define GEAR 15

// ISR per il Timer/Counter 4 in modalità Compare Match
unsigned int oldval=0;
volatile unsigned int rpm=0;
volatile unsigned int rpmgear=0;;

ISR(TIMER4_COMPA_vect) {
    char printbuffer [256];
    unsigned int conv=60/12;
    
    unsigned int delta=counter-oldval;
    rpm=delta*conv * 10;   //moltiplico per un fattore 10 in quanto la isr viene chiamate ogni 100 mS
    rpmgear=rpm/GEAR;
    //sprintf(printbuffer, "conv: %u cnt: %u rpm: %u rpm gear: %u\n", conv, delta, rpm, rpmgear);
    oldval=counter;
    //usart_TransmitString(printbuffer);
    
}

// Inizializza il Timer/Counter 5
void initTimer4(void) {
    // Imposta il Timer/Counter 4 in modalità CTC
    TCCR4A = 0; // Imposta TCCR4A a 0
    /*
    TCCR4B = (1 << WGM42) | (1 << CS42) | (1 << CS40); // Modalità CTC con prescaler di 1024
    // Calcola il valore di confronto per 1 secondo
    // (F_CPU / prescaler) = numero di tick al secondo
    // confronto = (F_CPU / prescaler) - 1
    // Per un prescaler di 1024 e F_CPU di 16 MHz, confronto = 15624
    OCR4A = (F_CPU / 1024) - 1;
    */

   TCCR4B = (1 << WGM42) | (1 << CS42);  // Modalità CTC con prescaler di 256
   // Calcola il valore di confronto per 100 mS
    // (F_CPU / prescaler) / 10 = numero di tick ogni 100 mS
    // confronto = ((F_CPU / prescaler) /10 )- 1
    // Per un prescaler di 256 e F_CPU di 16 MHz, confronto = 6249
   OCR4A = ((F_CPU / 256)/10) - 1;

    // Abilita l'interrupt per il Compare Match A del Timer/Counter 4
    TIMSK4 = (1 << OCIE4A);
}

