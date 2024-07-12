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
Object createObject(int clientId, int type, char name[20], int x1, int y1, int x2, int y2, char theText[20]);

void dls_store(Object newObj, Object *start, Object *last);

int enterObject(Object theObject);

int removeObject(int clientId, char findName[]);

Object findObject(char *name);

void showObjects();

void drawAllObjects(char *resultString);

void drawByNum(int objectNum, char *resultString);

void copyObjects(int clientId, Object obj1, Object obj2, char *newName);

void printObject(Object theObject);

int lockObject(int clientId, Object theObject);

int unlockObject(int clientId, Object theObject);

void drawObject(Object theObject, char *result);

int editObject(int clientId, Object obj1, int newX1, int newYl, int newX2, int newY2, int newLayer, char *newName);
#endif // OBJECT_H
