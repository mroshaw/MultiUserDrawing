/*	main.c	                                                                        	*/
/*	Code to set up Tcl interpreter, initialise the Tcl scripts and handle client socket		*/
/*	requests		                                                                        */
/*	lain 011erenshaw 16/2/98		                                                        */
/*	Version History:		                                                                */
/*	Date	    Who	    Comments	                                                        */
/*	            MND	    Created	                                                            */
/*	16/2/98	10	Altered for MUD		                                                        */

#define USE_INTERP_RESULT
#define USE_INTERP_ERRORLINE

#include <stdio.h>
#include <tcl.h>
#include <tk.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include "utils.h"

int main(int argc, char *argv[])
{
	Tcl_Interp *myinterp;
	int error;
	char *winWorkingDir;
	char workingDir[PATH_MAX];
	char *tclWorkingDir;
	char const *errorString;

	/* Create our Tcl interpreter */
	myinterp = Tcl_CreateInterp();

	/* Initialise the Tcl and Tk packages	*/
	error = Tcl_AppInit(myinterp);
	if (error==TCL_ERROR)
	{
	    fprintf(stderr, "Error in AppInit! %i\n", error);
	    return 0;
	}

	/* Source the init.tcl file and we are done!	*/
    if (getcwd(workingDir, sizeof(workingDir)) != NULL) {
        fprintf(stderr, "Current UN*X working directory: %s\n", workingDir);
    }
    else
    {
        fprintf(stderr, "Failed to get working directory. Using root . ");
        strcpy(workingDir, ".");
    }

    /* Get the Windows working DIR */
    winWorkingDir = str_replace(workingDir, "/cygdrive/c", "C:");
    fprintf(stderr, "Current WIN working directory: %s\n", winWorkingDir);

    /* Get Tcl dir */
    tclWorkingDir = strcat(winWorkingDir, "/Tcl/init.tcl");
    fprintf(stderr, "TCL working directory: %s\n", tclWorkingDir);

	// error = Tcl_EvalFile(myinterp, "C:\\MUD\\MUDClient\\Tcl\\init.tcl");
    error = Tcl_EvalFile(myinterp, tclWorkingDir);

    /* If we get a Tcl parse error, display the detail and fail nicely */
	if (error==TCL_ERROR)
    {
        errorString = Tcl_GetStringResult(myinterp);
        fprintf(stderr, "Error in eval!\nPlease contact ioadcs.stand.ac.uk for help! %i\n", error);
        fprintf(stderr, "%s\n", errorString);
        return 0;
    }
    else
    {
        // Run the Tk loop until user quites
        Tk_MainLoop();
    }
	return 0;
}
