#############################################################################
# Makefile for building: ../build/miniweb
# Template: app
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = 
CFLAGS        = -pipe -g -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W $(DEFINES)
INCPATH       = -I. -I../include
LINK          = gcc
LFLAGS        = 
LIBS          = $(SUBLIBS)   -ldl -lpthread ../build/lib/libcore.so 
AR            = ar cqs
RANLIB        = 
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = 
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.c \
		usage.c \
		net/http_server.c \
		net/client.c \
		net/anet.c \
		net/ae.c \
		http/request_handler.c 
OBJECTS       = main.o \
		usage.o \
		http_server.o \
		client.o \
		anet.o \
		ae.o \
		request_handler.o
DESTDIR       = ../build/
TARGET        = ../build/miniweb

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all:  $(TARGET)

$(TARGET):  $(OBJECTS)  
	@$(CHK_DIR_EXISTS) ../build/ || $(MKDIR) ../build/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)


dist: 
	@$(CHK_DIR_EXISTS) .tmp/miniweb1.0.0 || $(MKDIR) .tmp/miniweb1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/miniweb1.0.0/ && (cd `dirname .tmp/miniweb1.0.0` && $(TAR) miniweb1.0.0.tar miniweb1.0.0 && $(COMPRESS) miniweb1.0.0.tar) && $(MOVE) `dirname .tmp/miniweb1.0.0`/miniweb1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/miniweb1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


compiler_rcc_make_all:
compiler_rcc_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_image_collection_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

main.o: main.c signal_handler.h \
		usage.h \
		net/http_server.h \
		net/ae.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o main.o main.c

usage.o: usage.c usage.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o usage.o usage.c

http_server.o: net/http_server.c net/anet.h \
		net/client.h \
		net/http_server.h \
		net/ae.h \
		http/request_handler.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o http_server.o net/http_server.c

client.o: net/client.c net/client.h \
		net/http_server.h \
		net/ae.h \
		net/anet.h \
		http/request_handler.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o client.o net/client.c

anet.o: net/anet.c net/anet.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o anet.o net/anet.c

ae.o: net/ae.c net/ae.h \
		net/client.h \
		net/http_server.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o ae.o net/ae.c

request_handler.o: http/request_handler.c http/request_handler.h \
		net/http_server.h \
		net/ae.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o request_handler.o http/request_handler.c

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

