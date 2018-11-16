/* scene.h  */
/* Routines for handling scenes.    */
/*  */
/* lain 011erenshaw 13/10/97    */
/* Version History: */
/* Date Who Comments    */
/* 13/10/97 IO  Created */
/*  */
/* The scene is a doubly linked list of objects */
struct scene
{
    struct object *start;
    struct object *last;
};

typedef struct scene *Scene;

/* Create a new scene   */
Scene mkScene ();
