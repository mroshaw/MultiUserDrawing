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
#include "../common/utils.h"
#include "muderror.h"

extern Scene global_scene;

/* De-allocates all objects */
void clear_scene() {
    Object info = (Object) safe_malloc(sizeof(struct object));
    while (global_scene->start != NULL) {
        info = global_scene->start->next;
        free(info);
        global_scene->start = info;
    }
    /* Reset top and last pointers  */
    global_scene->start = NULL;
    global_scene->last = NULL;
}

/* Save scene to a file */
void save_scene(char *file_name) {
    Object info = global_scene->start;
    FILE *file;
    int count = 0;
    file = fopen(file_name, "wb");
    if (!file) {
        fprintf(stderr,"SAVE: Cannot open file!");
    } else {
        fprintf(stdout, "Saving to: %s\n", file_name);
        while (info != NULL) {
            fwrite(info, sizeof(struct object), 1, file);
            info = info->next;
            count++;
        }
    }
    fclose(file);
    /* fprintf(stdout, "Saved %i records\n", count);    */
}

/* Load file information into new document  */
void load_scene(char *file_name) {
    Object info;
    int count = 0;
    FILE *file;
    file = fopen(file_name, "rb");
    if (file == NULL) {
        /* Create a new file!   */
        fprintf(stdin,"File not found! Creating a new one...\n");
        file = fopen(file_name, "wb");
        fclose(file);
        fprintf(stdin, "Done!");
    } else {
        /* Free all allocated memory! */
        clear_scene();
        while (!feof(file)) {
            info = (Object) malloc(sizeof(struct object));
            if (!info) {
                fprintf(stderr,"LOAD: Out of memory! (Bummer...)");
                return;
            }
            if (1 != fread(info, sizeof(struct object), 1, file)) {
                break;
            }
            dls_store(info, &global_scene->start, &global_scene->last);
            count++;
        }
    }
    fclose(file);
    printf("Loaded %i objects.\n", count);
}

/*
void source (Scene global_scene, char *fName)
{
    FILE *fp;
    fp = fopen (fName, "r");
    if (!fp)
    {
        sendError ("SOURCE: Cannot open file!");
    }
    else
    {
        getInput(fp, global_scene);
        fclose (fp);
    }
}
*/
