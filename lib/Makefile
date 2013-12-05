#############################################################################
# Makefile for building: lib
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
		sub-corelib

corelib/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) corelib/ || $(MKDIR) corelib/ 
	@$(CHK_DIR_EXISTS) corelib/ || $(MKDIR) corelib/ 
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



make_default: sub-corelib-make_default-ordered FORCE
make_first: sub-corelib-make_first-ordered FORCE
all: sub-corelib-all-ordered FORCE
clean: sub-corelib-clean-ordered FORCE
distclean: sub-corelib-distclean-ordered FORCE
	-$(DEL_FILE) Makefile
install_subtargets: sub-corelib-install_subtargets-ordered FORCE
uninstall_subtargets: sub-corelib-uninstall_subtargets-ordered FORCE
install: install_subtargets  FORCE

uninstall:  uninstall_subtargets FORCE

FORCE:

