#!/bin/sh
find . -name Makefile -type f | xargs sed -i 's/all: Makefile/all: /'
find . -name Makefile -type f | xargs sed -i 's/-I\/usr\/lib\/qt4\/mkspecs\/linux-g++ //'
find . -name Makefile -type f | xargs sed -i 's/LINK          = g++/LINK          = gcc/'
find . -name Makefile -type f | xargs sed -i '/qt4\/mkspecs\/features/d'
find . -name Makefile -type f | xargs sed -i '/qt4\/mkspecs\/common/d'
find . -name Makefile -type f | xargs sed -i '/qt4\/mkspecs\/qconfig.pri/d'
find . -name Makefile -type f | xargs sed -i '/QMAKE/d'
find . -name Makefile -type f | xargs sed -i '/qmake/d'
find . -name Makefile -type f | xargs sed -i '/.pro/d'




