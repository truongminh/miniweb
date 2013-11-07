TEMPLATE = subdirs
CONFIG += console
CONFIG += ordered
CONFIG -= app_bundle
CONFIG -= qt

SUBDIRS = corelib \
          app \
          applog \
          status \
          ok

app.depends = corelib
applog.depends = corelib
status.depends = corelib
ok.depends = corelib

OTHER_FILES += README \
    qmake_to_Makefile.sh \
    distribute.sh


