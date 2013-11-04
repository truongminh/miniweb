TEMPLATE = lib
CONFIG += plugin
CONFIG -= app_bundle
CONFIG -= qt

TARGET = ../build/mnwlib/applog

SOURCES += \
    applog.c

HEADERS += \
    applog.h

INCLUDEPATH += ../include

LIBS += -lpthread ../build/libcore.so

