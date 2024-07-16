/* utils.h	                                                                        */
/* This file contains misc functions to perform various functions                   */
/* requests		                                                                    */
/*                                                                                  */
/* Iain Ollerenshaw 16/2/98		                                                    */
/*                                                                                  */
/* Version History:		                                                            */
/* Date	       Who	   Comments	                                                    */
/* 13/10/97    IO      Created 		                                                */

/* Converts a string to upper case*/
#ifndef UTILS_H
#define UTILS_H

void string_to_upper(char *nl, char *n2);

/* Safe allocation of memory */
void *safe_malloc(size_t size);

/* Convert a string to an integer */
int string_to_int(char *nl);

/* Generic procedure to convert integer to a stri void itoa (int theInt, char theString[]); */
/* Get a line from std input */
int get_line(char *line, int max);

/* Generic procedure to convert integer to a string */
void int_to_string(int the_int, char the_string[]);

/* Replace an occurrence of a string within a string */
char *str_replace(char *orig, char *rep, char *with);
#endif // UTILS_H
