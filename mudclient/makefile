###########################################
# Makefile for the client application
# lain 011erenshaw 14/10/97
# Version Histroy:
# Date	Who Comments
# 14/10/97	IO	Created
###########################################
mudClient: client.o tclInit.o utils.o
	gcc -o mudClient.exe -IC:/ActiveTcl86/include -LC:/ActiveTcl86/bin client.o tclInit.o utils.o -ltcl86t -lm -ltk86t

client.o: unixclient.c unixclient.h tclInit.h ../common/utils.h
	gcc -c -IC:/ActiveTcl86/include -LC:/ActiveTcl86/bin client.c -ltcl86t -lm -ltk86t
		
tclInit.o: tclInit.c tclInit.h
	gcc -c -IC:/ActiveTcl86/include -LC:/ActiveTcl86/bin tclInit.c -ltcl86t -lm -ltk86t
		
utils.o: ../common/utils.c ../common/utils.h
	gcc -c utils.c
