/* scene.h                          */
/* Routines for handling scenes.    */
/*                                  */
/* Iain Ollerenshaw 13/10/97        */
/*                                  */
/* Version History:                 */
/* Date     Who     Comments        */
/* 13/10/97 I0      Created         */
/*                                  */

/* The scene is a doubly linked list of objects */
#ifndef SCENE_H
#define SCENE_H

struct scene {
    struct object *start;
    struct object *last;
};

typedef struct scene *Scene;

/* Create a new scene   */
Scene make_scene();
#endif // SCENE_H
