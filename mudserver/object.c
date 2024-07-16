/* object.c                                     */
/* Routines for handling object storage and     */
/* manipulation                                 */
/*                                              */
/* Iain Ollerenshaw 13/10/97                    */
/*                                              */
/* Version History:                             */
/* Date     Who     Comments                    */
/* 13/10/97 IO      Created                     */
/*                                              */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __WIN32__
# include <winsock2.h>
#else
# include <sys/socket.h>
#endif
#include "muderror.h"
#include "matrix.h"
#include "object.h"
#include "../common/utils.h"
#include "server.h"
#include "scene.h"
extern Scene global_scene;
extern int them;

/* Finds an object in a scene. Returns NULL if not found.   */
Object find_object(char *name) {
    Object temp_object = (Object) safe_malloc(sizeof(struct object));
    temp_object = global_scene->start;

    while (temp_object != NULL) {
        if (!strcmp(name, temp_object->name)) {
            /* Found object */
            return temp_object;
        }
        temp_object = temp_object->next;
    }
    /* Object not found */
    return NULL;
}

/* Creates and returns a new object. Returns NULL if unsuccessful */
Object create_object(int client_id, int type, char name[20], int x1, int y1, int x2, int y2, char object_text[20]) {
    Object new_object = (Object) safe_malloc(sizeof(struct object));
    if (new_object != NULL) {
        new_object->type = type;
        strcpy(new_object->name, name);
        new_object->x1 = x1;
        new_object->y1 = y1;
        new_object->x2 = x2;
        new_object->y2 = y2;
        new_object->layer = 0; /* Initially front most */
        new_object->locked = client_id; /* Initially owned by creating user? */
        strcpy(new_object->text, object_text);
        return new_object;
    } else {
        return NULL; /* Unsuccessfull, out of memory?    */
    }
}

/* Create doubly linked list */
void dls_store(Object new_object, Object *start, Object *last) {
    Object old, p;
    if (*last == NULL) {
        new_object->next = NULL;
        new_object->previous = NULL;
        *last = new_object;
        *start = new_object;
        return;
    }

    p = *start; /* Start at top of list */
    old = NULL;
    while (p) {
        if (p->layer < new_object->layer) {
            old = p;
            p = p->next;
        } else {
            if (p->previous) {
                p->previous->next = new_object;
                new_object->next = p;
                new_object->previous = p->previous;
                p->previous = new_object;
                return;
            }
            new_object->next = p;
            new_object->previous = NULL;
            p->previous = new_object;
            *start = new_object;
            return;
        }
    }
    old->next = new_object;
    new_object->next = NULL;
    new_object->previous = old;
    *last = new_object;
}

/* Enter an object into the scene   */
/* Uses doubly linked list interface defined earlier */
int enter_object(Object object_to_enter) {
    Object temp = find_object(object_to_enter->name);
    if (temp == NULL) {
        dls_store(object_to_enter, &global_scene->start, &global_scene->last);
        return 1;
    } else
        return 0;
}

/* Remove an object from the specified scene    */
/* Client id must own object    */
int remove_object(int client_id, char name_to_find[]) {
    Object info = (Object) safe_malloc(sizeof(struct object));
    // Object find_object();
    Object *start = &global_scene->start;
    Object *last = &global_scene->last;

    info = find_object(name_to_find);

    /* User must own object!    */
    if (info && info->locked == client_id) {
        if (*start == info) {
            *start = info->next;
            if (*start) {
                (*start)->previous = NULL;
            } else {
                *last = NULL;
            }
        } else {
            info->previous->next = info->next;
            if (info != *last) {
                info->next->previous = info->previous;
            } else {
                *last = info->previous;
            }
            free(info);
        }
        return 1;
    } else if (info->locked != client_id) {
        /* Don't own object!    */
        return 0;
    } else {
        /* Unknown error. PANIC!    */
        return 0;
    }
}

/* Private debug procedure  */
void show_objects() {
    Object info = global_scene->start;
    while (info) {
        print_object(info);
        info = info->next;
    }
    fprintf(stdout, "\n\n");
}

/* Public procedure for getting ALL object information */
/* works by sending the client the number of objects */
/* then getting the client to collect objects by the */
/* draw_object_by_id procedure  */
void draw_all_objects(char *result_string) {
    int count = 0;
    char temp[20];
    Object info = global_scene->start;
    while (info) {
        count++;
        info = info->next;
    }
    int_to_string(count, temp);
    strcpy(result_string, temp);
}

/* Draw an object by its place in the list  */
void draw_object_by_id(int object_id, char *result_string) {
    int count = 0;
    Object info = global_scene->start;
    while (count < object_id) {
        count++;
        info = info->next;
    }
    draw_object(info, result_string);
}

/* Duplicate two objects. newName must be unique. Attributes of source_object */
/* will be copied into existing target_object. */
/* Will append clients id to new object making it theirs */
void copy_objects(int clientId, Object source_object, Object target_object, char *newName) {
    strcpy(target_object->name, newName);
    target_object->x1 = source_object->x1;
    target_object->y1 = source_object->y1;
    target_object->locked = clientId;
    target_object->type = source_object->type;
    target_object->x2 = source_object->x2;
    target_object->y2 = source_object->y2;
    target_object->layer = 0;
}

/* Private debug procedure  */
void print_object(Object object_to_print) {
    if (object_to_print == NULL) {
        fprintf(stderr,"PRINTOBJECT: Object is NULL\n");
    } else {
        fprintf(stdout, "\n");
        fprintf(stdout, "Object type : %i\n", object_to_print->type);
        fprintf(stdout, "Object name : %s\n", object_to_print->name);
        fprintf(stdout, "X1 : %i\n", object_to_print->x1);
        fprintf(stdout, "Y1 : %i\n", object_to_print->y1);
        fprintf(stdout, "X2 : %i\n", object_to_print->x2);
        fprintf(stdout, "Y2 : %i\n", object_to_print->y2);
        fprintf(stdout, "Layer  : %i\n", object_to_print->layer);
        fprintf(stdout, "Text (if text) : %s\n", object_to_print->text);
        fprintf(stdout, "Locked by: %i\n", object_to_print->locked);
    }
}

/* Lock an object with a user id    */
int lock_object(int client_id, Object object_to_lock) {
    if (object_to_lock->locked == client_id) {
        /* Owns object, but lock is successful! */
        ;
        return 1;
    } else if (object_to_lock->locked != 0) {
        /* Object belongs to someone else   */
        return 0;
    } else {
        /* Okay. Change object lock */
        object_to_lock->locked = client_id;
        return 1;
    }
}

/* Unlock an object */
int unlock_object(int client_id, Object object_to_unlock) {
    if (object_to_unlock->locked == client_id) {
        /* Client owns object and so can unlock it  */
        object_to_unlock->locked = 0;
        return 1;
    } else {
        /* Client does not own object!! */
        return 0;
    }
    /* draw_object(object_to_unlock);    */
}

/* Construct a string to send to the client */
void draw_object(Object object_to_draw, char *result) {
    /* Create the server string    */
    /* String is of the form:    */
    /* name, type, x1, y1, width, height, layer, locked */
    /* Client can use this information for manipulation purposes */
    char temp[20] = "";
    strcpy(result, object_to_draw->name);
    strcat(result, " ");
    int_to_string(object_to_draw->type, temp);
    strcat(result, temp);
    strcat(result, " ");
    int_to_string(object_to_draw->x1, temp);
    strcat(result, temp);
    strcat(result, " ");
    int_to_string(object_to_draw->y1, temp);
    strcat(result, temp);
    strcat(result, " ");
    int_to_string(object_to_draw->x2, temp);
    strcat(result, temp);
    strcat(result, " ");
    int_to_string(object_to_draw->y2, temp);
    strcat(result, temp);
    strcat(result, " ");
    int_to_string(object_to_draw->layer, temp);
    strcat(result, temp);
    strcat(result, " ");
    int_to_string(object_to_draw->locked, temp);
    strcat(result, temp);
    strcat(result, " ");
    strcat(result, object_to_draw->text);
    send(them, result, strlen(result) + 1, 0);
}

/* Alters the object obji by replacing the attributes with the one given. To alter */
/* less attributes at a time, simply pass the existing ones in too. */
/* If layer is different, re-arrange in list to maintain order by layer */
int edit_object(int client_id, Object object_to_edit, int new_x1, int new_y1, int new_x2, int new_y2, int new_layer, char *new_name) {
    if (object_to_edit->locked == client_id) {
        /* Different layer, so delete object an re-insert in correct position    */
        if (object_to_edit->layer != new_layer) {
            Object new_object = (Object) safe_malloc(sizeof(struct object));
            copy_objects(client_id, object_to_edit, new_object, "_temp");
            new_object->layer = new_layer;
            remove_object(client_id, object_to_edit->name);
            enter_object(new_object);
            object_to_edit = new_object;
        }
        strcpy(object_to_edit->name, new_name);
        object_to_edit->x1 = new_x1;
        object_to_edit->y1 = new_y1;
        object_to_edit->x2 = new_x2;
        object_to_edit->y2 = new_y2;
        strcpy(object_to_edit->name, new_name);
        return 1;
    } else {
        return 0;
    }
}
