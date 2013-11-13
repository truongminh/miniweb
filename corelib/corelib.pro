TEMPLATE = lib
CONFIG += console
CONFIG += plugin

CONFIG -= app_bundle
CONFIG -= qt

#CONFIG += staticlib

TARGET = ../build/core

SOURCES += \
    lib/util.c \
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

INCLUDEPATH += ../include ../include/syslib
LIBS += -lpthread -ldl

