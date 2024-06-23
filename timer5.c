#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "timer5.h"

volatile uint32_t countOverflow = 0;

// ISR per il timer overflow
ISR(TIMER5_OVF_vect) {
    countOverflow++;
    PORTD^=0x04;   // (debug) pin 2 output usato per verificare che l'overflow del timer avviene circa ogni 33 millisecondi
}

// Inizializza il Timer/Counter 5
void initTimer5(void) {
    TCCR5A = 0; // Imposta TCCR5A a 0
    TCCR5B = (1 << CS51); // Imposta il prescaler a 8 e si ottiene una frequenza di 20MHz
    TIMSK5 = (1 << TOIE5); // Abilita l'interrupt per gli overflow
    TCNT5 = 0; // Imposta il contatore a zero

    sei(); 


    /* imposto il timer a 16 bit in modo tale da generare un interupt di overflow.
       Tenendo conto che la frequenza della CPU è 16 MHz il numero di cicli
       per microsecondo si ottiene dividendo 16MHz per 10^6 (numero di microsecondi in un secondo)
       e questo valore è pari a 16 cicli per secondo. Impostando il prescaler a 8
       per ottenere ogni quanti microsecondi viene generato l' overflow
       si esegue il seguente calcolo 65536 (contatore a 16 bit, 2^16) * (8/16) (prescaler/cicli per secondo).
       Il risultato ottenuto è di 32768 microsecondi cioè circa 33 millisecondi */
}

// Restituisce il numero di microsecondi trascorsi
uint32_t restituisciMicrosecondi(void)  {
    uint32_t num_overflows;
    uint16_t timer_count;

    cli(); // disabilita gli interrupt per evitare che cambi la variabile countOverflow
    num_overflows = countOverflow;
    timer_count = TCNT5;
    if ((TIFR5 & (1 << TOV5)) && (timer_count < 0xFFFF)) {
        num_overflows++; // corregge il numero di overflow nel caso in cui l'overflow si è verificato dopo aver letto countOverflow ma prima di leggere TCNT5
    }
    sei();

    // Calcola i microsecondi
    uint32_t total_ticks = ((uint32_t)num_overflows << 16) + timer_count;
    uint32_t microsecondi = (total_ticks * 8) / (16000000 / 1000000UL);

    return microsecondi;
}

