#############################################################################
# Makefile for building: miniweb
# Template: subdirs
#############################################################################

first: make_default
MAKEFILE      = Makefile
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
		sub-lib \
		sub-miniweb \
		sub-handler

lib/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) lib/ || $(MKDIR) lib/ 
	@$(CHK_DIR_EXISTS) lib/ || $(MKDIR) lib/ 
sub-lib: lib/$(MAKEFILE) FORCE
	cd lib/ && $(MAKE) -f $(MAKEFILE)
sub-lib-make_default-ordered: lib/$(MAKEFILE) FORCE
	cd lib/ && $(MAKE) -f $(MAKEFILE) 
sub-lib-make_default: lib/$(MAKEFILE) FORCE
	cd lib/ && $(MAKE) -f $(MAKEFILE) 
sub-lib-make_first-ordered: lib/$(MAKEFILE) FORCE
	cd lib/ && $(MAKE) -f $(MAKEFILE) first
sub-lib-make_first: lib/$(MAKEFILE) FORCE
	cd lib/ && $(MAKE) -f $(MAKEFILE) first
sub-lib-all-ordered: lib/$(MAKEFILE) FORCE
	cd lib/ && $(MAKE) -f $(MAKEFILE) all
sub-lib-all: lib/$(MAKEFILE) FORCE
	cd lib/ && $(MAKE) -f $(MAKEFILE) all
sub-lib-clean-ordered: lib/$(MAKEFILE) FORCE
	cd lib/ && $(MAKE) -f $(MAKEFILE) clean
sub-lib-clean: lib/$(MAKEFILE) FORCE
	cd lib/ && $(MAKE) -f $(MAKEFILE) clean
sub-lib-distclean-ordered: lib/$(MAKEFILE) FORCE
	cd lib/ && $(MAKE) -f $(MAKEFILE) distclean
sub-lib-distclean: lib/$(MAKEFILE) FORCE
	cd lib/ && $(MAKE) -f $(MAKEFILE) distclean
sub-lib-install_subtargets-ordered: lib/$(MAKEFILE) FORCE
	cd lib/ && $(MAKE) -f $(MAKEFILE) install
sub-lib-install_subtargets: lib/$(MAKEFILE) FORCE
	cd lib/ && $(MAKE) -f $(MAKEFILE) install
sub-lib-uninstall_subtargets-ordered: lib/$(MAKEFILE) FORCE
	cd lib/ && $(MAKE) -f $(MAKEFILE) uninstall
sub-lib-uninstall_subtargets: lib/$(MAKEFILE) FORCE
	cd lib/ && $(MAKE) -f $(MAKEFILE) uninstall
miniweb/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) miniweb/ || $(MKDIR) miniweb/ 
	@$(CHK_DIR_EXISTS) miniweb/ || $(MKDIR) miniweb/ 
sub-miniweb: miniweb/$(MAKEFILE) sub-lib FORCE
	cd miniweb/ && $(MAKE) -f $(MAKEFILE)
sub-miniweb-make_default-ordered: miniweb/$(MAKEFILE) sub-lib-make_default-ordered  FORCE
	cd miniweb/ && $(MAKE) -f $(MAKEFILE) 
sub-miniweb-make_default: miniweb/$(MAKEFILE) sub-lib-make_default FORCE
	cd miniweb/ && $(MAKE) -f $(MAKEFILE) 
sub-miniweb-make_first-ordered: miniweb/$(MAKEFILE) sub-lib-make_first-ordered  FORCE
	cd miniweb/ && $(MAKE) -f $(MAKEFILE) first
sub-miniweb-make_first: miniweb/$(MAKEFILE) sub-lib-make_first FORCE
	cd miniweb/ && $(MAKE) -f $(MAKEFILE) first
sub-miniweb-all-ordered: miniweb/$(MAKEFILE) sub-lib-all-ordered  FORCE
	cd miniweb/ && $(MAKE) -f $(MAKEFILE) all
sub-miniweb-all: miniweb/$(MAKEFILE) sub-lib-all FORCE
	cd miniweb/ && $(MAKE) -f $(MAKEFILE) all
sub-miniweb-clean-ordered: miniweb/$(MAKEFILE) sub-lib-clean-ordered  FORCE
	cd miniweb/ && $(MAKE) -f $(MAKEFILE) clean
sub-miniweb-clean: miniweb/$(MAKEFILE) sub-lib-clean FORCE
	cd miniweb/ && $(MAKE) -f $(MAKEFILE) clean
sub-miniweb-distclean-ordered: miniweb/$(MAKEFILE) sub-lib-distclean-ordered  FORCE
	cd miniweb/ && $(MAKE) -f $(MAKEFILE) distclean
sub-miniweb-distclean: miniweb/$(MAKEFILE) sub-lib-distclean FORCE
	cd miniweb/ && $(MAKE) -f $(MAKEFILE) distclean
sub-miniweb-install_subtargets-ordered: miniweb/$(MAKEFILE) sub-lib-install_subtargets-ordered  FORCE
	cd miniweb/ && $(MAKE) -f $(MAKEFILE) install
sub-miniweb-install_subtargets: miniweb/$(MAKEFILE) sub-lib-install_subtargets FORCE
	cd miniweb/ && $(MAKE) -f $(MAKEFILE) install
sub-miniweb-uninstall_subtargets-ordered: miniweb/$(MAKEFILE) sub-lib-uninstall_subtargets-ordered  FORCE
	cd miniweb/ && $(MAKE) -f $(MAKEFILE) uninstall
sub-miniweb-uninstall_subtargets: miniweb/$(MAKEFILE) sub-lib-uninstall_subtargets FORCE
	cd miniweb/ && $(MAKE) -f $(MAKEFILE) uninstall
handler/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) handler/ || $(MKDIR) handler/ 
	@$(CHK_DIR_EXISTS) handler/ || $(MKDIR) handler/ 
sub-handler: handler/$(MAKEFILE) sub-lib FORCE
	cd handler/ && $(MAKE) -f $(MAKEFILE)
sub-handler-make_default-ordered: handler/$(MAKEFILE) sub-miniweb-make_default-ordered  FORCE
	cd handler/ && $(MAKE) -f $(MAKEFILE) 
sub-handler-make_default: handler/$(MAKEFILE) sub-lib-make_default FORCE
	cd handler/ && $(MAKE) -f $(MAKEFILE) 
sub-handler-make_first-ordered: handler/$(MAKEFILE) sub-miniweb-make_first-ordered  FORCE
	cd handler/ && $(MAKE) -f $(MAKEFILE) first
sub-handler-make_first: handler/$(MAKEFILE) sub-lib-make_first FORCE
	cd handler/ && $(MAKE) -f $(MAKEFILE) first
sub-handler-all-ordered: handler/$(MAKEFILE) sub-miniweb-all-ordered  FORCE
	cd handler/ && $(MAKE) -f $(MAKEFILE) all
sub-handler-all: handler/$(MAKEFILE) sub-lib-all FORCE
	cd handler/ && $(MAKE) -f $(MAKEFILE) all
sub-handler-clean-ordered: handler/$(MAKEFILE) sub-miniweb-clean-ordered  FORCE
	cd handler/ && $(MAKE) -f $(MAKEFILE) clean
sub-handler-clean: handler/$(MAKEFILE) sub-lib-clean FORCE
	cd handler/ && $(MAKE) -f $(MAKEFILE) clean
sub-handler-distclean-ordered: handler/$(MAKEFILE) sub-miniweb-distclean-ordered  FORCE
	cd handler/ && $(MAKE) -f $(MAKEFILE) distclean
sub-handler-distclean: handler/$(MAKEFILE) sub-lib-distclean FORCE
	cd handler/ && $(MAKE) -f $(MAKEFILE) distclean
sub-handler-install_subtargets-ordered: handler/$(MAKEFILE) sub-miniweb-install_subtargets-ordered  FORCE
	cd handler/ && $(MAKE) -f $(MAKEFILE) install
sub-handler-install_subtargets: handler/$(MAKEFILE) sub-lib-install_subtargets FORCE
	cd handler/ && $(MAKE) -f $(MAKEFILE) install
sub-handler-uninstall_subtargets-ordered: handler/$(MAKEFILE) sub-miniweb-uninstall_subtargets-ordered  FORCE
	cd handler/ && $(MAKE) -f $(MAKEFILE) uninstall
sub-handler-uninstall_subtargets: handler/$(MAKEFILE) sub-lib-uninstall_subtargets FORCE
	cd handler/ && $(MAKE) -f $(MAKEFILE) uninstall



make_default: sub-lib-make_default-ordered sub-miniweb-make_default-ordered sub-handler-make_default-ordered FORCE
make_first: sub-lib-make_first-ordered sub-miniweb-make_first-ordered sub-handler-make_first-ordered FORCE
all: sub-lib-all-ordered sub-miniweb-all-ordered sub-handler-all-ordered FORCE
clean: sub-lib-clean-ordered sub-miniweb-clean-ordered sub-handler-clean-ordered FORCE
distclean: sub-lib-distclean-ordered sub-miniweb-distclean-ordered sub-handler-distclean-ordered FORCE
	-$(DEL_FILE) Makefile
install_subtargets: sub-lib-install_subtargets-ordered sub-miniweb-install_subtargets-ordered sub-handler-install_subtargets-ordered FORCE
uninstall_subtargets: sub-lib-uninstall_subtargets-ordered sub-miniweb-uninstall_subtargets-ordered sub-handler-uninstall_subtargets-ordered FORCE
install: install_subtargets  FORCE

uninstall:  uninstall_subtargets FORCE

FORCE:

