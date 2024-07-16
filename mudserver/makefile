###########################################
# Makefile
# Makefile for the server application
# lain 011erenshaw 14/10/97
# Version History:
# Date	Who Comments
# 14/10/97	IO	Created
###########################################
mudServ: object.o parse.o utils.o server.o unixserver.o file.o help, muderror.o main.o
	gcc -o mudServ main.o object.o parse.o utils.o unixserver.o server.o help.o muderror.o file.o -lm
	
main.o:	main.c object.h parse.h utils.h unixserver.h server.h muderror.h
	gcc -c main.c

muderror.o: muderror.c muderror.h
	gcc -c muderror.c

object.o: object.c object.h utils.h muderror.h
	gcc -c object.c

utils.o: utils.c utils.h
	gcc -c utils.c

server.o: server.c server.h
	gcc -c server.c

unixserver.o:	unixserver.c unixserver.h
	gcc -c unixserver.c
	
parse.o: parse.c parse.h help.h utils.h object.h muderror.h
	gcc -c parse.c

file.o:  object.h file.c file.h utils.h
	gcc -c file.c

help.o: help.c help.h
	gcc -c help.c
