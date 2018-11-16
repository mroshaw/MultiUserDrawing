#include <stdio.h>
#include <stdlib.h>
#include <C:\ActiveTcl86\include\tcl.h>
#include <C:\ActiveTcl86\include\tk.h>
#include "client.h"
#include "tclInit.h"

int Tcl_AppInit (Tcl_Interp *interp)
{
	/* Init packages */
	if (Tcl_Init(interp)== TCL_ERROR)
	{
		printf("Error in Tcl_Init\n");
		return TCL_ERROR;
	}

	if (Tk_Init(interp)== TCL_ERROR)
	{
		printf("Error in Tk_Init\n");
		return TCL_ERROR;
	}

	/* Register app specfific commands */
	Command_Init(interp);
	return TCL_OK;
}

void Command_Init (Tcl_Interp *interp)
{
	/* Binds the Tcl command sendString to the C command sendStringCmc */
	Tcl_CreateCommand(interp, "sendString", (Tcl_CmdProc *)sendStringCmd, (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
	/* Binds the Tcl command receiveString to the C command receiveStringCmd */
	Tcl_CreateCommand(interp, "receiveString", (Tcl_CmdProc *)receiveStringCmd, (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
	/* Binds the Tcl command getUID TO the C command getUlDCmd */
	Tcl_CreateCommand(interp, "getUID", (Tcl_CmdProc *)getUIDCmd, (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
}
