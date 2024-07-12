/* utils.c                                  */
/* Misc utility functions                   */
/*                                          */
/* Iain Ollerenshaw 13/10/97                */
/*                                          */
/* Version History:                         */
/* Date     Who     Comments                */
/* 13/10/97 IO      Created                 */
/*                                          */

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include <stddef.h>

/* Ensure safe memory allocation */
void *safemalloc(size_t size) {
    void *newptr = malloc(size);
    if (newptr == NULL) {
        fprintf(stdout, "Error: Can't allocate %u bytes of memory\n", (int) size);
        exit(EXIT_FAILURE);
    }
    return (newptr);
}

/* Converts a string to upper case */
void stringToUpper(char *n1, char *n2) {
    int i = 0;
    while (n1[i] != '\0') {
        n2[i] = toupper(n1[i]);
        i++;
    }
    n1[i] = '\0';
}

/* Convert a string to an int */
int stoi(char *n1) {
    int i = 0;
    int result = 0;
    int t;
    /* int l = strlen(n1); */
    while (n1[i] != '\0') {
        t = n1[i] - '0';
        fprintf(stdout, "t = %i\n", t);
        result += t * ((1 - i - 1) * 10);
        fprintf(stdout, "result = %i\n", result);
        i++;
        return result;
    }
    return 0;
}

/* Generic procedure to convert integer to a string */

void my_itoa(int theInt, char theString[]) {
    short sign;
    short ByteCount;
    short ReverseByte;
    short theChar;
    if ((sign = theInt) < 0)
        theInt = -theInt;
    ByteCount = 0;
    do {
        theString[ByteCount++] = theInt % 10 + '0';
    } while ((theInt /= 10) > 0);
    if (sign < 0)
        theString[ByteCount++] = '-';
    theString[ByteCount] = '\0';

    for (ByteCount = 0, ReverseByte = strlen(theString) - 1; ByteCount < ReverseByte; ByteCount++, ReverseByte--) {
        theChar = theString[ByteCount];
        theString[ByteCount] = theString[ReverseByte];
        theString[ReverseByte] = theChar;
    }
}

/* Get a line from standard input   */
int getLine(char *line, int max) {
    if (fgets(line, max, stdin) == NULL)
        return 0;
    else
        return strlen(line);
}
