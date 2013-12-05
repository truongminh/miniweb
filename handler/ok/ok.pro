TEMPLATE = lib
CONFIG += plugin
CONFIG -= app_bundle
CONFIG -= qt

TARGET = ../../build/handler/ok

SOURCES += \
    ok.c

HEADERS += \
    ok.h

INCLUDEPATH += ../../include

LIBS += -lpthread -L../../build/lib/ -lcore


