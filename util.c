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
 
 