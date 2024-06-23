#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "uart.h"
#include "util.h"
#include "74HCT595N.h"


#define MAX_NUM_CMD_MIO 	10
char *helpStr[MAX_NUM_CMD_MIO] =
	{
		"'help'                 			(mostra la lista dei comandi possibili)",
		"'motor num_motore forward'                     			(attiva il motore num_motore muovendolo avanti)",
		"'motor num_motore backward'                   			(attiva il motore num_motore muovendolo indietro)",
		"'motor num_motore release'                      			(spegne il motore num_motore)",
		"'motorpwm num_motore dutycycle forward'                      			(imposta il pwm del motore a dutycycle muovendolo avanti)",
    "'motorpwm num_motore dutycycle backward'                      			(imposta il pwm del motore a dutycycle muovendolo indietro)",
    "'motorpwm num_motore dutycycle release'                      			(imposta il pwm del motore a dutycycle e lo spegne)",
		"'motorrpm num_motore rpmvalue forward'                      			(imposta il rpm del motore a rpmvalue muovendolo avanti)",
    "'motorrpm num_motore rpmvalue backward'                      			(imposta il rpm del motore a rpmvalue muovendolo indietro)",
    "'motorrpm num_motore rpmvalue release'                      			(imposta a zero i giri del motore ew disabilita il pid)",
		
	};



// prende in ingresso un buffer da analizzare attraverso token e in base al tipo di comando apre la lista di comandi possibili 
// o passa i parametri alla funzione che attiva il motore

void GestioneComandi(char *buffer){
  char printbuffer [256];
  char* rest;
  char* token=strtok_r(buffer, " ", &rest);
  toLower(token);
  usart_TransmitString(token);
  if(strcmp(token,"help")==0){
      //stampa su terminale i comandi possibili
        sprintf(printbuffer, "Lista comandi:\n");
	    usart_TransmitString(printbuffer);
		for(int i=0;i<MAX_NUM_CMD_MIO;i++)
		{			
            sprintf(printbuffer, "%s\n",helpStr[i]);
	        usart_TransmitString(printbuffer);

		}
		sprintf(printbuffer, "Fine Lista\n");
	    usart_TransmitString(printbuffer);

  } else if(strcmp(token,"motor")==0) {      
      token = strtok_r(NULL, " ", &rest);
      usart_TransmitString(token);

      if(isStringNumeric(token)){
        int num_motore=atoi(token);

        token = strtok_r(NULL, " ", &rest);
        toLower(token);
        if(strcmp(token,"forward")==0){
          usart_TransmitString(token);
          attivaMotore74HCT595N(num_motore, FORWARD);
        }
        else if(strcmp(token,"backward")==0){
          attivaMotore74HCT595N(num_motore, BACKWARD);
        }
        else if(strcmp(token,"release")==0){
          attivaMotore74HCT595N(num_motore, RELEASE);
        } else {
          sprintf(printbuffer, "Comando non valido, se serve aiuto scrivere 'help' \n");
          usart_TransmitString(printbuffer);
        }
      } else { 
        sprintf(printbuffer, "Numero motore non valido, se serve aiuto scrivere 'help' \n");
        usart_TransmitString(printbuffer);
      }    

  } else if(strcmp(token,"motorpwm")==0) {      
      token = strtok_r(NULL, " ", &rest);
      usart_TransmitString(token);

      if(isStringNumeric(token)){
        int num_motore=atoi(token);

        token = strtok_r(NULL, " ", &rest);
        if(isStringNumeric(token)){
          usart_TransmitString(token);
          unsigned short pwm_duty=atoi(token);

          // imposta il pwm value
          setPwm74HCT595N(num_motore, pwm_duty);

          token = strtok_r(NULL, " ", &rest);
          toLower(token);

          if(strcmp(token,"forward")==0){
            usart_TransmitString(token);
            attivaMotore74HCT595N(num_motore, FORWARD);
          }
          else if(strcmp(token,"backward")==0){
            attivaMotore74HCT595N(num_motore, BACKWARD);
          }
          else if(strcmp(token,"release")==0){
            attivaMotore74HCT595N(num_motore, RELEASE);
          } else {
            sprintf(printbuffer, "Comando non valido, se serve aiuto scrivere 'help' \n");
            usart_TransmitString(printbuffer);
          }


        } else { 
        sprintf(printbuffer, "duty cycle non valido, se serve aiuto scrivere 'help' \n");
        usart_TransmitString(printbuffer);
      }    
        
      } else { 
        sprintf(printbuffer, "Numero motore non valido, se serve aiuto scrivere 'help' \n");
        usart_TransmitString(printbuffer);
      }   

  } else if(strcmp(token,"motorrpm")==0) {      
      token = strtok_r(NULL, " ", &rest);
      usart_TransmitString(token);

      if(isStringNumeric(token)){
        int num_motore=atoi(token);

        token = strtok_r(NULL, " ", &rest);
        if(isStringNumeric(token)){
          usart_TransmitString(token);
          int rpm_value=atoi(token);

          token = strtok_r(NULL, " ", &rest);
          if(strcmp(token,"forward")==0){
            // imposta rpm value
            usart_TransmitString(token);
            setRpm74HCT595N(num_motore, rpm_value, FORWARD);
          }
          else if(strcmp(token,"backward")==0){
             // imposta rpm value
            setRpm74HCT595N(num_motore, rpm_value, BACKWARD);
          }
          else if(strcmp(token,"release")==0){
            // imposta rpm value
            setRpm74HCT595N(num_motore, rpm_value, RELEASE);
          } else {
            sprintf(printbuffer, "Comando non valido, se serve aiuto scrivere 'help' \n");
            usart_TransmitString(printbuffer);
          }
          
        } else { 
          sprintf(printbuffer, "rpmvalue non valido, se serve aiuto scrivere 'help' \n");
          usart_TransmitString(printbuffer);
        }    
        
      } else { 
        sprintf(printbuffer, "Numero motore non valido, se serve aiuto scrivere 'help' \n");
        usart_TransmitString(printbuffer);
      }    
  } else {
    sprintf(printbuffer, "Comando non valido, se serve aiuto scrivere 'help' \n");
	    usart_TransmitString(printbuffer);
  }
}
