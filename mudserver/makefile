###########################################
# Makefile
# Makefile for the server application
# lain 011erenshaw 14/10/97
# Version History:
# Date	Who Comments
# 14/10/97	IO	Created
###########################################

COMMON_INC = ..
OBJDIR = obj
BINDIR = bin

# Ensure output directories exist
$(shell mkdir -p $(OBJDIR) $(BINDIR))

$(BINDIR)/mudServ: $(OBJDIR)/main.o $(OBJDIR)/object.o $(OBJDIR)/parse.o $(OBJDIR)/utils.o $(OBJDIR)/server.o $(OBJDIR)/unixserver.o $(OBJDIR)/file.o $(OBJDIR)/help.o $(OBJDIR)/muderror.o $(OBJDIR)/serverconfig.o
	gcc -o $(BINDIR)/mudServ $(OBJDIR)/main.o $(OBJDIR)/object.o $(OBJDIR)/parse.o $(OBJDIR)/utils.o $(OBJDIR)/unixserver.o $(OBJDIR)/server.o $(OBJDIR)/help.o $(OBJDIR)/muderror.o $(OBJDIR)/file.o $(OBJDIR)/serverconfig.o -lm

$(OBJDIR)/main.o: main.c object.h parse.h ../common/utils.h unixserver.h server.h muderror.h
	gcc -c -I$(COMMON_INC) -o $(OBJDIR)/main.o main.c

$(OBJDIR)/serverconfig.o: serverconfig.c serverconfig.h
	gcc -c -o $(OBJDIR)/serverconfig.o serverconfig.c

$(OBJDIR)/muderror.o: muderror.c muderror.h
	gcc -c -o $(OBJDIR)/muderror.o muderror.c

$(OBJDIR)/object.o: object.c object.h ../common/utils.h muderror.h
	gcc -c -I$(COMMON_INC) -o $(OBJDIR)/object.o object.c

$(OBJDIR)/utils.o: ../common/utils.c ../common/utils.h
	gcc -c -o $(OBJDIR)/utils.o ../common/utils.c

$(OBJDIR)/server.o: server.c server.h
	gcc -c -o $(OBJDIR)/server.o server.c

$(OBJDIR)/unixserver.o: unixserver.c unixserver.h
	gcc -c -o $(OBJDIR)/unixserver.o unixserver.c

$(OBJDIR)/parse.o: parse.c parse.h help.h ../common/utils.h object.h muderror.h
	gcc -c -I$(COMMON_INC) -o $(OBJDIR)/parse.o parse.c

$(OBJDIR)/file.o: file.c file.h object.h ../common/utils.h
	gcc -c -I$(COMMON_INC) -o $(OBJDIR)/file.o file.c

$(OBJDIR)/help.o: help.c help.h
	gcc -c -o $(OBJDIR)/help.o help.c

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/mudServ