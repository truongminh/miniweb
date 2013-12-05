TEMPLATE = lib
CONFIG += plugin
CONFIG -= app_bundle
CONFIG -= qt

TARGET = ../../build/handler/applog

SOURCES += \
    applog.c

HEADERS += \
    applog.h

INCLUDEPATH += ../../include

LIBS += -lpthread -L../../build/lib/ -lcore

