/* main.c                                                           */
/* Main function to initiate the client MUD application            */
/*                                                                  */
/* Iain Ollerenshaw 16/2/98                                         */
/*                                                                  */
/* Version History:                                                 */
/* Date         Who     Comments                                    */
/*              MND     Created                                     */
/* 16/2/98      IO      Altered for MUD                             */
/*                                                                  */
#define USE_INTERP_RESULT
#define USE_INTERP_ERRORLINE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tcl.h>
#include <tk.h>
#include "main.h"
#include "../common/platform.h"

int main(int argc, char *argv[])
{
    Tcl_Interp *myinterp;
    int error;
    const char *errorString;
    char current_dir[PATH_MAX];
    char tcl_init_path[PATH_MAX + 20];

	fprintf(stdout, "Running MudClient...\n");

    /* Create our Tcl interpreter */
    myinterp = Tcl_CreateInterp();

    /* Initialise the Tcl and Tk packages */
    error = Tcl_AppInit(myinterp);
    if (error == TCL_ERROR) {
        fprintf(stderr, "Error in AppInit! %i\n", error);
        return 0;
    }

    /* Get current directory and build Tcl init path */
    getcwd(current_dir, PATH_MAX);
    fprintf(stdout, "Current directory: %s\n", current_dir);
    strcpy(tcl_init_path, current_dir);
    strcat(tcl_init_path, PATH_SEP "Tcl" PATH_SEP "init.tcl");
    fprintf(stdout, "TCL init path: %s\n", tcl_init_path);

    error = Tcl_EvalFile(myinterp, tcl_init_path);
    if (error == TCL_ERROR) {
        errorString = Tcl_GetStringResult(myinterp);
        fprintf(stderr, "Error in eval! %i\n", error);
        fprintf(stderr, "%s\n", errorString);
        return 0;
    } else {
        Tk_MainLoop();
    }
    return 0;
}