# Because variables can be masked at anypoint by declaring
# PASS_VAR, we need to call all variables as $(call xx,VAR)
# This little bit of magic makes it possible:
xx=$(if $($(curpass)_$(1)),$($(curpass)_$(1)),$($(1)))

# We want to log output to a logfile but we also need to preserve the
# return code of the command being run.
# This little bit of magic makes it possible:
# $(call logme, [-a] <log file>, <cmd>)
define logme
(exec 3>&1; exit `( ( ( $(2) ) 2>&1 3>&-; echo $$? >&4) | tee $(1) >&3) 4>&1`)
endef

ifneq ($(filter stage1,$(DEB_BUILD_PROFILES)),)
    libc_extra_config_options = $(extra_config_options) --disable-sanity-checks \
                               --enable-hacker-mode
endif

ifdef WITH_SYSROOT
    libc_extra_config_options += --with-headers=$(WITH_SYSROOT)/$(includedir)
endif

$(patsubst %,mkbuilddir_%,$(GLIBC_PASSES)) :: mkbuilddir_% : $(stamp)mkbuilddir_%
$(stamp)mkbuilddir_%: $(stamp)patch $(KERNEL_HEADER_DIR)
	@echo Making builddir for $(curpass)
	test -d $(DEB_BUILDDIR) || mkdir -p $(DEB_BUILDDIR)
	touch $@

$(patsubst %,configure_%,$(GLIBC_PASSES)) :: configure_% : $(stamp)configure_%
$(stamp)configure_%: $(stamp)mkbuilddir_%
	@echo Configuring $(curpass)
	rm -f $(DEB_BUILDDIR)/configparms
	echo "CC = $(call xx,CC)"                 >> $(DEB_BUILDDIR)/configparms
	echo "CXX = $(call xx,CXX)"               >> $(DEB_BUILDDIR)/configparms
	echo "MIG = $(call xx,MIG)"               >> $(DEB_BUILDDIR)/configparms
	echo "BUILD_CC = $(BUILD_CC)"             >> $(DEB_BUILDDIR)/configparms
	echo "BUILD_CXX = $(BUILD_CXX)"           >> $(DEB_BUILDDIR)/configparms
	echo "CFLAGS = $(HOST_CFLAGS)"            >> $(DEB_BUILDDIR)/configparms
	echo "ASFLAGS = $(HOST_CFLAGS)"           >> $(DEB_BUILDDIR)/configparms
	echo "BUILD_CFLAGS = $(BUILD_CFLAGS)"     >> $(DEB_BUILDDIR)/configparms
	echo "LDFLAGS = "                         >> $(DEB_BUILDDIR)/configparms
	echo "BASH := /bin/bash"                  >> $(DEB_BUILDDIR)/configparms
	echo "KSH := /bin/bash"                   >> $(DEB_BUILDDIR)/configparms
	echo "SHELL := /bin/bash"                 >> $(DEB_BUILDDIR)/configparms
	echo "LIBGD = no"                         >> $(DEB_BUILDDIR)/configparms
	echo "bindir = $(bindir)"                 >> $(DEB_BUILDDIR)/configparms
	echo "datadir = $(datadir)"               >> $(DEB_BUILDDIR)/configparms
	echo "complocaledir = $(complocaledir)"   >> $(DEB_BUILDDIR)/configparms
	echo "sysconfdir = $(sysconfdir)"         >> $(DEB_BUILDDIR)/configparms
	echo "libexecdir = $(libexecdir)"         >> $(DEB_BUILDDIR)/configparms
	echo "rootsbindir = $(rootsbindir)"       >> $(DEB_BUILDDIR)/configparms
	echo "includedir = $(call xx,includedir)" >> $(DEB_BUILDDIR)/configparms
	echo "docdir = $(docdir)"                 >> $(DEB_BUILDDIR)/configparms
	echo "mandir = $(mandir)"                 >> $(DEB_BUILDDIR)/configparms
	echo "sbindir = $(sbindir)"               >> $(DEB_BUILDDIR)/configparms
	echo "vardbdir = $(vardbdir)"             >> $(DEB_BUILDDIR)/configparms
	echo "libdir = $(call xx,libdir)"         >> $(DEB_BUILDDIR)/configparms
	echo "slibdir = $(call xx,slibdir)"       >> $(DEB_BUILDDIR)/configparms
	echo "rtlddir = $(call xx,rtlddir)"       >> $(DEB_BUILDDIR)/configparms

	# Define the installation directory for all calls to make. This avoid
	# broken glibc makefiles to spuriously trigger install rules trying to
	# overwrite system headers.
	echo "install_root = $(CURDIR)/debian/tmp-$(curpass)" >> $(DEB_BUILDDIR)/configparms

	# Per architecture debian specific tests whitelist
	echo "include $(CURDIR)/debian/testsuite-xfail-debian.mk" >> $(DEB_BUILDDIR)/configparms

	# Prevent autoconf from running unexpectedly by setting it to false.
	# Also explicitly pass CC down - this is needed to get -m64 on
	# Sparc, et cetera.
	configure_build=$(call xx,configure_build); \
	if [ $(call xx,configure_target) = $$configure_build ]; then \
	  echo "Checking that we're running at least kernel version: $(call xx,MIN_KERNEL_SUPPORTED)"; \
	  if ! $(call kernel_check,$(call xx,MIN_KERNEL_SUPPORTED)); then \
	    configure_build=`echo $$configure_build | sed 's/^\([^-]*\)-\([^-]*\)$$/\1-dummy-\2/'`; \
	    echo "No.  Forcing cross-compile by setting build to $$configure_build."; \
	  fi; \
	fi; \
	$(call logme, -a $(log_build), echo -n "Build started: " ; date --rfc-2822 ; echo "---------------") ; \
	$(call logme, -a $(log_build), \
		cd $(DEB_BUILDDIR) && \
		CC="$(call xx,CC)" \
		CXX="$(call xx,CXX)" \
		MIG="$(call xx,MIG)" \
		AUTOCONF=false \
		MAKEINFO=: \
		$(CURDIR)/configure \
		--host=$(call xx,configure_target) \
		--build=$$configure_build --prefix=/usr \
		--enable-add-ons=$(standard-add-ons)"$(call xx,add-ons)" \
		--without-selinux \
		--enable-stackguard-randomization \
		--enable-obsolete-rpc \
		--with-pkgversion="Ubuntu GLIBC $(DEB_VERSION)" \
		--with-bugurl="https://bugs.launchpad.net/ubuntu/+source/glibc/+bugs" \
		$(if $(filter $(pt_chown),yes),--enable-pt_chown) \
		$(if $(filter $(threads),no),--disable-nscd) \
		$(if $(filter $(call xx,mvec),no),--disable-mathvec) \
		$(call xx,with_headers) $(call xx,extra_config_options))
	touch $@

$(patsubst %,build_%,$(GLIBC_PASSES)) :: build_% : $(stamp)build_%
$(stamp)build_%: $(stamp)configure_%
	@echo Building $(curpass)

ifneq ($(filter stage1,$(DEB_BUILD_PROFILES)),)
	$(MAKE) cross-compiling=yes -C $(DEB_BUILDDIR) $(NJOBS) csu/subdir_lib
else
	$(call logme, -a $(log_build), $(MAKE) -C $(DEB_BUILDDIR) $(NJOBS))
	$(call logme, -a $(log_build), echo "---------------" ; echo -n "Build ended: " ; date --rfc-2822)
endif
	touch $@

$(patsubst %,check_%,$(GLIBC_PASSES)) :: check_% : $(stamp)check_%
$(stamp)check_%: $(stamp)build_%
	@set -e ; \
	if [ -n "$(findstring nocheck,$(DEB_BUILD_OPTIONS))" ]; then \
	  echo "Tests have been disabled via DEB_BUILD_OPTIONS." ; \
	elif [ $(call xx,configure_build) != $(call xx,configure_target) ] && \
	     ! $(DEB_BUILDDIR)/elf/ld.so $(DEB_BUILDDIR)/libc.so >/dev/null 2>&1 ; then \
	  echo "Flavour cross-compiled, tests have been skipped." ; \
	elif ! $(call kernel_check,$(call xx,MIN_KERNEL_SUPPORTED)); then \
	  echo "Kernel too old, tests have been skipped." ; \
	elif [ $(call xx,RUN_TESTSUITE) != "yes" ]; then \
	  echo "Testsuite disabled for $(curpass), skipping tests."; \
	else \
	  find $(DEB_BUILDDIR) -name '*.out' -delete ; \
	  LD_PRELOAD="" LANG="" TIMEOUTFACTOR="50" $(MAKE) -C $(DEB_BUILDDIR) $(NJOBS) check 2>&1 | tee $(log_test) ; \
	  if ! test -f $(DEB_BUILDDIR)/tests.sum ; then \
	    echo "+---------------------------------------------------------------------+" ; \
	    echo "|                     Testsuite failed to build.                      |" ; \
	    echo "+---------------------------------------------------------------------+" ; \
	    exit 1 ; \
	  fi ; \
	  echo "+---------------------------------------------------------------------+" ; \
	  echo "|                             Testsuite summary                       |" ; \
	  echo "+---------------------------------------------------------------------+" ; \
	  grep -E '^(FAIL|XFAIL|XPASS):' $(DEB_BUILDDIR)/tests.sum | sort ; \
	  for test in $$(sed -e '/^\(FAIL\|XFAIL\): /!d;s/^.*: //' $(DEB_BUILDDIR)/tests.sum) ; do \
	    echo "----------" ; \
	    cat $(DEB_BUILDDIR)/$$test.test-result ; \
	    cat $(DEB_BUILDDIR)/$$test.out ; \
	    echo "----------" ; \
	  done ; \
	  if grep -q '^FAIL:' $(DEB_BUILDDIR)/tests.sum ; then \
	    echo "+---------------------------------------------------------------------+" ; \
	    echo "|     Encountered regressions that don't match expected failures.     |" ; \
	    echo "+---------------------------------------------------------------------+" ; \
	    exit 1 ; \
	  else \
	    echo "+---------------------------------------------------------------------+" ; \
	    echo "| Passed regression testing.  Give yourself a hearty pat on the back. |" ; \
	    echo "+---------------------------------------------------------------------+" ; \
	  fi ; \
	fi
	touch $@

$(patsubst %,install_%,$(GLIBC_PASSES)) :: install_% : $(stamp)install_%
$(stamp)install_%: $(stamp)build_%
	@echo Installing $(curpass)
	rm -rf $(CURDIR)/debian/tmp-$(curpass)
ifneq ($(filter stage1,$(DEB_BUILD_PROFILES)),)
	$(call logme, -a $(log_build), $(MAKE) -C $(DEB_BUILDDIR) $(NJOBS)	\
	    cross-compiling=yes install_root=$(CURDIR)/debian/tmp-$(curpass)	\
	    install-bootstrap-headers=yes install-headers )

	install -d $(CURDIR)/debian/tmp-$(curpass)/$(call xx,libdir)
	install -m 644 $(DEB_BUILDDIR)/csu/crt[01in].o $(CURDIR)/debian/tmp-$(curpass)/$(call xx,libdir)/.
	$(call xx,CC) -nostdlib -nostartfiles -shared -x c /dev/null \
	        -o $(CURDIR)/debian/tmp-$(curpass)/$(call xx,libdir)/libc.so
else
	: # FIXME: why just needed for ARM multilib?
	case "$(curpass)" in \
	        armhf) \
			libgcc_dirs=/lib/arm-linux-gnueabihf; \
			if [ -n "$$WITH_BUILD_SYSROOT" ]; then \
			  libgcc_dirs="$$WITH_BUILD_SYSROOT/usr/arm-linux-gnueabi/lib/arm-linux-gnueabihf $$WITH_BUILD_SYSROOT/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/hf"; \
			fi; \
			;; \
	        armel) \
			libgcc_dirs=/lib/arm-linux-gnueabi; \
			if [ -n "$$WITH_BUILD_SYSROOT" ]; then \
			  libgcc_dirs="$$WITH_BUILD_SYSROOT/usr/arm-linux-gnueabihf/lib/arm-linux-gnueabi $$WITH_BUILD_SYSROOT/usr/lib/gcc-cross/arm-linux-gnueabihf/4.7/sf"; \
			fi; \
			;; \
	esac; \
	if [ -n "$$libgcc_dirs" ]; then \
	  for d in $$libgcc_dirs; do \
	    if [ -f $$d/libgcc_s.so.1 ]; then \
	      cp -p $$d/libgcc_s.so.1 $(DEB_BUILDDIR)/; \
	      break; \
	    fi; \
	  done; \
	fi
	$(MAKE) -C $(DEB_BUILDDIR) \
	  install_root=$(CURDIR)/debian/tmp-$(curpass) install

	# Generate gconv-modules.cache
	case $(curpass)-$(call xx,slibdir) in libc-* | *-/lib32 | *-/lib64 | *-/libo32 | *-/libx32) \
	  /usr/sbin/iconvconfig --nostdlib --prefix=$(CURDIR)/debian/tmp-$(curpass) \
				-o $(CURDIR)/debian/tmp-$(curpass)/$(call xx,libdir)/gconv/gconv-modules.cache \
				$(call xx,libdir)/gconv \
	  ;; \
	esac

	# Generate the list of SUPPORTED locales
	if [ $(curpass) = libc ]; then \
	  $(MAKE) -f debian/generate-supported.mk IN=localedata/SUPPORTED \
	    OUT=debian/tmp-$(curpass)/usr/share/i18n/SUPPORTED; \
	fi
endif

	# Create the multiarch directories, and the configuration file in /etc/ld.so.conf.d
	if [ $(curpass) = libc ]; then \
	  mkdir -p debian/tmp-$(curpass)/etc/ld.so.conf.d; \
	  conffile="debian/tmp-$(curpass)/etc/ld.so.conf.d/$(DEB_HOST_MULTIARCH).conf"; \
	  echo "# Multiarch support" > $$conffile; \
	  echo "$(call xx,slibdir)" >> $$conffile; \
	  echo "$(call xx,libdir)" >> $$conffile; \
	  if [ "$(DEB_HOST_GNU_TYPE)" != "$(DEB_HOST_MULTIARCH)" ]; then \
	    echo "/lib/$(DEB_HOST_GNU_TYPE)" >> $$conffile; \
	    echo "/usr/lib/$(DEB_HOST_GNU_TYPE)" >> $$conffile; \
	  fi; \
	  mkdir -p debian/tmp-$(curpass)/usr/include/$(DEB_HOST_MULTIARCH); \
	  mv debian/tmp-$(curpass)/usr/include/bits debian/tmp-$(curpass)/usr/include/$(DEB_HOST_MULTIARCH); \
	  mv debian/tmp-$(curpass)/usr/include/gnu debian/tmp-$(curpass)/usr/include/$(DEB_HOST_MULTIARCH); \
	  mv debian/tmp-$(curpass)/usr/include/sys debian/tmp-$(curpass)/usr/include/$(DEB_HOST_MULTIARCH); \
	  mv debian/tmp-$(curpass)/usr/include/fpu_control.h debian/tmp-$(curpass)/usr/include/$(DEB_HOST_MULTIARCH); \
	  mv debian/tmp-$(curpass)/usr/include/a.out.h debian/tmp-$(curpass)/usr/include/$(DEB_HOST_MULTIARCH); \
	  mv debian/tmp-$(curpass)/usr/include/ieee754.h debian/tmp-$(curpass)/usr/include/$(DEB_HOST_MULTIARCH); \
	fi

ifeq ($(filter stage1,$(DEB_BUILD_PROFILES)),)
	# For our biarch libc, add an ld.so.conf.d configuration; this
	# is needed because multiarch libc Replaces: libc6-i386 for ld.so, and
	# the multiarch ld.so doesn't look at the (non-standard) /lib32, so we
	# need path compatibility when biarch and multiarch packages are both
	# installed.
	case $(call xx,slibdir) in /lib32 | /lib64 | /libo32 | /libx32) \
	  mkdir -p debian/tmp-$(curpass)/etc/ld.so.conf.d; \
	  conffile="debian/tmp-$(curpass)/etc/ld.so.conf.d/zz_$(curpass)-biarch-compat.conf"; \
	  echo "# Legacy biarch compatibility support" > $$conffile; \
	  echo "$(call xx,slibdir)" >> $$conffile; \
	  echo "$(call xx,libdir)" >> $$conffile; \
	  ;; \
	esac

	# handle the non-default multilib for arm targets
	case $(curpass) in arm*) \
	  mkdir -p debian/tmp-$(curpass)/etc/ld.so.conf.d; \
	  conffile="debian/tmp-$(curpass)/etc/ld.so.conf.d/zz_$(curpass)-biarch-compat.conf"; \
	  echo "# Multiarch support" > $$conffile; \
	  echo "$(call xx,slibdir)" >> $$conffile; \
	  echo "$(call xx,libdir)" >> $$conffile; \
	esac

	# ARM: add dynamic linker name for the non-default multilib in ldd
	if [ $(curpass) = libc ]; then \
	  case $(DEB_HOST_ARCH) in \
	    armel) \
	      sed -i '/RTLDLIST=/s,=\(.*\),="\1 /lib/ld-linux-armhf.so.3",' debian/tmp-$(curpass)/usr/bin/ldd;; \
	    armhf) \
	      sed -i '/RTLDLIST=/s,=\(.*\),="\1 /lib/ld-linux.so.3",' debian/tmp-$(curpass)/usr/bin/ldd;; \
	  esac; \
	fi

	# Create the ld.so symlink to the multiarch directory
	if [ $(curpass) = libc ]; then \
	  rtld_so="$$(LANG=C LC_ALL=C readelf -l debian/tmp-$(curpass)/usr/bin/iconv | grep 'interpreter' | sed -e 's/.*interpreter: \(.*\)]/\1/g')" ; \
	  rtld_so="$$(basename $$rtld_so)" ; \
	  link_name="debian/tmp-$(curpass)/lib/$$rtld_so" ; \
	  target="$(call xx,slibdir)/$$(readlink debian/tmp-$(curpass)/$(call xx,slibdir)/$$rtld_so)" ; \
	  ln -s $$target $$link_name ;  \
	fi
	
	$(call xx,extra_install)
endif
	touch $@

#
# Make sure to use the just built localedef for native builds. When
# cross-compiling use the system localedef passing --little-endian
# or --big-endian to select the correct endianess. A cross-specific
# build-dependency makes sure that the correct version is used, as
# the format might change between upstream versions.
#
ifeq ($(DEB_BUILD_ARCH),$(DEB_HOST_ARCH))
LOCALEDEF = I18NPATH=$(CURDIR)/localedata \
	    GCONV_PATH=$(CURDIR)/$(DEB_BUILDDIRLIBC)/iconvdata \
	    LC_ALL=C \
	    $(CURDIR)/$(DEB_BUILDDIRLIBC)/elf/ld.so --library-path $(CURDIR)/$(DEB_BUILDDIRLIBC) \
	    $(CURDIR)/$(DEB_BUILDDIRLIBC)/locale/localedef
else
LOCALEDEF = I18NPATH=$(CURDIR)/localedata \
	    GCONV_PATH=$(CURDIR)/$(DEB_BUILDDIRLIBC)/iconvdata \
	    LC_ALL=C \
	    localedef --$(DEB_HOST_ARCH_ENDIAN)-endian
endif

$(stamp)build_C.UTF-8: $(stamp)/build_libc
	$(LOCALEDEF) --quiet -c -f UTF-8 -i C $(CURDIR)/build-tree/C.UTF-8
	touch $@

$(stamp)build_locales-all: $(stamp)/build_libc
	$(MAKE) -C $(DEB_BUILDDIRLIBC) $(NJOBS) \
		objdir=$(DEB_BUILDDIRLIBC) \
		install_root=$(CURDIR)/build-tree/locales-all \
		localedata/install-locales LOCALEDEF="$(LOCALEDEF)"
	rdfind -outputname /dev/null -makesymlinks true -removeidentinode false \
		$(CURDIR)/build-tree/locales-all/usr/lib/locale
	symlinks -r -s -c $(CURDIR)/build-tree/locales-all/usr/lib/locale
	touch $@

$(stamp)source: $(stamp)patch
	mkdir -p $(build-tree)
	cd .. && \
	       find $(GLIBC_SOURCES) -depth -newermt '$(DEB_BUILD_DATE)' \
			-print0 | \
               xargs -0r touch --no-dereference --date='$(DEB_BUILD_DATE)'
	cd .. && \
		find $(GLIBC_SOURCES) -print0 | \
		LC_ALL=C sort -z | \
		tar -c -J --null -T - --no-recursion \
			--mode=go=rX,u+rw,a-s \
			--owner=root --group=root --numeric-owner \
			-f $(CURDIR)/$(build-tree)/glibc-$(GLIBC_VERSION).tar.xz
	mkdir -p debian/glibc-source/usr/src/glibc
	tar cf - --files-from debian/glibc-source.filelist \
	  | tar -x -C debian/glibc-source/usr/src/glibc -f -

	touch $@

.NOTPARALLEL: $(patsubst %,check_%,$(GLIBC_PASSES))
