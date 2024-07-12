/* scene.c                          */
/* Routines for handling scenes.    */
/*                                  */
/* Iain Ollerenshaw 13/10/97        */
/*                                  */
/* Version History:                 */
/* Date     Who     Comments        */
/* 13/10/97 I0      Created         */
/*                                  */

#include <stdlib.h>
#include "utils.h"
#include "scene.h"

/* Create a new scene   */
Scene mkScene() {
    Scene theScene = (Scene) safemalloc(sizeof(struct scene));
    theScene->start = NULL;
    theScene->last = NULL;
    return theScene;
}
