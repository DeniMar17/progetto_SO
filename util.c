#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "util.h"


// serve a rendere tutta minuscola una stringa
void toLower(char* s) {
    if(s!=NULL) {
        for (int i = 0; i < strlen(s); i++) {
            s[i] = tolower(s[i]);
        }
    }
    
}
 // riporta 1 se la stringa è un numero 0 se contiene caratteri diversi da 0..9
 int isStringNumeric(char* s) {
    int result=1;
    if(s!=NULL) {
        for (int i=0;i<strlen(s); i++){
            if (!isdigit(s[i]))
            {
                result=0;
                break;
            }
        }
    }else{
        result=0;
    }
    return result;
 }

// converte float in stringa
 void FloatToStringNew(char *str, float f, char size) {

    char pos;  // posizione nella stringa
    char len;  // lunghezza della parte decimale
    char* curr;  // puntatore al prossimo
    int value;  

    pos = 0;  

    value = (int)f;  // si tronca il floating point 
    itoa(value,str,10);  // converte l'intero in stringa

    if (f < 0 ) 
    {
        f *= -1;
        value *= -1;
    }

    len = strlen(str);  // lumnghezza della parte intera
    pos = len;  // posiziona il puntatore alla fine della parte intera
    str[pos++] = '.';  // aggiunge il punto dei decimali

    while(pos < (size + len + 1) )  // si occupa delle altre cifre
    {
        f = f - (float)value;  
        f *= 10;  
        value = (int)f;  // prende la prossima cifra
        itoa(value, curr,10); // la converte in stringa
        str[pos++] = *curr; // la aggiunge alla stringa risultante incrementando il puntatore
    }
 }