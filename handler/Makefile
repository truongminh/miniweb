#############################################################################
# Makefile for building: handler
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
		sub-applog \
		sub-ok

applog/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) applog/ || $(MKDIR) applog/ 
	@$(CHK_DIR_EXISTS) applog/ || $(MKDIR) applog/ 
sub-applog: applog/$(MAKEFILE) FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE)
sub-applog-make_default-ordered: applog/$(MAKEFILE) FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) 
sub-applog-make_default: applog/$(MAKEFILE) FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) 
sub-applog-make_first-ordered: applog/$(MAKEFILE) FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) first
sub-applog-make_first: applog/$(MAKEFILE) FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) first
sub-applog-all-ordered: applog/$(MAKEFILE) FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) all
sub-applog-all: applog/$(MAKEFILE) FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) all
sub-applog-clean-ordered: applog/$(MAKEFILE) FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) clean
sub-applog-clean: applog/$(MAKEFILE) FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) clean
sub-applog-distclean-ordered: applog/$(MAKEFILE) FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) distclean
sub-applog-distclean: applog/$(MAKEFILE) FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) distclean
sub-applog-install_subtargets-ordered: applog/$(MAKEFILE) FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) install
sub-applog-install_subtargets: applog/$(MAKEFILE) FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) install
sub-applog-uninstall_subtargets-ordered: applog/$(MAKEFILE) FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) uninstall
sub-applog-uninstall_subtargets: applog/$(MAKEFILE) FORCE
	cd applog/ && $(MAKE) -f $(MAKEFILE) uninstall
ok/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) ok/ || $(MKDIR) ok/ 
	@$(CHK_DIR_EXISTS) ok/ || $(MKDIR) ok/ 
sub-ok: ok/$(MAKEFILE) FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE)
sub-ok-make_default-ordered: ok/$(MAKEFILE) sub-applog-make_default-ordered  FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) 
sub-ok-make_default: ok/$(MAKEFILE) FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) 
sub-ok-make_first-ordered: ok/$(MAKEFILE) sub-applog-make_first-ordered  FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) first
sub-ok-make_first: ok/$(MAKEFILE) FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) first
sub-ok-all-ordered: ok/$(MAKEFILE) sub-applog-all-ordered  FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) all
sub-ok-all: ok/$(MAKEFILE) FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) all
sub-ok-clean-ordered: ok/$(MAKEFILE) sub-applog-clean-ordered  FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) clean
sub-ok-clean: ok/$(MAKEFILE) FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) clean
sub-ok-distclean-ordered: ok/$(MAKEFILE) sub-applog-distclean-ordered  FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) distclean
sub-ok-distclean: ok/$(MAKEFILE) FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) distclean
sub-ok-install_subtargets-ordered: ok/$(MAKEFILE) sub-applog-install_subtargets-ordered  FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) install
sub-ok-install_subtargets: ok/$(MAKEFILE) FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) install
sub-ok-uninstall_subtargets-ordered: ok/$(MAKEFILE) sub-applog-uninstall_subtargets-ordered  FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) uninstall
sub-ok-uninstall_subtargets: ok/$(MAKEFILE) FORCE
	cd ok/ && $(MAKE) -f $(MAKEFILE) uninstall



make_default: sub-applog-make_default-ordered sub-ok-make_default-ordered FORCE
make_first: sub-applog-make_first-ordered sub-ok-make_first-ordered FORCE
all: sub-applog-all-ordered sub-ok-all-ordered FORCE
clean: sub-applog-clean-ordered sub-ok-clean-ordered FORCE
distclean: sub-applog-distclean-ordered sub-ok-distclean-ordered FORCE
	-$(DEL_FILE) Makefile
install_subtargets: sub-applog-install_subtargets-ordered sub-ok-install_subtargets-ordered FORCE
uninstall_subtargets: sub-applog-uninstall_subtargets-ordered sub-ok-uninstall_subtargets-ordered FORCE
install: install_subtargets  FORCE

uninstall:  uninstall_subtargets FORCE

FORCE:

