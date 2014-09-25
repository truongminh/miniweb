TEMPLATE = subdirs
CONFIG += ordered
CONFIG -= app_bundle
CONFIG -= qt
#sys = system
#ds = data structure

SUBDIRS = \
    sys \
    ds \
    http

http.depends += sys ds
