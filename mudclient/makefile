###########################################
# Makefile for the client application
# Iain Ollerenshaw 14/10/97
###########################################

COMMON_INC  = ..

# Cygwin (Unix/X11) build tools and paths
CC_UNIX     = gcc
TCL_INC     = /usr/include
TCL_LIB     = /usr/lib
TCL_FLAGS   = -ltcl8.6 -ltk8.6

# MinGW (Native Windows) build tools and paths
CC_WIN      = x86_64-w64-mingw32-gcc
WIN_TCL_INC = /usr/x86_64-w64-mingw32/sys-root/mingw/include
WIN_TCL_LIB = /usr/x86_64-w64-mingw32/sys-root/mingw/lib
WIN_FLAGS   = -ltcl86 -ltk86 -lws2_32 -mconsole 
WIN_INC     = -nostdinc \
              -I$(COMMON_INC) \
              -isystem $(WIN_TCL_INC) \
              -isystem $(WIN_TCL_INC)/tcl8.6 \
              -isystem /usr/x86_64-w64-mingw32/sys-root/mingw/sys-include \
              -isystem /usr/lib/gcc/x86_64-w64-mingw32/13/include

# Output directories
UNIX_OBJDIR = obj/unix
WIN_OBJDIR  = obj/win
BINDIR      = bin

$(shell mkdir -p $(UNIX_OBJDIR) $(WIN_OBJDIR) $(BINDIR))

# Default builds both
all: unix windows

# ─── Unix/Cygwin build ────────────────────────────────────────────
unix: $(BINDIR)/mudClient

$(BINDIR)/mudClient: $(UNIX_OBJDIR)/main.o $(UNIX_OBJDIR)/unixclient.o $(UNIX_OBJDIR)/tclInit.o $(UNIX_OBJDIR)/utils.o
	$(CC_UNIX) -o $(BINDIR)/mudClient \
		$(UNIX_OBJDIR)/main.o \
		$(UNIX_OBJDIR)/unixclient.o \
		$(UNIX_OBJDIR)/tclInit.o \
		$(UNIX_OBJDIR)/utils.o \
		-L$(TCL_LIB) $(TCL_FLAGS) -lm -mconsole

$(UNIX_OBJDIR)/main.o: main.c main.h ../common/platform.h
	$(CC_UNIX) -c -I$(COMMON_INC) -I$(TCL_INC) -o $(UNIX_OBJDIR)/main.o main.c

$(UNIX_OBJDIR)/unixclient.o: unixclient.c unixclient.h ../common/utils.h
	$(CC_UNIX) -c -I$(COMMON_INC) -I$(TCL_INC) -o $(UNIX_OBJDIR)/unixclient.o unixclient.c

$(UNIX_OBJDIR)/tclInit.o: tclInit.c tclInit.h ../common/platform.h
	$(CC_UNIX) -c -I$(COMMON_INC) -I$(TCL_INC) -o $(UNIX_OBJDIR)/tclInit.o tclInit.c

$(UNIX_OBJDIR)/utils.o: ../common/utils.c ../common/utils.h
	$(CC_UNIX) -c -o $(UNIX_OBJDIR)/utils.o ../common/utils.c

# ─── Native Windows build ─────────────────────────────────────────
windows: $(BINDIR)/mudClientWin.exe

$(BINDIR)/mudClientWin.exe: $(WIN_OBJDIR)/main.o $(WIN_OBJDIR)/winclient.o $(WIN_OBJDIR)/tclInit.o $(WIN_OBJDIR)/utils.o
	$(CC_WIN) -o $(BINDIR)/mudClientWin.exe \
		$(WIN_OBJDIR)/main.o \
		$(WIN_OBJDIR)/winclient.o \
		$(WIN_OBJDIR)/tclInit.o \
		$(WIN_OBJDIR)/utils.o \
		-L$(WIN_TCL_LIB) $(WIN_FLAGS) -lm

$(WIN_OBJDIR)/main.o: main.c main.h ../common/platform.h
	$(CC_WIN) -c $(WIN_INC) -o $(WIN_OBJDIR)/main.o main.c

$(WIN_OBJDIR)/winclient.o: winclient.c winclient.h ../common/utils.h
	$(CC_WIN) -c $(WIN_INC) -o $(WIN_OBJDIR)/winclient.o winclient.c

$(WIN_OBJDIR)/tclInit.o: tclInit.c tclInit.h ../common/platform.h
	$(CC_WIN) -c $(WIN_INC) -o $(WIN_OBJDIR)/tclInit.o tclInit.c

$(WIN_OBJDIR)/utils.o: ../common/utils.c ../common/utils.h
	$(CC_WIN) -c $(WIN_INC) -o $(WIN_OBJDIR)/utils.o ../common/utils.c

# ─── Clean ────────────────────────────────────────────────────────
.PHONY: all unix windows clean
clean:
	rm -f $(UNIX_OBJDIR)/*.o $(WIN_OBJDIR)/*.o
	rm -f $(BINDIR)/mudClient $(BINDIR)/mudClientWin.exe

deploy: windows
	cp /usr/x86_64-w64-mingw32/sys-root/mingw/bin/tcl86.dll bin/tcl86.dll
	cp /usr/x86_64-w64-mingw32/sys-root/mingw/bin/tk86.dll bin/tk86.dll
	cp -r /usr/x86_64-w64-mingw32/sys-root/mingw/lib/tcl8.6 $(BINDIR)/
	cp -r /usr/x86_64-w64-mingw32/sys-root/mingw/lib/tk8.6 $(BINDIR)/
	rm -rf $(BINDIR)/Tcl
	cp -r Tcl $(BINDIR)/