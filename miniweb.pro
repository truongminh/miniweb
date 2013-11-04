TEMPLATE = subdirs
CONFIG += console
CONFIG += ordered
CONFIG -= app_bundle
CONFIG -= qt

SUBDIRS = corelib \
          app \
          applog

app.depends = corelib
applog.depends = corelib

OTHER_FILES += README
