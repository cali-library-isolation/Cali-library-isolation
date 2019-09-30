ifeq ($(filter -mabi=32,$(CC)),)
CC += -mabi=32
endif

ifeq ($(subdir),csu)
ifeq (yes,$(build-shared))
# Compatibility
sysdep_routines += libgcc-compat
shared-only-routines += libgcc-compat
endif
endif
