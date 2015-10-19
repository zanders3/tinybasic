#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tinybasic.h"

int main() {
    while (1) {
        char input[80];
        printf(": ");
        fgets(input, sizeof(input), stdin);
        char* p = input;
        int i = 0;
        int isInString = 0;
        while (*p != '\n' && i<79) {
            if (*p == '"') {
                isInString = !isInString;
            }
            if (!isInString && *p >= 'A' && *p <= 'Z') {
                *p -= 32;
            }
            p++;
            i++;
        }
        *p = '\0';
        
        runprogram(input);
    }
}