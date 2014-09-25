TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += *.h
SOURCES += *.c

TARGET = ../../build/lib/http

INCLUDEPATH += ../

LIBS += -lpthread -ldl -L../../build/lib -lsys -lcore
