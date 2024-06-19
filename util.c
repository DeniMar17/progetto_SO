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