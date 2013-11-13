#############################################################################
# Makefile for building: libcore.so
# Template: lib
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = 
CFLAGS        = -pipe -g -Wall -W -fPIC $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -fPIC $(DEFINES)
INCPATH       = -I. -I../include -I../include/syslib
LINK          = gcc
LFLAGS        = -shared
LIBS          = $(SUBLIBS)   -lpthread -ldl 
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

SOURCES       = lib/util.c \
		lib/sds.c \
		lib/safe_queue.c \
		lib/kfifo.c \
		lib/objSds.c \
		lib/dicttype.c \
		lib/dict.c \
		lib/adlist.c \
		net/http_server.c \
		net/client.c \
		net/anet.c \
		net/ae.c \
		http/request_handler.c \
		http/request.c \
		http/reply.c \
		syslib/unix_sockets.c \
		syslib/ugid_functions.c \
		syslib/tty_functions.c \
		syslib/signal_functions.c \
		syslib/signal.c \
		syslib/region_locking.c \
		syslib/read_line_buf.c \
		syslib/read_line.c \
		syslib/rdwrn.c \
		syslib/pty_master_open.c \
		syslib/pty_fork.c \
		syslib/print_wait_status.c \
		syslib/print_rusage.c \
		syslib/print_rlimit.c \
		syslib/itimerspec_from_str.c \
		syslib/inet_sockets.c \
		syslib/get_num.c \
		syslib/file_perms.c \
		syslib/event_flags.c \
		syslib/error_functions.c \
		syslib/curr_time.c \
		syslib/create_pid_file.c \
		syslib/binary_sems.c \
		syslib/become_daemon.c \
		syslib/alt_functions.c 
OBJECTS       = util.o \
		sds.o \
		safe_queue.o \
		kfifo.o \
		objSds.o \
		dicttype.o \
		dict.o \
		adlist.o \
		http_server.o \
		client.o \
		anet.o \
		ae.o \
		request_handler.o \
		request.o \
		reply.o \
		unix_sockets.o \
		ugid_functions.o \
		tty_functions.o \
		signal_functions.o \
		signal.o \
		region_locking.o \
		read_line_buf.o \
		read_line.o \
		rdwrn.o \
		pty_master_open.o \
		pty_fork.o \
		print_wait_status.o \
		print_rusage.o \
		print_rlimit.o \
		itimerspec_from_str.o \
		inet_sockets.o \
		get_num.o \
		file_perms.o \
		event_flags.o \
		error_functions.o \
		curr_time.o \
		create_pid_file.o \
		binary_sems.o \
		become_daemon.o \
		alt_functions.o
DESTDIR       = ../build/
TARGET        = libcore.so
TARGETD       = libcore.so

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

all:   ../build/$(TARGET)

../build/$(TARGET):  $(OBJECTS) $(SUBLIBS) $(OBJCOMP)  
	@$(CHK_DIR_EXISTS) ../build/ || $(MKDIR) ../build/ 
	-$(DEL_FILE) $(TARGET)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) $(OBJCOMP)
	-$(MOVE) $(TARGET) ../build/




dist: 
	@$(CHK_DIR_EXISTS) .tmp/core1.0.0 || $(MKDIR) .tmp/core1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/core1.0.0/ && (cd `dirname .tmp/core1.0.0` && $(TAR) core1.0.0.tar core1.0.0 && $(COMPRESS) core1.0.0.tar) && $(MOVE) `dirname .tmp/core1.0.0`/core1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/core1.0.0


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

util.o: lib/util.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o util.o lib/util.c

sds.o: lib/sds.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o sds.o lib/sds.c

safe_queue.o: lib/safe_queue.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o safe_queue.o lib/safe_queue.c

kfifo.o: lib/kfifo.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o kfifo.o lib/kfifo.c

objSds.o: lib/objSds.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o objSds.o lib/objSds.c

dicttype.o: lib/dicttype.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o dicttype.o lib/dicttype.c

dict.o: lib/dict.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o dict.o lib/dict.c

adlist.o: lib/adlist.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o adlist.o lib/adlist.c

http_server.o: net/http_server.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o http_server.o net/http_server.c

client.o: net/client.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o client.o net/client.c

anet.o: net/anet.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o anet.o net/anet.c

ae.o: net/ae.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o ae.o net/ae.c

request_handler.o: http/request_handler.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o request_handler.o http/request_handler.c

request.o: http/request.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o request.o http/request.c

reply.o: http/reply.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o reply.o http/reply.c

unix_sockets.o: syslib/unix_sockets.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o unix_sockets.o syslib/unix_sockets.c

ugid_functions.o: syslib/ugid_functions.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o ugid_functions.o syslib/ugid_functions.c

tty_functions.o: syslib/tty_functions.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o tty_functions.o syslib/tty_functions.c

signal_functions.o: syslib/signal_functions.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o signal_functions.o syslib/signal_functions.c

signal.o: syslib/signal.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o signal.o syslib/signal.c

region_locking.o: syslib/region_locking.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o region_locking.o syslib/region_locking.c

read_line_buf.o: syslib/read_line_buf.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o read_line_buf.o syslib/read_line_buf.c

read_line.o: syslib/read_line.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o read_line.o syslib/read_line.c

rdwrn.o: syslib/rdwrn.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o rdwrn.o syslib/rdwrn.c

pty_master_open.o: syslib/pty_master_open.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o pty_master_open.o syslib/pty_master_open.c

pty_fork.o: syslib/pty_fork.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o pty_fork.o syslib/pty_fork.c

print_wait_status.o: syslib/print_wait_status.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o print_wait_status.o syslib/print_wait_status.c

print_rusage.o: syslib/print_rusage.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o print_rusage.o syslib/print_rusage.c

print_rlimit.o: syslib/print_rlimit.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o print_rlimit.o syslib/print_rlimit.c

itimerspec_from_str.o: syslib/itimerspec_from_str.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o itimerspec_from_str.o syslib/itimerspec_from_str.c

inet_sockets.o: syslib/inet_sockets.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o inet_sockets.o syslib/inet_sockets.c

get_num.o: syslib/get_num.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o get_num.o syslib/get_num.c

file_perms.o: syslib/file_perms.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o file_perms.o syslib/file_perms.c

event_flags.o: syslib/event_flags.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o event_flags.o syslib/event_flags.c

error_functions.o: syslib/error_functions.c syslib/ename.c.inc
	$(CC) -c $(CFLAGS) $(INCPATH) -o error_functions.o syslib/error_functions.c

curr_time.o: syslib/curr_time.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o curr_time.o syslib/curr_time.c

create_pid_file.o: syslib/create_pid_file.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o create_pid_file.o syslib/create_pid_file.c

binary_sems.o: syslib/binary_sems.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o binary_sems.o syslib/binary_sems.c

become_daemon.o: syslib/become_daemon.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o become_daemon.o syslib/become_daemon.c

alt_functions.o: syslib/alt_functions.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o alt_functions.o syslib/alt_functions.c

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

