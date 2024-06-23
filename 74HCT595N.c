#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "74HCT595N.h"
#include "uart.h"
#include "pid.h"

// definizione bit motore
#define FORWARD_M1		0x1 << 2
#define BACKWARD_M1  0x2 << 2
#define RELEASE_M1		0x3 << 2

// definizione dei pin dell'AVR MEGA2560 per pilotare lo shift register 74HCT595
#define Data_Pin		    (1<<5)
#define Enable_Pin		    (1<<4)
#define Latch_Clock_Pin     (1<<6)
#define Shift_Clock_Pin     (1<<5)

#define Pwm_pin				(1<<5)

#define Output_Enable_H  		PORTH |= Enable_Pin
#define Output_Enable_L	 		PORTH &= ~Enable_Pin
#define ShiftReg_Clk_H			   	PORTG |= Shift_Clock_Pin
#define ShiftReg_Clk_L			    PORTG &= ~Shift_Clock_Pin
#define LatchSR_Clk_H			    PORTB |= Latch_Clock_Pin
#define LatchSR_Clk_L				PORTB &= ~Latch_Clock_Pin

#define Output_Pwm_H				PORTB |= Pwm_pin
#define Output_Pwm_L				PORTB &= ~Pwm_pin

unsigned char bitpattern_Motore=0x0;

void init74HCT595N() {
		DDRH |= Enable_Pin | Data_Pin;  //definisce pin 7 e 8 OUTPUT (bit 4 e 5)
		Output_Enable_L;  //imposta Pin 7 LOW (OE) 
		PORTH |=  Data_Pin; //mette pin 8 HIGH (bit 5)
		
		DDRG |= Shift_Clock_Pin;  //definisce pin 4 OUTPUT (bit 5)
		ShiftReg_Clk_L;     //mette pin 4 LOW (bit 5)		
		
		DDRB |= Latch_Clock_Pin;  //definisce pin 12 OUTPUT (bit 6)		
		LatchSR_Clk_L;   //mette pin 12 LOW (bit 6)
		

		set74HCT595N(bitpattern_Motore);


}
// Muove il clock dei dati dello shift register
void Dati_Clk_transizione(void)
{
	ShiftReg_Clk_H;
	_delay_ms(3);
	ShiftReg_Clk_L;
	_delay_ms(3);   

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


	for (int i=7; i>=0; i--) {  // Invia il pattern di dati verso lo shift register
		unsigned char val = pattern & (1<<i);   
		//printf("bit: %d %d\n",i,val);
		if(val!=0){
			PORTH |= Data_Pin;  
		} else {
			PORTH &= ~Data_Pin;
		}
        
		Dati_Clk_transizione();  //La transizione ALTO-BASSO del clock memorizza i bit del pattern nello shift register

	}  // end for
	Latch_Clk_transizione();  // La transizione BASSO-ALTO del clock aggiorna il buffer di output dello shift register
	//printf("PORTB: %02X\n",PORTB);
	Output_Enable_L;   	  // abilita il buffer di output dello shift register
	//printf("PORTB: %02X\n",PORTB);

}

// prende in ingresso numero del motore e comando da inviare e setta il bitpattern
void attivaMotore74HCT595N(unsigned char id_motore, unsigned char comando){
	
	switch (id_motore) {
		case MOTORE_1:
			switch (comando) {
				case FORWARD:
					bitpattern_Motore&= ~RELEASE_M1;
					bitpattern_Motore|= FORWARD_M1;
					break;
				case BACKWARD:
					bitpattern_Motore&= ~RELEASE_M1;
					bitpattern_Motore|= BACKWARD_M1;
					break;
				case RELEASE:
					bitpattern_Motore&= ~RELEASE_M1;
					bitpattern_Motore|= RELEASE_M1;
					break;

			}
			break;

		case MOTORE_2:
			break;
		case MOTORE_3:
			break;
		case MOTORE_4:
			break;

	}
	
	set74HCT595N(bitpattern_Motore);
}





void setPwmDutyCycle(uint8_t duty) {
	//char printbuffer [256];
	
    //OCR1A = (duty / 100.0) * ICR1; // Usa OCR1A per PB5

	uint32_t val00=ICR1;	
	uint32_t val0=((uint32_t) duty) * val00;
	uint32_t val1=( val0 / 100);

	uint8_t low=((uint8_t)val1) & 0x00FF;
	uint8_t high=(uint8_t)((val1 & 0xFF00) >>8);
// in scrittura prima H e poi L	
	OCR1AH = high;
	OCR1AL = low;

/*
	sprintf(printbuffer, "\n %lu\n", val00);
    usart_TransmitString(printbuffer);

	sprintf(printbuffer, "\n %04X\n", (uint16_t)duty);
    usart_TransmitString(printbuffer);

	sprintf(printbuffer, "\n %04X\n", (uint16_t)val1);
    usart_TransmitString(printbuffer);

	sprintf(printbuffer, "\n %04X\n", (uint16_t)val0);
    usart_TransmitString(printbuffer);

	sprintf(printbuffer, "\n %04X\n", (uint8_t)low);
    usart_TransmitString(printbuffer);

	sprintf(printbuffer, "\n %04X\n", (uint8_t)high);
    usart_TransmitString(printbuffer);
*/
// in lettura prima L e poi H
	low=OCR1AL;
	high=OCR1AH;

/*

	sprintf(printbuffer, "\n %04X \n", (uint8_t)low);
    usart_TransmitString(printbuffer);

	sprintf(printbuffer, "\n %04X \n", (uint8_t)high);
    usart_TransmitString(printbuffer);
*/

}

void setPwm74HCT595N(unsigned char id_motore, unsigned int duty) {
	setPwmDutyCycle(duty);
}

void InitPWM(void)
{
 	DDRB |= Pwm_pin;   // PB5 Output
    TCCR1A = (1 << COM1A1) | (1 << COM1A0) | (1 << WGM11); // Abilita OC1B (compare match output B) e inverte il segnale generato in modo che il valore di duty
														   // cycle corrisponda alla parte alta
	
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler = 8 divide la frequenza dell'oscillatore da 16 Mhz per 8
    ICR1 = 19999; // Imposta il top per una frequenza di 50 Hz

	setPwmDutyCycle(0);
}

void EncoderInterruptInit(void){
	DDRD &=~(0x03);  //pin 0 e pin 1 PORTD input
	PORTD|=0x03;   //pin 0 e pin 1 PORTD  pullup	
	EICRA |=0x03;  // Configuro INT0 per attivare ISR sul fronte di salita (ISC00 e ISC01 messi a 1)
	EIMSK |= 0x01;  //  Abilito l'interrupt su INT0
}

unsigned int counter=0;
// Interrupt Service Routine per INT0
ISR(INT0_vect) {
    counter++;
}



void setRpm74HCT595N(unsigned char id_motore, unsigned int rpm_value, unsigned char comando) {
	switch (comando) {
				case FORWARD:
					rpm_attivo=1;
					num_giri=rpm_value;
					break;
				case BACKWARD:
					rpm_attivo=1;
					num_giri=rpm_value;
					break;
				case RELEASE:
					rpm_attivo=0;
					num_giri=0;
					break;

	}
	
}







