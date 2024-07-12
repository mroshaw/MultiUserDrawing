/* parse.h                                          */
/* Routines for parsing and handling MUDScript      */
/* strings. Also provides debug facilities.         */
/*                                                  */
/* Iain O11erenshaw 13/10/97                        */
/*                                                  */
/* Version History:                                 */
/* Date     Who     Comments                        */
/* 13/10/97 IO      Created                         */
#ifndef PARSE_H
#define PARSE_H
#include "serverdetails.h"

void parseScript(char *inString, char *resultString, ServerDetails *serverDetails);

void testParseScript(char *theString, ServerDetails *serverDetails);

void testParse(ServerDetails *serverDetails);
#endif // PARSE_H
