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
#include "serverconfig.h"

void parse_script(char *script_string, char *result_string, ServerConfig *server_config);

void test_parse_script(char *script_string, ServerConfig *server_config);

void test_parse(ServerConfig *server_config);
#endif // PARSE_H
