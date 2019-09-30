libc_packages := libc6 libc6.1 libc0.1 libc0.3
libc0_1_archs := kfreebsd-amd64 kfreebsd-i386
libc0_3_archs := hurd-i386
libc6_archs   := amd64 arm64 armel armhf hppa i386 m68k mips mipsel mipsn32 mipsn32el mips64 mips64el nios2 powerpc powerpcspe ppc64 ppc64el sparc sparc64 s390x sh4 x32
libc6_1_archs := alpha

control_deps := $(wildcard debian/control.in/*) $(addprefix debian/control.in/, $(libc_packages))

$(patsubst %,debian/control.in/%,$(libc_packages)) :: debian/control.in/% : debian/control.in/libc debian/rules.d/control.mk
	sed -e "s%@libc@%$*%g" \
	    -e "s%@archs@%$($(subst .,_,$*)_archs)%g" \
	    -e "s%@libc-dev-conflict@%$(foreach arch,$(filter-out $*,$(libc_packages)),$(arch)-dev,)%g" \
	    < $< > $@

debian/control: $(stamp)control
$(stamp)control: debian/rules.d/control.mk $(control_deps)

	# Check that all files end with a new line
	set -e ; for i in debian/control.in/* ; do \
		tail -n1 $$i | grep -q "^$$" ; \
	done

	cat debian/control.in/main		>  $@T
	cat debian/control.in/libc6		>> $@T
	cat debian/control.in/libc6.1		>> $@T
	cat debian/control.in/libc0.3		>> $@T
	cat debian/control.in/libc0.1		>> $@T
	cat debian/control.in/i386		>> $@T
	cat debian/control.in/sparc		>> $@T
	cat debian/control.in/sparc64		>> $@T
	cat debian/control.in/s390 		>> $@T
	cat debian/control.in/amd64		>> $@T
	cat debian/control.in/powerpc		>> $@T
	cat debian/control.in/ppc64		>> $@T
	cat debian/control.in/mips32		>> $@T
	cat debian/control.in/mipsn32		>> $@T
	cat debian/control.in/mips64		>> $@T
	cat debian/control.in/armhf		>> $@T
	cat debian/control.in/armel		>> $@T
	cat debian/control.in/kfreebsd-i386	>> $@T
	cat debian/control.in/x32		>> $@T
	cat debian/control.in/opt		>> $@T
	sed -e 's%@libc@%$(libc)%g' -e 's%@GLIBC_VERSION@%$(GLIBC_VERSION)%g' < $@T > debian/control
	rm $@T
	touch $@
