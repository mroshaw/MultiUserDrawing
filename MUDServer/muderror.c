/* muderror.c   */
/* Error routines for MUD server    */
/*  */
/* lain 011erenshaw 18/12/97    */
/* Version Histroy: */
/* Date Who Comments    */
/* 18/12/97 IO  Created */
/*  */
#include <stdio.h>
#include "muderror.h"

/* Send an error message to stderr*/
void sendError (char *error)
{
    fprintf(stderr, "ERROR: ");
    fprintf(stderr, error);
    fprintf(stderr, "\n");
}