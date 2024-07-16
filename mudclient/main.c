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
#include "winclient.h"

#include <windows.h>

int main(int argc, char *argv[]) {
    Tcl_Interp *tcl_interp;
    int tcl_result_code;
    char *tcl_init_path;
    char const *error_string;

    /* int cId = (int)getuid(); */
    int client_id = 1;
    fprintf(stdout, "Weclome to MUD, user: %i\n", client_id);
    fprintf(stdout, "Client Version: %f\n", VERSION);
    /* Create our Tcl interpreter */
    tcl_interp = Tcl_CreateInterp();

    /* Initialise the Tcl and Tk packages	*/
    tcl_result_code = Tcl_AppInit(tcl_interp);
    if (tcl_result_code == TCL_ERROR) {
        fprintf(stderr, "Error in AppInit! %i\n", tcl_result_code);
        return 0;
    }

    server_connect(argv[1], argv[2]);

    /* Get Tcl dir */
    TCHAR current_dir[255];
    DWORD dwRet;
    dwRet = GetCurrentDirectory(255, current_dir);
    // fprintf(stdout, "Current directory: %s\n", currdir);
    tcl_init_path = strcat(current_dir, "/Tcl/init.tcl");
    fprintf(stdout, "TCL init path: %s\n", tcl_init_path);
    tcl_result_code = Tcl_EvalFile(tcl_interp, tcl_init_path);

    /* If we get a Tcl parse error, display the detail and fail nicely */
    if (tcl_result_code == TCL_ERROR) {
        error_string = Tcl_GetStringResult(tcl_interp);
        fprintf(stderr, "Error in eval!\nPlease contact ioadcs.stand.ac.uk for help! %i\n", tcl_result_code);
        fprintf(stderr, "%s\n", error_string);
        return 0;
    } else {
        // Run the Tk loop until user quits
        Tk_MainLoop();
    }
    return 0;
}
