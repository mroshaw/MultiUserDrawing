/* object.h                                     */
/* Routines for handling object storage and     */
/* manipulation                                 */
/*                                              */
/* Iain Ollerenshaw 13/10/97                    */
/*                                              */
/* Version History:                             */
/* Date     Who     Comments                    */
/* 13/10/97 IO      Created                     */
/*                                              */

/* Define the object structure andthe doubly linked list method with */
/* which the objects will be stored */
#ifndef OBJECT_H
#define OBJECT_H

struct object {
    char name[20]; /* Unique name  */
    int type; /* 0 - circle, 1 - box, 2 - line, 3 - text */
    int x1; /* Co-ords for topleft and bottom right */
    int y1; /* Circle is now an OVAL - IO 24/2/98 */
    int x2;
    int y2;
    int colour; /* Object colour    */
    int locked; /* 0 if unlocked, otherwise users id */
    int layer; /* Layer within doc. 0 if front most, 9999 if rearmost  */
    char text[255]; /* If text, string. Otherwise blank */
    struct object *previous;
    struct object *next;
};

typedef struct object *Object;

/* Function prototypes  */
Object create_object(int client_id, int type, char name[20], int x1, int y1, int x2, int y2, char object_text[20]);

void dls_store(Object new_object, Object *start, Object *last);

int enter_object(Object object_to_enter);

int remove_object(int client_id, char name_to_find[]);

Object find_object(char *name);

void show_objects();

void draw_all_objects(char *result_string);

void draw_object_by_id(int object_id, char *result_string);

void copy_objects(int clientId, Object source_object, Object target_object, char *newName);

void print_object(Object object_to_print);

int lock_object(int client_id, Object object_to_lock);

int unlock_object(int client_id, Object object_to_unlock);

void draw_object(Object object_to_draw, char *result_string);

int edit_object(int client_id, Object object_to_edit, int new_x1, int new_y1, int new_x2, int new_y2, int new_layer,
                char *new_name);
#endif // OBJECT_H
