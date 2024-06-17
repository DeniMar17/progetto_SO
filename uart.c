#include "uart.h"
// ********************************************************************************
// Includes
// ********************************************************************************
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>

// ********************************************************************************
// Macros and Defines
// ********************************************************************************
#define BAUD 19600
#define MYUBRR F_CPU/16/BAUD-1

//********************************************************************************
// Definisco i buffer di trasmissione e ricezione
//********************************************************************************
#define RX_BUFFER_SIZE 256     /* deve essere una potenza di 2 */
#define TX_BUFFER_SIZE 256     /* deve essere una pèotenza di 2 */
#define RX_BUFFER_MASK (RX_BUFFER_SIZE - 1)
#define TX_BUFFER_MASK (TX_BUFFER_SIZE - 1)

 static unsigned char RxBuf[RX_BUFFER_SIZE];
 static volatile unsigned char RxPtrTesta;
 static volatile unsigned char RxPtrCoda;
 static unsigned char TxBuf[TX_BUFFER_SIZE];
 static volatile unsigned char TxPtrTesta;
 static volatile unsigned char TxPtrCoda;

// ********************************************************************************
// Function Prototypes
// ********************************************************************************
void usart_init(uint16_t ubrr);
char usart_getchar( void );
void usart_putchar( char data );
void usart_pstr (char *s);
unsigned char usart_kbhit(void);
int usart_putchar_printf(char var, FILE *stream);


//static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);

// **********************************************************
// Interrupt Service Routine
// **********************************************************

/* isr di ricezione */
ISR(USART0_RX_vect) 
{
        unsigned char dato;
        unsigned char tmpPtrTesta;

        /* Legge i dati ricevuti */
        dato = UDR0;                 
        /* Calcola l'indice del buffer */
        tmpPtrTesta = (RxPtrTesta + 1) & RX_BUFFER_MASK;
        /* Salva il nuovo indice */
        RxPtrTesta = tmpPtrTesta;

        if (tmpPtrTesta == RxPtrCoda) {
        /* Il buffer è in overflow */
        }
        /* Mette i dati ricevuti nel buffer */
        RxBuf[tmpPtrTesta] = dato; 
}

/* isr di trasmissione */
ISR(USART0_UDRE_vect)
{
 unsigned char tmpPtrCoda;

 /* Controlla se tutti i dati sono stati trasmessi */
 if (TxPtrTesta != TxPtrCoda) {
  /* Calcola l'indice del buffer */
  tmpPtrCoda = (TxPtrCoda + 1) & TX_BUFFER_MASK;
  /* Salva il nuovo indice */
  TxPtrCoda = tmpPtrCoda;      
  /* Fa iniziare la trasmissione */
  UDR0 = TxBuf[tmpPtrCoda];  
 } else {       
  /* Disabilita l'interrupt UDRE */
  UCSR0B &= ~_BV(UDRIE0);         
 }
}

// ********************************************************************************
// Routine di gestione buffer circolare
// ********************************************************************************

unsigned char usart_ReceiveByte(void)
{
 unsigned char tmpPtrCoda;
    cli();
        printf("H: %d T: %d \n",RxPtrTesta , RxPtrCoda);
    sei();
    /* Aspetta i dati in arrivo */
    while (RxPtrTesta == RxPtrCoda);  // da rivedere non bloccante
    /* Calcola l'indice del buffer */
    tmpPtrCoda = (RxPtrCoda + 1) & RX_BUFFER_MASK;
    /* Salva il nuovo indice */
    RxPtrCoda = tmpPtrCoda;                
    /* Restituisce i dati */
    return RxBuf[tmpPtrCoda];          
}

void usart_TransmitByte(unsigned char data)
{
    unsigned char tmpPtrTesta;
    /* Calcola l'indice del buffer */
    tmpPtrTesta = (TxPtrTesta + 1) & TX_BUFFER_MASK;   //cambiare per gestire buffer lunghi
    /* Aspetta per dello spazio libero nel buffer */
    while (tmpPtrTesta == TxPtrCoda);
    /* Salva i dati nel buffer */
    TxBuf[tmpPtrTesta] = data;
    /* Salva il nuovo indice */
    TxPtrTesta = tmpPtrTesta;               
    /* Abilita l'interrupt UDRE */
    UCSR0B |= (_BV(UDRIE0));                    
}

void usart_TransmitString(char *ptrStr)
{
    while( *ptrStr )
    {
        /*--- Manda un carattere ---*/
        usart_TransmitByte( *ptrStr++ );
    }
}

// ********************************************************************************
// usart Related
// ********************************************************************************
void usart_init( uint16_t ubrr) {
    // Set baud rate
    UBRR0H = (uint8_t)(ubrr>>8);
    UBRR0L = (uint8_t)ubrr;

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 
    UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);   /* Enable RX and TX  e la generazione dell'interrupt sulla ricezione*/  
    RxPtrCoda = 0;
    RxPtrTesta = 0;
    TxPtrCoda = 0;
    TxPtrTesta = 0;
}
void usart_putchar(char data) {
    // Wait for empty transmit buffer
    while ( !(UCSR0A & (_BV(UDRE0))) );
    // Start transmission
    UDR0 = data; 
}
char usart_getchar(void) {
    // Wait for incoming data
    while ( !(UCSR0A & (_BV(RXC0))) );
    // Return the data
    return UDR0;
}
unsigned char usart_kbhit(void) {
    //return nonzero if char waiting polled version
    unsigned char b;
    b=0;
    if(UCSR0A & (1<<RXC0)) b=1;
    return b;
}
void usart_pstr(char *s) {
    // loop through entire string
    while (*s) { 
        usart_putchar(*s);
        s++;
    }
}
 
// this function is called by printf as a stream handler
int usart_putchar_printf(char var, FILE *stream) {
    // translate \n to \r for br@y++ terminal
    if (var == '\n') usart_putchar('\r');
    usart_putchar(var);
    return 0;
}

void printf_init(void){
  //stdout = &mystdout;
  
  // fire up the usart
  usart_init ( MYUBRR );
}

void init_USART(void){

  usart_init ( MYUBRR );
}
