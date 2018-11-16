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
void stringToUpper (char *nl, char *n2);

/* Safe allocation of memory */
void *safemalloc(size_t size);

/* Convert a string to an integer */
int stoi (char *nl);

/* Generic procedure to convert integer to a stri void itoa (int theInt, char theString[]); */
/* Get a line from std input */
int getLine (char *line, int max);

/* Generic procedure to convert integer to a string */
void my_itoa (int theInt, char theString[]);

/* Replace an occurrence of a string within a string */
char *str_replace(char *orig, char *rep, char *with);
