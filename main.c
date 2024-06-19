#include <avr/interrupt.h>
#include <avr/io.h>
#include <assert.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "tcb.h"
#include "tcb_list.h"
#include "uart.h"
#include "util.h"
#include "atomport_asm.h"
#include "scheduler.h"
#include "gestionecomandi.h"
#include "74HCT595N.h"
#define THREAD_STACK_SIZE 1024
#define IDLE_STACK_SIZE 128



//statically allocated variables where we put our stuff

TCB idle_tcb;
uint8_t idle_stack[IDLE_STACK_SIZE];
void idle_fn(uint32_t thread_arg __attribute__((unused))){
  while(1) {

    _delay_ms(5000);
  }
}

TCB p1_tcb;
uint8_t p1_stack[THREAD_STACK_SIZE];
void p1_fn(uint32_t arg __attribute__((unused))){
  while(1){

    _delay_ms(1000);
  }
}

TCB p2_tcb;
uint8_t p2_stack[THREAD_STACK_SIZE];
void p2_fn(uint32_t arg __attribute__((unused))){
  char buffer[256];
  unsigned char buffptr=0;

  memset(buffer,0x0,256);
  while(1){
	// leggere un carattere dalla porta seriale 

	  char ch=usart_ReceiveByte();

    if( (ch!='\n') && (ch!='\r') ){
      buffer[buffptr]=ch;
      buffer[++buffptr]=0x0;
    }
    else {
      //usart_TransmitString(&buffer);
      GestioneComandi(&buffer);
      
      buffptr=0;
    }
    
  }
}







int main(void){
  // inizializzazione della usart 0
  init_USART();

  init74HCT595N();

  InitPWM();




//while(1){
//  set74HCT595N(0xff);
//}


  TCB_create(&idle_tcb,
             idle_stack+IDLE_STACK_SIZE-1,
             idle_fn,
             0);

  TCB_create(&p1_tcb,
             p1_stack+THREAD_STACK_SIZE-1,
             p1_fn,
             0);

  TCB_create(&p2_tcb,
             p2_stack+THREAD_STACK_SIZE-1,
             p2_fn,
             0);

  
  TCBList_enqueue(&running_queue, &p1_tcb);
  TCBList_enqueue(&running_queue, &p2_tcb);
  TCBList_enqueue(&running_queue, &idle_tcb);

  usart_pstr("starting\n");
  startSchedule();
}
