TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES = main.c \
    usage.c


TARGET = ../build/miniweb # move executable one dire up

HEADERS += \
    usage.h \
    signal_handler.h

INCLUDEPATH += ../include

LIBS += -ldl

