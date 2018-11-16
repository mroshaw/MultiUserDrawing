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

void doHelp()
{
    int c;
    FILE *helpFile;
    helpFile = fopen("helpFile.txt", "r");
    if (!helpFile)
    {
        fprintf(stdout, "Cannot open help file!");
	}
    else
	{
	    while ((c=getc(helpFile)) !=EOF)
		{
			fprintf(stdout, "%c", c);
		}
        fclose (helpFile);
    }
}
