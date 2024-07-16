/*	utils.c	                                                                                */
/*  This file contains misc functions to perform various functions                          */
/*	requests		                                                                        */
/*                                                                                          */
/*	Iain Ollerenshaw 16/2/98		                                                        */
/*	Version History:		                                                                */
/*	Date	    Who	    Comments	                                                        */
/*	13/10/97    IO      Created 		                                                    */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include <stdlib.h>
#include <stddef.h>

/* Ensure safe memory allocation */
void *safe_malloc(size_t size) {
    void *newptr = malloc(size);
    if (newptr == NULL) {
        fprintf(stdout, "Error: Can't allocate %u bytes of memory\n", (int) size);
        exit(EXIT_FAILURE);
    }
    return (newptr);
}

/* Converts a string to upper case */
void string_to_upper(char *nl, char *n2) {
    int i = 0;
    while (nl[i] != '\0') {
        n2[i] = toupper(nl[i]);
        i++;
    }
    n2[i] = '\0';
}

/* Convert a string to an int */
int string_to_int(char *nl) {
    int i = 0;
    int result = 0;
    int t;
    /* int l = strlen(nl); */
    while (nl[i] != '\0')
        t = nl[i] - '0';
    fprintf(stdout, "t = %i\n", t);
    result += t * ((1 - i - 1) * 10);

    fprintf(stdout, "result = %i\n", result);
    i++;
    return result;
}

/* Generic procedure to convert integer to a string */
void int_to_string(int the_int, char the_string[]) {
    short sign;
    short byte_count;
    short reverse_byte;
    short cur_char;
    if ((sign = the_int) < 0)
        the_int = -the_int;
    byte_count = 0;
    do {
        the_string[byte_count++] = the_int % 10 + '0';
    } while ((the_int /= 10) > 0);
    if (sign < 0)
        the_string[byte_count++] = '-';
    the_string[byte_count] = '\0';

    for (byte_count = 0, reverse_byte = strlen(the_string) - 1; byte_count < reverse_byte; byte_count++, reverse_byte--) {
        cur_char = the_string[byte_count];
        the_string[byte_count] = the_string[reverse_byte];
        the_string[reverse_byte] = cur_char;
    }
}

int get_line(char *line, int max) {
    if (fgets(line, max, stdin) == NULL)
        return 0;
    else
        return strlen(line);
}


// You must free the result if result is non-NULL.
char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins; // the next insert point
    char *tmp; // varies
    int len_rep; // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count; // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}
