#!/bin/sh
find . -name Makefile -type f | xargs sed -i 's/all: Makefile/all: /'
find . -name Makefile -type f | xargs sed -i 's/LINK          = g++/LINK          = gcc/'
