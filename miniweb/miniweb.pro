TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES = main.c \
    usage.c \
    net/*.c \
    http/request_handler.c


TARGET = ../build/miniweb # move the executable to the build folder

HEADERS += \
    usage.h \
    signal_handler.h \
    net/*.h \
    http/request_handler.h \
    miniweb_config.h

INCLUDEPATH += ../include ../lib

LIBS += -lpthread -ldl -L../build/lib -lsys -lds -lhttp

LibUV.commands = cd ../vendor/libuv && make && cp ./libuv.a ../../build/lib/
PRE_TARGETDEPS += LibUV
QMAKE_EXTRA_TARGETS += LibUV
