#!/bin/sh
rm miniweb.zip
zip -r miniweb.zip . -i *.[hc]
zip -r miniweb.zip . -i Makefile
zip -r miniweb.zip . -i */Makefile
zip -r miniweb.zip . -i *.sh
zip -r miniweb.zip . -i */*.sh
zip miniweb.zip README

