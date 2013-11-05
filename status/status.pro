TEMPLATE = lib
CONFIG += plugin
CONFIG -= app_bundle
CONFIG -= qt

TARGET = ../build/mnwlib/status

SOURCES += \
    status.c

HEADERS += \
    status.h

INCLUDEPATH += ../include

LIBS += -lpthread ../build/libcore.so

