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

