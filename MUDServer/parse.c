/* parse.c                                          */
/* Routines for parsing and handling MUDScript      */
/* strings. Also provides debug facilities.         */
/*                                                  */
/* Iain O11erenshaw 13/10/97                        */
/*                                                  */
/* Version History:                                 */
/* Date     Who     Comments                        */
/* 13/10/97 IO      Created                         */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "object.h"
#include "parse.h"
#include "help.h"
#include "utils.h"
#include "file.h"
#include "muderror.h"

/* This procedure parses the script as given by the inString string. */
/* All script commands should be preceded by the user id of the client requesting the   */
/* database */

/* Use global details   */
extern SDet details;

/* Parse script string. Result is stored in resultString
*/ void parseScript (char *inString, char *resultString)
{
    /* These variable hold arguments given to each script command */
    int clientId;
    char command[10]="";
    char target[10];
    char targetUpper[10];
    char arg1[10];
    int arg2;
    int arg3;
    int arg4;
    int arg5;
    int arg6;
    /* This is used only by the create text script command  */
    char theText[20];

    /* Result of a procedure    */
    int result;

    /* These objects can be used privately by any of the func below. Such */
    /* as findObject and copyObjects
    */
    Object tempObject = (Object) safemalloc (sizeof (struct object));
    Object newObject = (Object) safemalloc (sizeof (struct object));

    /* Okay so far! */
    strcpy(resultString, "-9");
    /* fprintf(stdout, "Now processing: %s/n", inString);*/
    /* Retrieve the parameters. */
    sscanf(inString, "%i %s %s %s %i %i %i %i %i %s", &clientId, command, target, &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, theText);

    /* Convert to upper case for ease   */
    stringToUpper(command, command);
    stringToUpper(target, targetUpper);

    if (strcmp("CREATE",command)==0)
    {
        if (strcmp("CIRCLE", target)==0)
        {
            newObject = createObject (clientId, 1, arg1, arg2, arg3, arg4, arg5, "");
            if (newObject == NULL)
            {
                strcpy(resultString, CREATEERROR);
            }
            else
            {
                result = enterObject (newObject);
                if (result==0)
                    strcpy( resultString,ENTERERROR);
                else
                    strcpy( resultString,OKAY);
            }
        }
        else if (strcmp("BOX",targetUpper)==0)
        {
            newObject = createObject (clientId, 2, arg1,arg2, arg3, arg4, arg5, "");
            if (newObject == NULL)
            {
                strcpy(resultString, CREATEERROR);
            }
            else
            {
                result = enterObject (newObject);
                if (result==0)
                    strcpy(resultString, ENTERERROR);
                else
                    strcpy( resultString,OKAY);
            }
        }
        else if (strcmp("LINE",targetUpper)==0)
        {
            newObject = createObject (clientId, 3, arg1,arg2, arg3, arg4, arg5, "");
            if (newObject == NULL)
            {
                strcpy(resultString, CREATEERROR);
            }
            else
            {
                result = enterObject (newObject);
                if (result==0)
                    strcpy(resultString, ENTERERROR);
                else
                    strcpy( resultString,OKAY);
            }
        }
        else if (strcmp("TEXT",targetUpper)==0)
        {
            newObject = createObject (clientId, 4, arg1,arg2, arg3, arg4, arg5, theText);
            if (newObject == NULL)
            {
                strcpy(resultString, CREATEERROR);
            }
            else
            {
                result = enterObject (newObject);
                 if (result==0)
                 {
                      strcpy(resultString, ENTERERROR);
                 }
                 else
                 {
                     strcpy( resultString,OKAY);
                 }
            }
        }
        else
        {
            strcpy(resultString, OBJECTERROR);
        }
    }
    else if (strcmp("DELETE", command)==0)
    {
        result = removeObject (clientId, target);
        if (result==0)
        {
            strcpy(resultString, DELETEERROR);
        }
        else
        {
            strcpy(resultString,OKAY);
        }
    }
    else if (strcmp("DRAW", command)==0)
    {
        newObject=findObject(target);
        if (newObject!=NULL)
        {
            drawObject (newObject, resultString);
        }
        else
        {
            strcpy(resultString, EXISTERROR);
        }
    }
    else if (strcmp("DRAWALL", command)==0)
    {
        drawAllObjects(resultString);
    }
    else if (strcmp("EDIT",command)==0)
    {
        printf("Before find");
        tempObject = findObject(target);
        fprintf(stdout, "After find: %s", tempObject->name);

        if (tempObject==NULL)
        {
            strcpy(resultString, EXISTERROR);
        }
        else
        {
            result = editObject (clientId, tempObject, arg2, arg3, arg4, arg5, arg6, arg1);
            if (result == 0)
            {
                strcpy(resultString, EDITERROR);
            }
            else
            {
                strcpy( resultString, OKAY);
            }
        }
    }
    else if (strcmp("COPY", command)==0)
    {
        tempObject=findObject(target);
        if (tempObject)
        {
            copyObjects(clientId, tempObject, newObject, arg1);
            printObject (newObject);
            enterObject (newObject);
            strcpy( resultString,OKAY);
        }
        else
        {
            strcpy(resultString, EXISTERROR);
        }
    }
    else if (strcmp("SHOWALL", command)==0)
        showObjects ();
    else if (strcmp("DRAWBYNUM", command)==0)
    {
        arg2 = atoi(target);
        drawByNum(arg2, resultString);
    }
    else if (strcmp("SAVE", command)==0)
    {
        save (details->file);
        strcpy( resultString,OKAY);
    }
    else if (strcmp("LOAD", command)==0)
    {
        load (details->file);
        strcpy( resultString,OKAY);
    }
    else if (strcmp ("SOURCE", command)==0)
    {
        /* source (target); */
    }
    else if (strcmp("LOCK", command)==0)
    {
        newObject=findObject(target);
        if (newObject == NULL)
            strcpy(resultString, EXISTERROR);
        else
        {
            result = lockObject (clientId, newObject);
            if (result == 0)
                strcpy(resultString, LOCKEDERROR);
        else
            strcpy( resultString,OKAY);
        }
    }
    else if (strcmp("UNLOCK", command)==0)
    {
        newObject=findObject(target);
        if (newObject == NULL)
        {
            strcpy( resultString, EXISTERROR);
        }
        else
        {
            result = unlockObject (clientId, newObject);
            if (result == 0)
            {
                strcpy(resultString, LOCKEDERROR);
            }
            else
            {
                strcpy( resultString,OKAY);
            }
       }
   }
    else if (strcmp("CLEAR", command)==0)
    {
        clear ();
    }
    else if (strcmp("HELP", command)==0)
    {
        doHelp();
    }
    else if (strcmp("EXIT", command)!=0)
    {
        strcpy(resultString, COMMANDERROR);
    }
    else
    {
        printf("Bye bye!");
        save (details->file);
        exit(0);
    }
}
/* Lets us test the script parser   */
/* For debugging ONLY   */
void testParseScript (char *theString)
{
    char result[255];
    parseScript (theString, result);
    printf("Result: %s\n", result);
}

/* Allows us to test the server without a connection */
void testParse(void)
{
    char string[255];
    strcpy (string, "");
    while (strcmp(string,"EXIT\n") != 0)
    {
        printf("& ");
        getLine(string, 255);
        testParseScript (string);
    }
}
