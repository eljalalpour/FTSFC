SHELL = /bin/bash


# THESE VARIABLES MAY CHANGE BETWEEN PACKAGES

# Set 'package' to the name of your package.
package := FTSFC

# Set these variables appropriately.
top_builddir := .
subdir := .

# Require Click prefix settings.
# Generally, you will provide a '--with-click=CLICKPREFIX' option, and set:
clickbuild_prefix := /usr/local
include /usr/local/share/click/config.mk

# Set 'MAKE_UPACKAGE', 'MAKE_KPACKAGE', and 'MAKE_BPACKAGE' to '0' or '1'
# to determine whether to build userlevel, linuxmodule, and bsdmodule
# packages, respectively.
MAKE_UPACKAGE = 1
MAKE_KPACKAGE = 0
MAKE_BPACKAGE = 0

# Set 'KVERSIONFLAGS' to '--gpl' to generate a dual BSD/GPL licensed
# linuxmodule package.
KVERSIONFLAGS = --gpl

# Set 'STRIP_UPACKAGE' to 'strip -g' to strip debugging symbols from the
# userlevel package before installing.
#STRIP_UPACKAGE = $(STRIP) -g

# Set these variables as required.
DEFS = -DPACKAGE_NAME=\"click-FTSFC\" -DPACKAGE_TARNAME=\"click-ftsfc\" -DPACKAGE_VERSION=\"2.0.1\" -DPACKAGE_STRING=\"click-FTSFC\ 2.0.1\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" $(CLICKDEFS)
INCLUDES = -I. -I$(top_builddir) -I$(srcdir) $(CLICKINCLUDES)

# Set this variable to force 'click-elem2package' to include a header,
# such as your package's '<config.h>', if necessary.
#ELEM2PACKAGE_INCLUDES = -i'<config.h>'

# END OF VARIABLES THAT MAY CHANGE BETWEEN PACKAGES


# Everything below here will probably stay unchanged

srcdir := .
top_srcdir := .

ifeq ($(CLICK_PACKAGE_MAKING),)
prefix = /usr/local
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libdir = ${exec_prefix}/lib
mandir = ${datarootdir}/man
datarootdir = ${prefix}/share
datadir = ${datarootdir}
endif

include $(clickbuild_datadir)/pkg-Makefile
