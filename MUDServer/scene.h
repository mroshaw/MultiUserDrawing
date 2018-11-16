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
struct scene
{
    struct object *start;
    struct object *last;
};

typedef struct scene *Scene;

/* Create a new scene   */
Scene mkScene ();
