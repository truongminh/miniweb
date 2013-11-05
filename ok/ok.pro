TEMPLATE = lib
CONFIG += plugin
CONFIG -= app_bundle
CONFIG -= qt

TARGET = ../build/mnwlib/ok

SOURCES += \
    ok.c

HEADERS += \
    ok.h

INCLUDEPATH += ../include

LIBS += -lpthread ../build/libcore.so

