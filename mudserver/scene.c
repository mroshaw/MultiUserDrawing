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
#include "../common/utils.h"
#include "scene.h"

/* Create a new scene   */
Scene make_scene() {
    Scene new_scene = (Scene) safe_malloc(sizeof(struct scene));
    new_scene->start = NULL;
    new_scene->last = NULL;
    return new_scene;
}
