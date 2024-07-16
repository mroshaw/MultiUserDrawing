/* parse.c                                          */
/* Routines for parsing and handling MUDScript      */
/* strings. Also provides debug facilities.         */
/*                                                  */
/* Ia
O11erenshaw 13/10/97                        */
/*                                                  */
/* Version History:                                 */
/* Date     Who     Comments                        */
/* 13/10/97 IO      Created                         */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"
#include "parse.h"
#include "help.h"
#include "../common/utils.h"
#include "file.h"
#include "muderror.h"
#include "serverconfig.h"

/* This procedure parses the script as given by the inString string. */
/* All script commands should be preceded by the user id of the client requesting the   */
/* database */

/* Parse script string. Result is stored in result_string
*/
void parse_script(char *script_string, char *result_string, ServerConfig *server_config) {
    /* These variable hold arguments given to each script command */
    int client_id;
    char command[10] = "";
    char target[10];
    char target_upper[10];
    char arg1[10];
    int arg2;
    int arg3;
    int arg4;
    int arg5;
    int arg6;
    /* This is used only by the create text script command  */
    char label_text[20];

    /* Result of a procedure    */
    int result;

    printf("Parsing: %s\n", script_string);

    /* These objects can be used privately by any of the func below. Such */
    /* as find_object and copy_objects
    */
    Object temp_object = (Object) safe_malloc(sizeof(struct object));
    Object new_object = (Object) safe_malloc(sizeof(struct object));

    /* Okay so far! */
    strcpy(result_string, "-9");
    /* fprintf(stdout, "Now processing: %s/n", script_string);*/
    /* Retrieve the parameters. */
    sscanf(script_string, "%i %s %s %s %i %i %i %i %i %s", &client_id, command, target, &arg1, &arg2, &arg3, &arg4, &arg5,
           &arg6, label_text);

    /* Convert to upper case for ease   */
    string_to_upper(command, command);
    string_to_upper(target, target_upper);

    if (strcmp("CREATE", command) == 0) {
        if (strcmp("CIRCLE", target) == 0) {
            new_object = create_object(client_id, 1, arg1, arg2, arg3, arg4, arg5, "");
            if (new_object == NULL) {
                strcpy(result_string, CREATEERROR);
            } else {
                result = enter_object(new_object);
                if (result == 0)
                    strcpy(result_string,ENTERERROR);
                else
                    strcpy(result_string,OKAY);
            }
        } else if (strcmp("BOX", target_upper) == 0) {
            new_object = create_object(client_id, 2, arg1, arg2, arg3, arg4, arg5, "");
            if (new_object == NULL) {
                strcpy(result_string, CREATEERROR);
            } else {
                result = enter_object(new_object);
                if (result == 0)
                    strcpy(result_string, ENTERERROR);
                else
                    strcpy(result_string,OKAY);
            }
        } else if (strcmp("LINE", target_upper) == 0) {
            new_object = create_object(client_id, 3, arg1, arg2, arg3, arg4, arg5, "");
            if (new_object == NULL) {
                strcpy(result_string, CREATEERROR);
            } else {
                result = enter_object(new_object);
                if (result == 0)
                    strcpy(result_string, ENTERERROR);
                else
                    strcpy(result_string,OKAY);
            }
        } else if (strcmp("TEXT", target_upper) == 0) {
            new_object = create_object(client_id, 4, arg1, arg2, arg3, arg4, arg5, label_text);
            if (new_object == NULL) {
                strcpy(result_string, CREATEERROR);
            } else {
                result = enter_object(new_object);
                if (result == 0) {
                    strcpy(result_string, ENTERERROR);
                } else {
                    strcpy(result_string,OKAY);
                }
            }
        } else {
            strcpy(result_string, OBJECTERROR);
        }
    } else if (strcmp("DELETE", command) == 0) {
        result = remove_object(client_id, target);
        if (result == 0) {
            strcpy(result_string, DELETEERROR);
        } else {
            strcpy(result_string,OKAY);
        }
    } else if (strcmp("DRAW", command) == 0) {
        new_object = find_object(target);
        if (new_object != NULL) {
            draw_object(new_object, result_string);
        } else {
            strcpy(result_string, EXISTERROR);
        }
    } else if (strcmp("DRAWALL", command) == 0) {
        draw_all_objects(result_string);
    } else if (strcmp("EDIT", command) == 0) {
        temp_object = find_object(target);

        if (temp_object == NULL) {
            strcpy(result_string, EXISTERROR);
        } else {
            result = edit_object(client_id, temp_object, arg2, arg3, arg4, arg5, arg6, arg1);
            if (result == 0) {
                strcpy(result_string, EDITERROR);
            } else {
                strcpy(result_string, OKAY);
            }
        }
    } else if (strcmp("COPY", command) == 0) {
        temp_object = find_object(target);
        if (temp_object) {
            copy_objects(client_id, temp_object, new_object, arg1);
            print_object(new_object);
            enter_object(new_object);
            strcpy(result_string,OKAY);
        } else {
            strcpy(result_string, EXISTERROR);
        }
    } else if (strcmp("SHOWALL", command) == 0)
        show_objects();
    else if (strcmp("DRAWBYNUM", command) == 0) {
        arg2 = atoi(target);
        draw_object_by_id(arg2, result_string);
    } else if (strcmp("SAVE", command) == 0) {
        save_scene(server_config->file_name);
        strcpy(result_string,OKAY);
    } else if (strcmp("LOAD", command) == 0) {
        load_scene(server_config->file_name);
        strcpy(result_string,OKAY);
    } else if (strcmp("SOURCE", command) == 0) {
        /* source (target); */
    } else if (strcmp("LOCK", command) == 0) {
        new_object = find_object(target);
        if (new_object == NULL)
            strcpy(result_string, EXISTERROR);
        else {
            result = lock_object(client_id, new_object);
            if (result == 0)
                strcpy(result_string, LOCKEDERROR);
            else
                strcpy(result_string,OKAY);
        }
    } else if (strcmp("UNLOCK", command) == 0) {
        new_object = find_object(target);
        if (new_object == NULL) {
            strcpy(result_string, EXISTERROR);
        } else {
            result = unlock_object(client_id, new_object);
            if (result == 0) {
                strcpy(result_string, LOCKEDERROR);
            } else {
                strcpy(result_string,OKAY);
            }
        }
    } else if (strcmp("CLEAR", command) == 0) {
        clear_scene();
    } else if (strcmp("HELP", command) == 0) {
        show_help();
    } else if (strcmp("EXIT", command) == 0) {
        printf("Bye bye!");
        save_scene(server_config->file_name);
        exit(0);
    } else {
        strcpy(result_string, COMMANDERROR);
    }
}

/* Lets us test the script parser   */
/* For debugging ONLY   */
void test_parse_script(char *script_string, ServerConfig *server_config) {
    char result[255];
    parse_script(script_string, result, server_config);
    printf("Result: %s\n", result);
}

/* Allows us to test the server without a connection */
void test_parse(ServerConfig *server_config) {
    char input_string[255];
    strcpy(input_string, "");
    while (strcmp(input_string, "EXIT\n") != 0) {
        printf("& ");
        get_line(input_string, 255);
        test_parse_script(input_string, server_config);
    }
}
