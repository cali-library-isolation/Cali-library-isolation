# Should each of these have per-package options?

$(patsubst %,binaryinst_%,$(DEB_ARCH_REGULAR_PACKAGES) $(DEB_INDEP_REGULAR_PACKAGES)) :: binaryinst_% : $(stamp)binaryinst_%

# Make sure the debug packages are built last, since other packages may add
# files to them.
debug-packages = $(filter %-dbg,$(DEB_ARCH_REGULAR_PACKAGES))
non-debug-packages = $(filter-out %-dbg,$(DEB_ARCH_REGULAR_PACKAGES))
$(patsubst %,$(stamp)binaryinst_%,$(debug-packages)):: $(patsubst %,$(stamp)binaryinst_%,$(non-debug-packages))

ifeq ($(filter stage1,$(DEB_BUILD_PROFILES)),)
DH_STRIP_DEBUG_PACKAGE=--dbg-package=$(libc)-dbg
endif

$(patsubst %,$(stamp)binaryinst_%,$(DEB_ARCH_REGULAR_PACKAGES) $(DEB_INDEP_REGULAR_PACKAGES)):: $(patsubst %,$(stamp)install_%,$(GLIBC_PASSES)) debhelper
	@echo Running debhelper for $(curpass)
	dh_testroot
	dh_installdirs -p$(curpass)
	dh_install -p$(curpass)
	dh_installman -p$(curpass)
	dh_installinfo -p$(curpass)
	dh_installdebconf -p$(curpass)
	if [ $(curpass) = glibc-doc ] ; then \
		dh_installchangelogs -p$(curpass) ; \
	else \
		dh_installchangelogs -p$(curpass) debian/changelog.upstream ; \
	fi
	dh_installinit -p$(curpass)
	dh_installdocs -p$(curpass) 
	dh_lintian -p $(curpass)
	dh_link -p$(curpass)
	dh_bugfiles -p$(curpass)

	if test "$(curpass)" = "libc-bin"; then			\
	  mv debian/$(curpass)/sbin/ldconfig			\
	    debian/$(curpass)/sbin/ldconfig.real;		\
	  install -m755 -o0 -g0 debian/local/sbin/ldconfig	\
	    debian/$(curpass)/sbin/ldconfig;			\
	fi

	# when you want to install extra packages, use extra_pkg_install.
	$(call xx,extra_pkg_install)

ifeq ($(filter nostrip,$(DEB_BUILD_OPTIONS)),)
	# libpthread must be stripped specially; GDB needs the
	# non-dynamic symbol table in order to load the thread
	# debugging library.  We keep a full copy of the symbol
	# table in libc6-dbg but basic thread debugging should
	# work even without that package installed.
	if test "$(NOSTRIP_$(curpass))" != 1; then					\
	  if test "$(NODEBUG_$(curpass))" != 1; then					\
	    DH_COMPAT=8 dh_strip -p$(curpass) -Xlibpthread $(DH_STRIP_DEBUG_PACKAGE);	\
	    for f in $$(find debian/$(curpass) -name libpthread-\*.so) ; do		\
	      dbgfile=$$(LC_ALL=C readelf -n $$f | sed -e '/Build ID:/!d'		\
	        -e "s#^.*Build ID: \([0-9a-f]\{2\}\)\([0-9a-f]\+\)#\1/\2.debug#") ;	\
	      dbgpath=debian/$(libc)-dbg/usr/lib/debug/.build-id/$$dbgfile ;		\
	      mkdir -p $$(dirname $$dbgpath) ;						\
	      $(DEB_HOST_GNU_TYPE)-objcopy --only-keep-debug $$f $$dbgpath ;		\
	      $(DEB_HOST_GNU_TYPE)-objcopy --add-gnu-debuglink=$$dbgpath $$f ;		\
	      $(DEB_HOST_GNU_TYPE)-strip --strip-debug --remove-section=.comment	\
	                                 --remove-section=.note $$f ;			\
	    done ;									\
	    for f in $$(find debian/$(curpass) -name \*crt\*.o) ; do			\
	      $(DEB_HOST_GNU_TYPE)-strip --strip-debug --remove-section=.comment	\
	                                 --remove-section=.note $$f ;			\
	    done ;									\
	  else										\
	    DH_COMPAT=8 dh_strip -p$(curpass) -Xlibpthread;				\
	  fi										\
	fi
endif

	dh_compress -p$(curpass)
	dh_fixperms -p$(curpass) -Xpt_chown
	if [ $(curpass) = locales ] ; then \
		chmod +x debian/$(curpass)/usr/share/locales/*-language-pack ; \
	fi
	# Use this instead of -X to dh_fixperms so that we can use
	# an unescaped regular expression.  ld.so must be executable;
	# libc.so and NPTL's libpthread.so print useful version
	# information when executed.
	find debian/$(curpass) -type f \( -regex '.*/ld.*so' \
		-o -regex '.*/libpthread-.*so' \
		-o -regex '.*/libc-.*so' \) \
		-exec chmod a+x '{}' ';'
	dh_makeshlibs -Xgconv/ -p$(curpass) -V "$(call xx,shlib_dep)"
	# Add relevant udeb: lines in shlibs files
	chmod a+x debian/shlibs-add-udebs
	./debian/shlibs-add-udebs $(curpass)

	dh_installdeb -p$(curpass)
	dh_shlibdeps -p$(curpass)
	dh_gencontrol -p$(curpass)
	if [ $(curpass) = nscd ] ; then \
		sed -i -e "s/\(Depends:.*libc[0-9.]\+\)-[a-z0-9]\+/\1/" debian/nscd/DEBIAN/control ; \
	fi
	dh_md5sums -p$(curpass)

	# We adjust the compression format depending on the package:
	# - we slightly increase the compression level for locales-all as it
	#   contains highly compressible data
	# - other packages use dpkg's default xz format
	case $(curpass) in \
	locales-all ) \
		dh_builddeb -p$(curpass) -- -Zxz -z7 ;; \
	*) \
		dh_builddeb -p$(curpass) ;; \
	esac

	touch $@

$(patsubst %,binaryinst_%,$(DEB_UDEB_PACKAGES)) :: binaryinst_% : $(stamp)binaryinst_%
$(patsubst %,$(stamp)binaryinst_%,$(DEB_UDEB_PACKAGES)): debhelper $(patsubst %,$(stamp)install_%,$(GLIBC_PASSES))
	@echo Running debhelper for $(curpass)
	dh_testroot
	dh_installdirs -p$(curpass)
	dh_install -p$(curpass)
	DH_COMPAT=8 dh_strip -p$(curpass)
	
	# when you want to install extra packages, use extra_pkg_install.
	$(call xx,extra_pkg_install)

	dh_compress -p$(curpass)
	dh_fixperms -p$(curpass)
	find debian/$(curpass) -type f \( -regex '.*/ld.*so' \
		-o -regex '.*/libpthread-.*so' \
		-o -regex '.*/libc-.*so' \) \
		-exec chmod a+x '{}' ';'
	dh_installdeb -p$(curpass)
	# dh_shlibdeps -p$(curpass)
	dh_gencontrol -p$(curpass)
	dh_builddeb -p$(curpass)

	touch $@

debhelper: $(stamp)debhelper-common $(patsubst %,$(stamp)debhelper_%,$(GLIBC_PASSES))
$(stamp)debhelper-common: 
	for x in `find debian/debhelper.in -maxdepth 1 -type f`; do \
	  y=debian/`basename $$x`; \
	  perl -p \
	      -e 'BEGIN {undef $$/; open(IN, "debian/script.in/nsscheck.sh"); $$j=<IN>;} s/__NSS_CHECK__/$$j/g;' \
	      -e 'BEGIN {undef $$/; open(IN, "debian/script.in/nohwcap.sh"); $$k=<IN>;} s/__NOHWCAP__/$$k/g;' \
	      -e 'BEGIN {undef $$/; open(IN, "debian/tmp-libc/usr/share/i18n/SUPPORTED"); $$l=<IN>;} s/__PROVIDED_LOCALES__/$$l/g;' \
	      -e 's#GLIBC_VERSION#$(GLIBC_VERSION)#g;' \
	      -e 's#CURRENT_VER#$(DEB_VERSION)#g;' \
	      -e 's#BUILD-TREE#$(build-tree)#g;' \
	      -e 's#LIBC#$(libc)#g;' \
	      -e 's#DEB_HOST_ARCH#$(DEB_HOST_ARCH)#g;' \
	      $$x > $$y ; \
	  case $$y in \
	    *.install) \
	      sed -e "s/^#.*//" -i $$y ; \
	      $(if $(filter $(pt_chown),no),sed -e "/pt_chown/d" -i $$y ;) \
	      $(if $(filter $(pldd),no),sed -e "/pldd/d" -i $$y ;) \
	      ;; \
	  esac; \
	done

	# Generate common substvars files.
	: > tmp.substvars
ifeq ($(filter stage2,$(DEB_BUILD_PROFILES)),)
	echo 'libgcc:Depends=libgcc1 [!hppa !m68k], libgcc2 [m68k], libgcc4 [hppa]' >> tmp.substvars
endif

	for pkg in $(DEB_ARCH_REGULAR_PACKAGES) $(DEB_INDEP_REGULAR_PACKAGES) $(DEB_UDEB_PACKAGES); do \
	  cp tmp.substvars debian/$$pkg.substvars; \
	done
	rm -f tmp.substvars

	touch $@

ifneq ($(filter stage1,$(DEB_BUILD_PROFILES)),)
$(patsubst %,debhelper_%,$(GLIBC_PASSES)) :: debhelper_% : $(stamp)debhelper_%
$(stamp)debhelper_%: $(stamp)debhelper-common $(stamp)install_%
	libdir=$(call xx,libdir) ; \
	slibdir=$(call xx,slibdir) ; \
	rtlddir=$(call xx,rtlddir) ; \
	curpass=$(curpass) ; \
	templates="libc-dev" ;\
	pass="" ; \
	suffix="" ;\
	case "$$curpass:$$slibdir" in \
	  libc:*) \
	    ;; \
	  *:/lib32 | *:/lib64 | *:/libo32 | *:/libx32 | *:/lib/arm-linux-gnueabi*) \
	    pass="-alt" \
	    suffix="-$(curpass)" \
	    ;; \
	  *:* ) \
           templates="" \
	    ;; \
	esac ; \
	for t in $$templates ; do \
	  for s in debian/$$t$$pass.* ; do \
	    t=`echo $$s | sed -e "s#libc\(.*\)$$pass#$(libc)\1$$suffix#"` ; \
	    echo "Generating $$t ..."; \
	    if [ "$$s" != "$$t" ] ; then \
	      cp $$s $$t ; \
	    fi ; \
	    sed -i \
		-e "/LIBDIR.*\.a /d" \
		-e "s#TMPDIR#debian/tmp-$$curpass#g" \
		-e "s#RTLDDIR#$$rtlddir#g" \
		-e "s#SLIBDIR#$$slibdir#g" \
		-e "s#LIBDIR#$$libdir#g" \
	      $$t; \
	  done ; \
	done
else
$(patsubst %,debhelper_%,$(GLIBC_PASSES)) :: debhelper_% : $(stamp)debhelper_%
$(stamp)debhelper_%: $(stamp)debhelper-common $(stamp)install_%
	libdir=$(call xx,libdir) ; \
	slibdir=$(call xx,slibdir) ; \
	rtlddir=$(call xx,rtlddir) ; \
	curpass=$(curpass) ; \
	rtld_so=`LANG=C LC_ALL=C readelf -l debian/tmp-$$curpass/usr/bin/iconv | grep "interpreter" | sed -e 's/.*interpreter: \(.*\)]/\1/g'`; \
	case "$$curpass:$$slibdir" in \
	  libc:*) \
	    templates="libc libc-dev libc-pic libc-udeb" \
	    pass="" \
	    suffix="" \
	    ;; \
	  *:/lib32 | *:/lib64 | *:/libo32 | *:/libx32 | *:/lib/arm-linux-gnueabi*) \
	    templates="libc libc-dev" \
	    pass="-alt" \
	    suffix="-$(curpass)" \
	    ;; \
	  *:*) \
	    templates="libc" \
	    pass="-otherbuild" \
	    suffix="-$(curpass)" \
	    ;; \
	esac ; \
	for t in $$templates ; do \
	  for s in debian/$$t$$pass.* ; do \
	    t=`echo $$s | sed -e "s#libc\(.*\)$$pass#$(libc)\1$$suffix#"` ; \
	    if [ "$$s" != "$$t" ] ; then \
	      cp $$s $$t ; \
	    fi ; \
	    sed -e "s#TMPDIR#debian/tmp-$$curpass#g" -i $$t; \
	    sed -e "s#RTLDDIR#$$rtlddir#g" -i $$t; \
	    sed -e "s#SLIBDIR#$$slibdir#g" -i $$t; \
	    sed -e "s#LIBDIR#$$libdir#g" -i $$t; \
	    sed -e "s#FLAVOR#$$curpass#g" -i $$t; \
	    sed -e "s#RTLD_SO#$$rtld_so#g" -i $$t ; \
	    sed -e "s#MULTIARCHDIR#$$DEB_HOST_MULTIARCH#g" -i $$t ; \
	    $(if $(filter $(call xx,mvec),no),sed -e "/libmvec/d" -i $$t ;) \
	  done ; \
	done
endif

	touch $@

clean::
	dh_clean 

	rm -f debian/*.install
	rm -f debian/*.install.*
	rm -f debian/*.manpages
	rm -f debian/*.links
	rm -f debian/*.postinst
	rm -f debian/*.preinst
	rm -f debian/*.postinst
	rm -f debian/*.prerm
	rm -f debian/*.postrm
	rm -f debian/*.info
	rm -f debian/*.init
	rm -f debian/*.config
	rm -f debian/*.templates
	rm -f debian/*.dirs
	rm -f debian/*.docs
	rm -f debian/*.doc-base
	rm -f debian/*.generated
	rm -f debian/*.lintian-overrides
	rm -f debian/*.NEWS
	rm -f debian/*.README.Debian
	rm -f debian/*.triggers

	rm -f $(stamp)binaryinst*
