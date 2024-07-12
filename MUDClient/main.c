/*	main.c	                                                                            	*/
/*	Main function to initiate the client MUD application                            		*/
/*	requests		                                                                        */
/*                                                                                          */
/*	Iain Ollerenshaw 16/2/98		                                                        */
/*                                                                                          */
/*	Version History:		                                                                */
/*	Date	    Who	    Comments	                                                        */
/*	            MND	    Created	                                                            */
/*	16/2/98     IO      Altered for MUD		                                                */

#define USE_INTERP_RESULT
#define USE_INTERP_ERRORLINE

#include <stdio.h>
#include <tcl.h>
#include <tk.h>
#include <unistd.h>
#include <limits.h>
#include "main.h"

#include <windows.h>

int main(int argc, char *argv[]) {
    Tcl_Interp *myinterp;
    int error;
    char workingDir[PATH_MAX];
    char *cygPath;
    char *tclInitPath;
    char const *errorString;

    /* int cId = (int)getuid(); */
    int cId = 1;
    fprintf(stdout, "Weclome to MUD, user: %i\n", cId);
    fprintf(stdout, "Client Version: %f\n", VERSION);
    /* Create our Tcl interpreter */
    myinterp = Tcl_CreateInterp();

    /* Initialise the Tcl and Tk packages	*/
    error = Tcl_AppInit(myinterp);
    if (error == TCL_ERROR) {
        fprintf(stderr, "Error in AppInit! %i\n", error);
        return 0;
    }


    /* Get Tcl dir */
    TCHAR currdir[255];
    DWORD dwRet;
    dwRet = GetCurrentDirectory(255, currdir);
    // fprintf(stdout, "Current directory: %s\n", currdir);
    tclInitPath = strcat(currdir, "/Tcl/init.tcl");
    fprintf(stdout, "TCL init path: %s\n", tclInitPath);
    error = Tcl_EvalFile(myinterp, tclInitPath);

    /* If we get a Tcl parse error, display the detail and fail nicely */
    if (error == TCL_ERROR) {
        errorString = Tcl_GetStringResult(myinterp);
        fprintf(stderr, "Error in eval!\nPlease contact ioadcs.stand.ac.uk for help! %i\n", error);
        fprintf(stderr, "%s\n", errorString);
        return 0;
    } else {
        // Run the Tk loop until user quites
        Tk_MainLoop();
    }
    return 0;
}
