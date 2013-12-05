TEMPLATE = subdirs
CONFIG += console
CONFIG += ordered
CONFIG -= app_bundle
CONFIG -= qt

SUBDIRS = lib \
          miniweb \
          handler


miniweb.depends = lib
handler.depends = lib

OTHER_FILES += README \
    qmake_to_Makefile.sh \
    distribute.sh


