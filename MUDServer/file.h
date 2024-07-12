/* file.h                                           */
/* Routines for loading, saving a clearing data     */
/* in server                                        */
/*                                                  */
/* Iain Ollerenshaw 13/10/97                        */
/*                                                  */
/* Version History:                                 */
/* Date     Who     Comments                        */
/* 13/10/97 IO      Created                         */
/*                                                  */
#ifndef FILE_H
#define FILE_H

void save(char *fName);

void load(char *fName);

void clear();

/* void source (Scene theScene, char *fName); */
#endif // FILE_H
