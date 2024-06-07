#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include "74HCT595N.h"

// definizione dei pin dell'AVR MEGA2560 per pilotare lo shift register 74HCT595
#define Data_Pin		    1<<4
#define Enable_Pin		    1<<3
#define Latch_Clock_Pin     1<<6     
#define Shift_Clock_Pin     1<<5


#define Output_Enable_clear  				PORTH|=Enable_Pin
#define Output_Enable_set	 				PORTH&=~Enable_Pin
#define ShiftReg_Clk_H			   	PORTG|=Shift_Clock_Pin
#define ShiftReg_Clk_L			    PORTG&=~Shift_Clock_Pin
#define LatchSR_Clk_H			    PORTB|=Latch_Clock_Pin
#define LatchSR_Clk_L				PORTB&=~Latch_Clock_Pin


void init74HCT595N(){
		DDRH = Enable_Pin | Data_Pin;  //definisce pin 7 e 8 OUTPUT (bit 3 e 4)
		PORTH = Enable_Pin | Data_Pin; //mette pin 7 e 8 HIGH (bit 3 e 4)
		
		DDRG = Shift_Clock_Pin;  //definisce pin 4 OUTPUT (bit 4)
		PORTG = Shift_Clock_Pin; //mette pin 4 HIGH (bit 4)
		
		DDRB = Latch_Clock_Pin;  //definisce pin  OUTPUT (bit 6)
		PORTB = Latch_Clock_Pin; //mette pin 6 HIGH (bit 6)
		
		set74HCT595N(0x0);
}
// Muove il clock dei dati dello shift register
void Dati_Clk_transizione(void)
{
	ShiftReg_Clk_H;
	_delay_ms(20);   
	ShiftReg_Clk_L;
	_delay_ms(20);
}
// Muove il clock per memorizzare i dati sul buffer di output dello shift register
void Latch_Clk_transizione(void)
{
	LatchSR_Clk_H;
	_delay_ms(3); 
	LatchSR_Clk_L;
	_delay_ms(3);
}

// invia i dati allo shift register e ne abilita l'output
void set74HCT595N(unsigned char pattern){
	for (int i=0; i<8; i++) {  // Invia il pattern di dati verso lo shift register
		unsigned char val = pattern & 1<<i;   
		if(val!=0){
			PORTH |= Data_Pin;  
		} else {
			PORTH &= ~Data_Pin;
		}
        
		Dati_Clk_transizione();  //La transizione ALTO-BASSO del clock memorizza i bit del pattern nello shift register
	}  // end for
	Latch_Clk_transizione();  // La transizione BASSO-ALTO del clock aggiorna il buffer di output dello shift register
	Output_Enable_set;   	  // abilita il buffer di output dello shift register
}




