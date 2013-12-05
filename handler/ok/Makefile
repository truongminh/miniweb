#############################################################################
# Makefile for building: libok.so
# Template: lib
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = 
CFLAGS        = -pipe -g -Wall -W -fPIC $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -fPIC $(DEFINES)
INCPATH       = -I. -I../../include
LINK          = gcc
LFLAGS        = -shared
LIBS          = $(SUBLIBS)   -lpthread -L../../build/lib/ -lcore 
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

SOURCES       = ok.c 
OBJECTS       = ok.o
DESTDIR       = ../../build/handler/
TARGET        = libok.so
TARGETD       = libok.so

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

all:   ../../build/handler/$(TARGET)

../../build/handler/$(TARGET):  $(OBJECTS) $(SUBLIBS) $(OBJCOMP)  
	@$(CHK_DIR_EXISTS) ../../build/handler/ || $(MKDIR) ../../build/handler/ 
	-$(DEL_FILE) $(TARGET)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) $(OBJCOMP)
	-$(MOVE) $(TARGET) ../../build/handler/




dist: 
	@$(CHK_DIR_EXISTS) .tmp/ok1.0.0 || $(MKDIR) .tmp/ok1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/ok1.0.0/ && (cd `dirname .tmp/ok1.0.0` && $(TAR) ok1.0.0.tar ok1.0.0 && $(COMPRESS) ok1.0.0.tar) && $(MOVE) `dirname .tmp/ok1.0.0`/ok1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/ok1.0.0


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

ok.o: ok.c ok.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o ok.o ok.c

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

