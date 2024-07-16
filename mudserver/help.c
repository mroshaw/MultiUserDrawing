/* help.c	                                */
/* Routines for debug help on server	    */
/*	                                        */
/* Iain Ollerenshaw 13/10/97            	*/
/*	                                        */
/* Version History:                     	*/
/* Date     Who     Comments            	*/
/* 13/10/97	IO  	Created	                */
/*	                                        */

#include "help.h"
#include <stdio.h>

void show_help() {
    int c;
    FILE *help_file;
    help_file = fopen("helpFile.txt", "r");
    if (!help_file) {
        fprintf(stderr, "Cannot open help file!");
    } else {
        while ((c = getc(help_file)) != EOF) {
            fprintf(stdout, "%c", c);
        }
        fclose(help_file);
    }
}
