TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES = main.c \
    usage.c \
    net/http_server.c \
    net/client.c \
    net/anet.c \
    net/ae.c \
    http/request_handler.c


TARGET = ../build/miniweb # move executable one dire up

HEADERS += \
    usage.h \
    signal_handler.h \
    net/http_server.h \
    net/client.h \
    net/anet.h \
    net/ae.h \
    http/request_handler.h

INCLUDEPATH += ../include

LIBS += -ldl
LIBS += -lpthread ../build/lib/libcore.so

