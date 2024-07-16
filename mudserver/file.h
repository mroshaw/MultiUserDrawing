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

void save_scene(char *fName);

void load_scene(char *fName);

void clear_scene();

/* void source (Scene theScene, char *fName); */
#endif // FILE_H
