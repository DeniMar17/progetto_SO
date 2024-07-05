#pragma once

#define FORWARD 1
#define BACKWARD 2
#define RELEASE 0

#define MOTORE_1 1
#define MOTORE_2 2
#define MOTORE_3 3
#define MOTORE_4 4

// inizializza i pin della MEGA2560 per pilotare lo shift register
void init74HCT595N(void);

// invia i dati allo shift register
void set74HCT595N(unsigned char pattern);

// funzione per accendere/spegnere i motori 1 2 3 4 
void attivaMotore74HCT595N(unsigned char id_motore, unsigned char comando);

// inizializza il pwm
void InitPWM(void);

//imposta il valore del pwm  
void setPwmDutyCycle(uint16_t duty);

//imposta il valore del pwm in percentuale 
void setPwmDutyCyclePercentuale(uint8_t duty);

// imposta il duty cicle del pwm per il motore specificato
void setPwm74HCT595N(unsigned char id_motore, unsigned int duty);

// imposta il numero di giri da raggiungere per il motore specificato utilizzando un controllore PID
void setRpm74HCT595N(unsigned char id_motore,  int rpm_value, unsigned char comando);

// inizializza l'interrupt relativo all'encoder
void EncoderInterruptInit(void);



// variabili globali
extern volatile unsigned int counter;



