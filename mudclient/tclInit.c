/* tclInit.c                                                        */
/* Main initialisation function for the Tcl interpreter and        */
/* the binding of command objects                                   */
/*                                                                  */
/* Iain Ollerenshaw 16/2/98                                         */
/*                                                                  */
/* Version History:                                                 */
/* Date         Who     Comments                                    */
/* 16/2/98      IO      Created                                     */
/*                                                                  */
#include <stdio.h>
#include <stdlib.h>
#include <tcl.h>
#include <tk.h>
#include "tclInit.h"
#include "../common/platform.h"

int Tcl_AppInit(Tcl_Interp *interp)
{
    
#ifdef _WIN32
    Tcl_SetVar(interp, "tcl_library", "./tcl8.6", TCL_GLOBAL_ONLY);
    Tcl_SetVar(interp, "tk_library", "./tk8.6", TCL_GLOBAL_ONLY);
#endif

    if (Tcl_Init(interp) == TCL_ERROR) {
        fprintf(stderr, "Error in Tcl_Init: %s\n", Tcl_GetStringResult(interp));
        return TCL_ERROR;
    }
    if (Tk_Init(interp) == TCL_ERROR) {
        fprintf(stderr, "Error in Tk_Init: %s\n", Tcl_GetStringResult(interp));
        return TCL_ERROR;
    }
    Command_Init(interp);
    return TCL_OK;
}

void Command_Init(Tcl_Interp *interp)
{
    Tcl_CreateCommand(interp, "sendString",
        (Tcl_CmdProc *) send_string_cmd,       (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "receiveString",
        (Tcl_CmdProc *) receive_string_cmd,    (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "getUID",
        (Tcl_CmdProc *) get_uid_cmd,           (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "sendReceiveString",
        (Tcl_CmdProc *) process_string_cmd,    (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "openClient",
        (Tcl_CmdProc *) open_socket_cmd,       (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "closeClient",
        (Tcl_CmdProc *) close_socket_cmd,      (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
}