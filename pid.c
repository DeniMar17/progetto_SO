#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "74HCT595N.h"
#include "uart.h"
#include "pid.h"
#include "timer4.h"
#include <float.h>
#include "util.h"

int num_giri_desiderati=0;
unsigned char rpm_attivo=0;
uint8_t  dutyCycle=0;

float Iout=0.0;
float Iout_1=0.0;
float Pout=0.0;
float Dout=0.0;
float e_1=0.0;

int start=0;
void PID(void){
    char printbuffer [256];
    char floatbuffer[20];


    //1 giro motore 12
    //1 giro ruota 170
    if(rpm_attivo){

        if(start){
            start=0;
            sprintf(printbuffer, "START\n");
            usart_TransmitString(printbuffer); 
        }

        //pid
        int u=num_giri_desiderati;  //ingresso (valore desiderato)
        float Kp=6.0; //proporzionale
        float T=0.2;
        float Ti=0.5; 
        float Td=0.3;
        //float Ki=Kp*(T/Ti); //integrale
        float Ki=7.0;
        //float Kd=Kp*(Td/T); //derivativo
        float Kd=1.0; //derivativo
        float y=rpmgear; //uscita


        float e=u-y; //calcolo dell'errore
        Iout=Ki*e+Iout_1;
        Pout=Kp*e;
        Dout=Kd*(e-e_1);
        unsigned int uk=Pout+Iout+Dout;


        FloatToStringNew(&floatbuffer, e, 2);
	    sprintf(printbuffer, "#%u,%u,%u,%s\n", uk ,num_giri_desiderati,rpmgear,floatbuffer);
        usart_TransmitString(printbuffer); 

        setPwmDutyCycle(uk);  

        Iout_1=Iout;
        e_1=e;

        

    }else{
        start=0;
        unsigned int count=counter;
        sprintf(printbuffer, "STOP\n");
        usart_TransmitString(printbuffer); 
        _delay_ms(1000);
    }
}
 