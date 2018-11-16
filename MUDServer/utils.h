/* utils.h                                  */
/* Misc utility functions                   */
/*                                          */
/* Iain Ollerenshaw 13/10/97                */
/*                                          */
/* Version History:                         */
/* Date     Who     Comments                */
/* 13/10/97 IO      Created                 */
/*                                          */

/* Converts a string to upper case */
void stringToUpper (char *n1, char *n2);

/* Safe allocation of memory */
void *safemalloc(size_t size);

/* Convert a string to an integer */
int stoi (char *n1);

/* Generic procedure to convert integer to a string */
void my_itoa (int thelnt, char theString[]);

/* Get a line from std input */
int getLine (char *line, int max);
