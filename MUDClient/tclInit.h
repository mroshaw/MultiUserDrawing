/*	tclInit.h	                                                                            */
/*	Main initialisation function for the Tcl interpreter and the binding command objects    */
/*	requests		                                                                        */
/*                                                                                          */
/*	Iain Ollerenshaw 16/2/98		                                                        */
/*                                                                                          */
/*	Version History:		                                                                */
/*	Date	    Who	    Comments	                                                        */
/*	16/2/98     IO      Created 		                                                    */

int Tcl_AppInit (Tcl_Interp *interp);
void Command_Init (Tcl_Interp *interp);
