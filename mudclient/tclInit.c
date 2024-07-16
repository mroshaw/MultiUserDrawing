/*	tclInit.c	                                                                            */
/*	Main initialisation function for the Tcl interpreter and the binding command objects    */
/*	requests		                                                                        */
/*                                                                                          */
/*	Iain Ollerenshaw 16/2/98		                                                        */
/*                                                                                          */
/*	Version History:		                                                                */
/*	Date	    Who	    Comments	                                                        */
/*	16/2/98     IO      Created 		                                                    */

#include <stdio.h>
#include <stdlib.h>
#include <tcl.h>
#include <tk.h>
#include "winclient.h"
#include "tclInit.h"

int Tcl_AppInit(Tcl_Interp *interp) {
    /* Init packages */
    if (Tcl_Init(interp) == TCL_ERROR) {
        printf("Error in Tcl_Init\n");
        return TCL_ERROR;
    }

    if (Tk_Init(interp) == TCL_ERROR) {
        printf("Error in Tk_Init\n");
        return TCL_ERROR;
    }

    /* Register app specfific commands */
    Command_Init(interp);
    return TCL_OK;
}

void Command_Init(Tcl_Interp *interp) {
    /* Binds the Tcl command sendString to the C command sendStringCmc */
    Tcl_CreateCommand(interp, "sendString", (Tcl_CmdProc *) send_string_cmd, (ClientData) NULL,
                      (Tcl_CmdDeleteProc *) NULL);
    /* Binds the Tcl command receiveString to the C command receive_string_cmd */
    Tcl_CreateCommand(interp, "receiveString", (Tcl_CmdProc *) receive_string_cmd, (ClientData) NULL,
                      (Tcl_CmdDeleteProc *) NULL);
    /* Binds the Tcl command getUID TO the C command getUlDCmd */
    Tcl_CreateCommand(interp, "getUID", (Tcl_CmdProc *) get_uid_cmd, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    /* Binds the Tcl command sendReceiveString TO the C command process_string_cmd */
    Tcl_CreateCommand(interp, "sendReceiveString", (Tcl_CmdProc *) process_string_cmd, (ClientData) NULL,
                      (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "closeClient", (Tcl_CmdProc *) close_client_cmd, (ClientData) NULL,
                      (Tcl_CmdDeleteProc *) NULL);
}
