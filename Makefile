#############################################################################
# Makefile for building: miniweb
# Generated by qmake (2.01a) (Qt 4.6.2) on: Thu Nov 7 13:11:30 2013
# Project:  miniweb.pro
# Template: subdirs
# Command: /usr/bin/qmake-qt4 -spec /usr/lib/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile miniweb.pro
#############################################################################

first: make_default
MAKEFILE      = Makefile
QMAKE         = /usr/bin/qmake-qt4
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
INSTALL_FILE  = install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   = $(COPY_DIR)
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
SUBTARGETS    =  \
		sub-corelib \
		sub-app \
		sub-applog \
		sub-status \
		sub-ok

corelib/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) corelib/ || $(MKDIR) corelib/ 
	cd corelib/ && $(QMAKE) /home/minhnt/Downloads/miniweb/corelib/corelib.pro -spec /usr/lib/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o $(MAKEFILE)
sub-corelib-qmake_all:  FORCE
	@$(CHK_DIR_EXISTS) corelib/ || $(MKDIR) corelib/ 
	cd corelib/ && $(QMAKE) /home/minhnt/Downloads/miniweb/corelib/corelib.pro -spec /usr/lib/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o $(MAKEFILE)
sub-corelib: corelib/$(MAKEFILE) FORCE
	cd corelib/ && $(MAKE) -f $(MAKEFILE)
sub-corelib-make_default-ordered: corelib/$(MAKEFILE) FORCE
	cd corelib/ && $(MAKE) -f $(MAKEFILE) 
sub-corelib-make_default: corelib/$(MAKEFILE) FORCE
	cd corelib/ && $(MAKE) -f $(MAKEFILE) 
sub-corelib-make_first-ordered: corelib/$(MAKEFILE) FORCE
	cd corelib/ && $(MAKE) -f $(MAKEFILE) first
sub-corelib-make_first: corelib/$(MAKEFILE) FORCE
	cd corelib/ && $(MAKE) -f $(MAKEFILE) first
sub-corelib-all-ordered: corelib/$(MAKEFILE) FORCE
	cd corelib/ && $(MAKE) -f $(MAKEFILE) all
sub-corelib-all: corelib/$(MAKEFILE) FORCE
	cd corelib/ && $(MAKE) -f $(MAKEFILE) all
sub-corelib-clean-ordered: corelib/$(MAKEFILE) FORCE
	cd corelib/ && $(MAKE) -f $(MAKEFILE) clean
sub-corelib-clean: corelib/$(MAKEFILE) FORCE
	cd corelib/ && $(MAKE) -f $(MAKEFILE) clean
sub-corelib-distclean-ordered: corelib/$(MAKEFILE) FORCE
	cd corelib/ && $(MAKE) -f $(MAKEFILE) distclean
sub-corelib-distclean: corelib/$(MAKEFILE) FORCE
	cd corelib/ && $(MAKE) -f $(MAKEFILE) distclean
sub-corelib-install_subtargets-ordered: corelib/$(MAKEFILE) FORCE
	cd corelib/ && $(MAKE) -f $(MAKEFILE) install
sub-corelib-install_subtargets: corelib/$(MAKEFILE) FORCE
	cd corelib/ && $(MAKE) -f $(MAKEFILE) install
sub-corelib-uninstall_subtargets-ordered: corelib/$(MAKEFILE) FORCE
	cd corelib/ && $(MAKE) -f $(MAKEFILE) uninstall
sub-corelib-uninstall_subtargets: corelib/$(MAKEFILE) FORCE
	cd corelib/ && $(MAKE) -f $(MAKEFILE) uninstall
app/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) app/ || $(MKDIR) app/ 
	cd app/ && $(QMAKE) /home/minhnt/Downloads/miniweb/app/app.pro -spec /usr/lib/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o $(MAKEFILE)
sub-app-qmake_all:  FORCE
	@$(CHK_DIR_EXISTS) app/ || $(MKDIR) app/ 
	cd app/ && $(QMAKE) /home/minhnt/Downloads/miniweb/app/app.pro -spec /usr/lib/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o $(MAKEFILE)
sub-app: app/$(MAKEFILE) sub-corelib FORCE
	cd app/ && $(MAKE) -f $(MAKEFILE)
sub-app-make_default-ordered: app/$(MAKEFILE) sub-corelib-make_default-ordered  FORCE
	cd app/ && $(MAKE) -f $(MAKEFILE) 
sub-app-make_default: app/$(MAKEFILE) sub-corelib-make_default FORCE
	cd app/ && $(MAKE) -f $(MAKEFILE) 
sub-app-make_first-ordered: app/$(MAKEFILE) sub-corelib-make_first-ordered  FORCE
	cd app/ && $(MAKE) -f $(MAKEFILE) first
sub-app-make_first: app/$(MAKEFILE) sub-corelib-make_first FORCE
	cd app/ && $(MAKE) -f $(MAKEFILE) first
sub-app-all-ordered: app/$(MAKEFILE) sub-corelib-all-ordered  FORCE
	cd app/ && $(MAKE) -f $(MAKEFILE) all
sub-app-all: app/$(MAKEFILE) sub-corelib-all FORCE
	cd app/ && $(MAKE) -f $(MAKEFILE) all
sub-app-clean-ordered: app/$(MAKEFILE) sub-corelib-clean-ordered  FORCE
	cd app/ && $(MAKE) -f $(MAKEFILE) clean
sub-app-clean: app/$(MAKEFILE) sub-corelib-clean FORCE
	cd app/ && $(MAKE) -f $(MAKEFILE) clean
sub-app-distclean-ordered: app/$(MAKEFILE) sub-corelib-distclean-ordered  FORCE
	cd app/ && $(MAKE) -f $(MAKEFILE) distclean
sub-app-distclean: app/$(MAKEFILE) sub-corelib-distclean FORCE
	cd app/ && $(MAKE) -f $(MAKEFILE) distclean
sub-app-install_subtargets-ordered: app/$(MAKEFILE) sub-corelib-install_subtargets-ordered  FORCE
	cd app/ && $(MAKE) -f $(MAKEFILE) install
sub-app-install_subtargets: app/$(MAKEFILE) sub-corelib-install_subtargets FORCE
	cd app/ && $(MAKE) -f $(MAKEFILE) install
sub-app-uninstall_subtargets-ordered: app/$(MAKEFILE) sub-corelib-uninstall_subtargets-ordered  FORCE
	cd app/ && $(MAKE) -f $(MAKEFILE) uninstall
sub-app-uninstall_subtargets: app/$(MAKEFILE) sub-corelib-uninstall_subtargets FORCE
	cd app/ && $(MAKE) -f $(MAKEFILE) uninstall
applog/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) applog/ || $(MKDIR) applog/ 
	cd applog/ && $(QMAKE) /home/minhnt/Downloads/miniweb/applog/applog.pro -spec /usr/lib/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o $(MAKEFILE)
sub-applog-qmake_all:  FORCE
	@$(CHK_DIR_EXISTS) applog/ || $(MKDIR) applog/ 
	cd applog/ && $(QMAKE) /home/minhnt/Downloads/miniweb/applog/applog.pro -spec /usr/lib/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o $(MAKEFILE)
sub-applog: applog/$(MAKEFILE) sub-corelib FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE)
sub-applog-make_default-ordered: applog/$(MAKEFILE) sub-app-make_default-ordered  FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) 
sub-applog-make_default: applog/$(MAKEFILE) sub-corelib-make_default FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) 
sub-applog-make_first-ordered: applog/$(MAKEFILE) sub-app-make_first-ordered  FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) first
sub-applog-make_first: applog/$(MAKEFILE) sub-corelib-make_first FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) first
sub-applog-all-ordered: applog/$(MAKEFILE) sub-app-all-ordered  FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) all
sub-applog-all: applog/$(MAKEFILE) sub-corelib-all FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) all
sub-applog-clean-ordered: applog/$(MAKEFILE) sub-app-clean-ordered  FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) clean
sub-applog-clean: applog/$(MAKEFILE) sub-corelib-clean FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) clean
sub-applog-distclean-ordered: applog/$(MAKEFILE) sub-app-distclean-ordered  FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) distclean
sub-applog-distclean: applog/$(MAKEFILE) sub-corelib-distclean FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) distclean
sub-applog-install_subtargets-ordered: applog/$(MAKEFILE) sub-app-install_subtargets-ordered  FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) install
sub-applog-install_subtargets: applog/$(MAKEFILE) sub-corelib-install_subtargets FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) install
sub-applog-uninstall_subtargets-ordered: applog/$(MAKEFILE) sub-app-uninstall_subtargets-ordered  FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) uninstall
sub-applog-uninstall_subtargets: applog/$(MAKEFILE) sub-corelib-uninstall_subtargets FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) uninstall
status/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) status/ || $(MKDIR) status/ 
	cd status/ && $(QMAKE) /home/minhnt/Downloads/miniweb/status/status.pro -spec /usr/lib/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o $(MAKEFILE)
sub-status-qmake_all:  FORCE
	@$(CHK_DIR_EXISTS) status/ || $(MKDIR) status/ 
	cd status/ && $(QMAKE) /home/minhnt/Downloads/miniweb/status/status.pro -spec /usr/lib/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o $(MAKEFILE)
sub-status: status/$(MAKEFILE) sub-corelib FORCE
	cd status/ && $(MAKE) -f $(MAKEFILE)
sub-status-make_default-ordered: status/$(MAKEFILE) sub-applog-make_default-ordered  FORCE
	cd status/ && $(MAKE) -f $(MAKEFILE) 
sub-status-make_default: status/$(MAKEFILE) sub-corelib-make_default FORCE
	cd status/ && $(MAKE) -f $(MAKEFILE) 
sub-status-make_first-ordered: status/$(MAKEFILE) sub-applog-make_first-ordered  FORCE
	cd status/ && $(MAKE) -f $(MAKEFILE) first
sub-status-make_first: status/$(MAKEFILE) sub-corelib-make_first FORCE
	cd status/ && $(MAKE) -f $(MAKEFILE) first
sub-status-all-ordered: status/$(MAKEFILE) sub-applog-all-ordered  FORCE
	cd status/ && $(MAKE) -f $(MAKEFILE) all
sub-status-all: status/$(MAKEFILE) sub-corelib-all FORCE
	cd status/ && $(MAKE) -f $(MAKEFILE) all
sub-status-clean-ordered: status/$(MAKEFILE) sub-applog-clean-ordered  FORCE
	cd status/ && $(MAKE) -f $(MAKEFILE) clean
sub-status-clean: status/$(MAKEFILE) sub-corelib-clean FORCE
	cd status/ && $(MAKE) -f $(MAKEFILE) clean
sub-status-distclean-ordered: status/$(MAKEFILE) sub-applog-distclean-ordered  FORCE
	cd status/ && $(MAKE) -f $(MAKEFILE) distclean
sub-status-distclean: status/$(MAKEFILE) sub-corelib-distclean FORCE
	cd status/ && $(MAKE) -f $(MAKEFILE) distclean
sub-status-install_subtargets-ordered: status/$(MAKEFILE) sub-applog-install_subtargets-ordered  FORCE
	cd status/ && $(MAKE) -f $(MAKEFILE) install
sub-status-install_subtargets: status/$(MAKEFILE) sub-corelib-install_subtargets FORCE
	cd status/ && $(MAKE) -f $(MAKEFILE) install
sub-status-uninstall_subtargets-ordered: status/$(MAKEFILE) sub-applog-uninstall_subtargets-ordered  FORCE
	cd status/ && $(MAKE) -f $(MAKEFILE) uninstall
sub-status-uninstall_subtargets: status/$(MAKEFILE) sub-corelib-uninstall_subtargets FORCE
	cd status/ && $(MAKE) -f $(MAKEFILE) uninstall
ok/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) ok/ || $(MKDIR) ok/ 
	cd ok/ && $(QMAKE) /home/minhnt/Downloads/miniweb/ok/ok.pro -spec /usr/lib/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o $(MAKEFILE)
sub-ok-qmake_all:  FORCE
	@$(CHK_DIR_EXISTS) ok/ || $(MKDIR) ok/ 
	cd ok/ && $(QMAKE) /home/minhnt/Downloads/miniweb/ok/ok.pro -spec /usr/lib/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o $(MAKEFILE)
sub-ok: ok/$(MAKEFILE) sub-corelib FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE)
sub-ok-make_default-ordered: ok/$(MAKEFILE) sub-status-make_default-ordered  FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) 
sub-ok-make_default: ok/$(MAKEFILE) sub-corelib-make_default FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) 
sub-ok-make_first-ordered: ok/$(MAKEFILE) sub-status-make_first-ordered  FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) first
sub-ok-make_first: ok/$(MAKEFILE) sub-corelib-make_first FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) first
sub-ok-all-ordered: ok/$(MAKEFILE) sub-status-all-ordered  FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) all
sub-ok-all: ok/$(MAKEFILE) sub-corelib-all FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) all
sub-ok-clean-ordered: ok/$(MAKEFILE) sub-status-clean-ordered  FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) clean
sub-ok-clean: ok/$(MAKEFILE) sub-corelib-clean FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) clean
sub-ok-distclean-ordered: ok/$(MAKEFILE) sub-status-distclean-ordered  FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) distclean
sub-ok-distclean: ok/$(MAKEFILE) sub-corelib-distclean FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) distclean
sub-ok-install_subtargets-ordered: ok/$(MAKEFILE) sub-status-install_subtargets-ordered  FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) install
sub-ok-install_subtargets: ok/$(MAKEFILE) sub-corelib-install_subtargets FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) install
sub-ok-uninstall_subtargets-ordered: ok/$(MAKEFILE) sub-status-uninstall_subtargets-ordered  FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) uninstall
sub-ok-uninstall_subtargets: ok/$(MAKEFILE) sub-corelib-uninstall_subtargets FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) uninstall

Makefile: miniweb.pro  /usr/lib/qt4/mkspecs/linux-g++/qmake.conf /usr/lib/qt4/mkspecs/common/g++-multilib.conf \
		/usr/lib/qt4/mkspecs/common/unix.conf \
		/usr/lib/qt4/mkspecs/common/linux.conf \
		/usr/lib/qt4/mkspecs/qconfig.pri \
		/usr/lib/qt4/mkspecs/features/qt_functions.prf \
		/usr/lib/qt4/mkspecs/features/qt_config.prf \
		/usr/lib/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/lib/qt4/mkspecs/features/default_pre.prf \
		/usr/lib/qt4/mkspecs/features/debug.prf \
		/usr/lib/qt4/mkspecs/features/default_post.prf \
		/usr/lib/qt4/mkspecs/features/warn_on.prf \
		/usr/lib/qt4/mkspecs/features/resources.prf \
		/usr/lib/qt4/mkspecs/features/uic.prf \
		/usr/lib/qt4/mkspecs/features/yacc.prf \
		/usr/lib/qt4/mkspecs/features/lex.prf \
		/usr/lib/qt4/mkspecs/features/include_source_dir.prf
	$(QMAKE) -spec /usr/lib/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile miniweb.pro
/usr/lib/qt4/mkspecs/common/g++-multilib.conf:
/usr/lib/qt4/mkspecs/common/unix.conf:
/usr/lib/qt4/mkspecs/common/linux.conf:
/usr/lib/qt4/mkspecs/qconfig.pri:
/usr/lib/qt4/mkspecs/features/qt_functions.prf:
/usr/lib/qt4/mkspecs/features/qt_config.prf:
/usr/lib/qt4/mkspecs/features/exclusive_builds.prf:
/usr/lib/qt4/mkspecs/features/default_pre.prf:
/usr/lib/qt4/mkspecs/features/debug.prf:
/usr/lib/qt4/mkspecs/features/default_post.prf:
/usr/lib/qt4/mkspecs/features/warn_on.prf:
/usr/lib/qt4/mkspecs/features/resources.prf:
/usr/lib/qt4/mkspecs/features/uic.prf:
/usr/lib/qt4/mkspecs/features/yacc.prf:
/usr/lib/qt4/mkspecs/features/lex.prf:
/usr/lib/qt4/mkspecs/features/include_source_dir.prf:
qmake: qmake_all FORCE
	@$(QMAKE) -spec /usr/lib/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile miniweb.pro

qmake_all: sub-corelib-qmake_all sub-app-qmake_all sub-applog-qmake_all sub-status-qmake_all sub-ok-qmake_all FORCE

make_default: sub-corelib-make_default-ordered sub-app-make_default-ordered sub-applog-make_default-ordered sub-status-make_default-ordered sub-ok-make_default-ordered FORCE
make_first: sub-corelib-make_first-ordered sub-app-make_first-ordered sub-applog-make_first-ordered sub-status-make_first-ordered sub-ok-make_first-ordered FORCE
all: sub-corelib-all-ordered sub-app-all-ordered sub-applog-all-ordered sub-status-all-ordered sub-ok-all-ordered FORCE
clean: sub-corelib-clean-ordered sub-app-clean-ordered sub-applog-clean-ordered sub-status-clean-ordered sub-ok-clean-ordered FORCE
distclean: sub-corelib-distclean-ordered sub-app-distclean-ordered sub-applog-distclean-ordered sub-status-distclean-ordered sub-ok-distclean-ordered FORCE
	-$(DEL_FILE) Makefile
install_subtargets: sub-corelib-install_subtargets-ordered sub-app-install_subtargets-ordered sub-applog-install_subtargets-ordered sub-status-install_subtargets-ordered sub-ok-install_subtargets-ordered FORCE
uninstall_subtargets: sub-corelib-uninstall_subtargets-ordered sub-app-uninstall_subtargets-ordered sub-applog-uninstall_subtargets-ordered sub-status-uninstall_subtargets-ordered sub-ok-uninstall_subtargets-ordered FORCE
install: install_subtargets  FORCE

uninstall:  uninstall_subtargets FORCE

FORCE:

