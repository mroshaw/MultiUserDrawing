/* file.c                                           */
/* Routines for loading, saving a clearing data     */
/* in server                                        */
/*                                                  */
/* Iain Ollerenshaw 13/10/97                        */
/*                                                  */
/* Version History:                                 */
/* Date     Who     Comments                        */
/* 13/10/97 IO      Created                         */
/*                                                  */

#include <stdio.h>
#include <stdlib.h>
#include "object.h"
#include "scene.h"
#include "file.h"
#include "utils.h"
#include "parse.h"
#include "muderror.h"

extern Scene theScene;

/* De-allocates all objects */
void clear ()
{
    Object info = (Object) safemalloc (sizeof (struct object));
    while (theScene->start != NULL)
    {
        info = theScene->start->next;
        free(info);
        theScene->start=info;
    }
    /* Reset top and last pointers  */
    theScene->start = NULL;
    theScene->last = NULL;

}

/* Save scene to a file */
void save (char *fName)
{
    Object info = theScene->start;
    FILE *fp;
    int count = 0;
    fp = fopen(fName, "wb");
    if (!fp)
    {
        sendError("SAVE: Cannot open file!");
    }
    else
    {
        while(info != NULL)
        {
            fwrite(info, sizeof(struct object), 1, fp);
            info = info->next;
        count++;
        }
    }
    fclose(fp);
    /* fprintf(stdout, "Saved %i records\n", count);    */
}

/* Load file information into new document  */
void load (char *fName)
{
    Object info;
    int count=0;
    FILE *fp;
    fp = fopen(fName, "rb");
    if (fp == NULL)
    {
        /* Create a new file!   */
        sendError("File not found! Creating a new one...\n");
        fp = fopen(fName, "wb");
        fclose (fp);
        sendError("Done!");
    }
    else
    {
        /* Free all allocated memory! */
        clear ();
        while (!feof(fp))
        {
            info = (Object) malloc (sizeof (struct object));
            if (!info)
            {
                sendError("LOAD: Out of memory! (Bummer...)");
                return;
            }
            if (1!=fread(info, sizeof(struct object), 1, fp))
            {
                break;
            }
            dls_store (info, &theScene->start, &theScene->last);
            count++;
        }
    }
    fclose (fp);
    printf("Loaded %i objects.\n", count);
}

/*
void source (Scene theScene, char *fName)
{
    FILE *fp;
    fp = fopen (fName, "r");
    if (!fp)
    {
        sendError ("SOURCE: Cannot open file!");
    }
    else
    {
        getInput(fp, theScene);
        fclose (fp);
    }
}
*/
