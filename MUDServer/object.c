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
#include "utils.h"
#include "server.h"
#include "scene.h"
extern Scene theScene;
extern int them;

/* Finds an object in a scene. Returns NULL if not found.   */
Object findObject(char *name) {
    Object tempObject = (Object) safemalloc(sizeof(struct object));
    tempObject = theScene->start;

    while (tempObject != NULL) {
        if (!strcmp(name, tempObject->name)) {
            /* Found object */
            return tempObject;
        }
        tempObject = tempObject->next;
    }
    /* Object not found */
    return NULL;
}

/* Creates and returns a new object. Returns NULL if unsuccessful */
Object createObject(int clientId, int type, char name[20], int x1, int y1, int x2, int y2, char theText[20]) {
    Object newObject = (Object) safemalloc(sizeof(struct object));
    if (newObject != NULL) {
        newObject->type = type;
        strcpy(newObject->name, name);
        newObject->x1 = x1;
        newObject->y1 = y1;
        newObject->x2 = x2;
        newObject->y2 = y2;
        newObject->layer = 0; /* Initially front most */
        newObject->locked = clientId; /* Initially owned by creating user? */
        strcpy(newObject->text, theText);
        return newObject;
    } else {
        return NULL; /* Unsuccessfull, out of memory?    */
    }
}

/* Create doubly linked list */
void dls_store(Object newObj, Object *start, Object *last) {
    Object old, p;
    if (*last == NULL) {
        newObj->next = NULL;
        newObj->previous = NULL;
        *last = newObj;
        *start = newObj;
        return;
    }

    p = *start; /* Start at top of list */
    old = NULL;
    while (p) {
        if (p->layer < newObj->layer) {
            old = p;
            p = p->next;
        } else {
            if (p->previous) {
                p->previous->next = newObj;
                newObj->next = p;
                newObj->previous = p->previous;
                p->previous = newObj;
                return;
            }
            newObj->next = p;
            newObj->previous = NULL;
            p->previous = newObj;
            *start = newObj;
            return;
        }
    }
    old->next = newObj;
    newObj->next = NULL;
    newObj->previous = old;
    *last = newObj;
}

/* Enter an object into the scene   */
/* Uses doubly linked list interface defined earlier */
int enterObject(Object theObject) {
    Object temp = findObject(theObject->name);
    if (temp == NULL) {
        dls_store(theObject, &theScene->start, &theScene->last);
        return 1;
    } else
        return 0;
}

/* Remove an object from the specified scene    */
/* Client id must own object    */
int removeObject(int clientId, char findName[]) {
    Object info = (Object) safemalloc(sizeof(struct object));
    Object findObject();
    Object *start = &theScene->start;
    Object *last = &theScene->last;

    info = findObject(findName);

    /* User must own object!    */
    if (info && info->locked == clientId) {
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
    } else if (info->locked != clientId) {
        /* Don't own object!    */
        return 0;
    } else {
        /* Unknown error. PANIC!    */
        return 0;
    }
}

/* Private debug procedure  */
void showObjects() {
    Object info = theScene->start;
    while (info) {
        printObject(info);
        info = info->next;
    }
    fprintf(stdout, "\n\n");
}

/* Public procedure for getting ALL object information */
/* works by sending the client the number of objects */
/* then getting the client to collect objects by the */
/* drawByNum procedure  */
void drawAllObjects(char *resultString) {
    int count = 0;
    char temp[20];
    Object info = theScene->start;
    while (info) {
        count++;
        info = info->next;
    }
    my_itoa(count, temp);
    strcpy(resultString, temp);
}

/* Draw an object by its place in the list  */
void drawByNum(int objectNum, char *resultString) {
    int count = 0;
    Object info = theScene->start;
    while (count < objectNum) {
        count++;
        info = info->next;
    }
    drawObject(info, resultString);
}

/* Duplicate two objects. newName must be unique. Attributes of obj1 */
/* will be copied into existing obj2. */
/* Will append clients id to new object making it theirs */
void copyObjects(int clientId, Object obj1, Object obj2, char *newName) {
    strcpy(obj2->name, newName);
    obj2->x1 = obj1->x1;
    obj2->y1 = obj1->y1;
    obj2->locked = clientId;
    obj2->type = obj1->type;
    obj2->x2 = obj1->x2;
    obj2->y2 = obj1->y2;
    obj2->layer = 0;
}

/* Private debug procedure  */
void printObject(Object theObject) {
    if (theObject == NULL) {
        sendError("PRINTOBJECT: Object is NULL\n");
    } else {
        fprintf(stdout, "\n");
        fprintf(stdout, "Object type : %i\n", theObject->type);
        fprintf(stdout, "Object name : %s\n", theObject->name);
        fprintf(stdout, "X1 : %i\n", theObject->x1);
        fprintf(stdout, "Y1 : %i\n", theObject->y1);
        fprintf(stdout, "X2 : %i\n", theObject->x2);
        fprintf(stdout, "Y2 : %i\n", theObject->y2);
        fprintf(stdout, "Layer  : %i\n", theObject->layer);
        fprintf(stdout, "Text (if text) : %s\n", theObject->text);
        fprintf(stdout, "Locked by: %i\n", theObject->locked);
    }
}

/* Lock an object with a user id    */
int lockObject(int clientld, Object theObject) {
    if (theObject->locked == clientld) {
        /* Owns object, but lock is successful! */
        ;
        return 1;
    } else if (theObject->locked != 0) {
        /* Object belongs to someone else   */
        return 0;
    } else {
        /* Okay. Change object lock */
        theObject->locked = clientld;
        return 1;
    }
}

/* Unlock an object */
int unlockObject(int clientld, Object theObject) {
    if (theObject->locked == clientld) {
        /* Client owns object and so can unlock it  */
        theObject->locked = 0;
        return 1;
    } else {
        /* Client does not own object!! */
        return 0;
    }
    /* drawObject(theObject);    */
}

/* Construct a string to send to the client */
void drawObject(Object theObject, char *result) {
    /* Create the server string    */
    /* String is of the form:    */
    /* name, type, x1, y1, width, height, layer, locked */
    /* Client can use this information for manipulation purposes */
    char temp[20] = "";
    strcpy(result, theObject->name);
    strcat(result, " ");
    my_itoa(theObject->type, temp);
    strcat(result, temp);
    strcat(result, " ");
    my_itoa(theObject->x1, temp);
    strcat(result, temp);
    strcat(result, " ");
    my_itoa(theObject->y1, temp);
    strcat(result, temp);
    strcat(result, " ");
    my_itoa(theObject->x2, temp);
    strcat(result, temp);
    strcat(result, " ");
    my_itoa(theObject->y2, temp);
    strcat(result, temp);
    strcat(result, " ");
    my_itoa(theObject->layer, temp);
    strcat(result, temp);
    strcat(result, " ");
    my_itoa(theObject->locked, temp);
    strcat(result, temp);
    strcat(result, " ");
    strcat(result, theObject->text);
    send(them, result, strlen(result) + 1, 0);
}

/* Alters the object obji by replacing the attributes with the one given. To alter */
/* less attributes at a time, simply pass the existing ones in too. */
/* If layer is different, re-arrange in list to maintain order by layer */
int editObject(int clientId, Object obj1, int newX1, int newY1, int newX2, int newY2, int newLayer, char *newName) {
    if (obj1->locked == clientId) {
        /* Different layer, so delete object an re-insert in correct position    */
        if (obj1->layer != newLayer) {
            Object newObj = (Object) safemalloc(sizeof(struct object));
            copyObjects(clientId, obj1, newObj, "_temp");
            newObj->layer = newLayer;
            removeObject(clientId, obj1->name);
            enterObject(newObj);
            obj1 = newObj;
        }
        strcpy(obj1->name, newName);
        obj1->x1 = newX1;
        obj1->y1 = newY1;
        obj1->x2 = newX2;
        obj1->y2 = newY2;
        strcpy(obj1->name, newName);
        return 1;
    } else {
        return 0;
    }
}
