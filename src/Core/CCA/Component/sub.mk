#
# Makefile fragment for this subdirectory
# $Id$
#

include $(OBJTOP_ABS)/scripts/largeso_prologue.mk

SRCDIR := Component

SUBDIRS := $(SRCDIR)/CIA $(SRCDIR)/PIDL

include $(OBJTOP_ABS)/scripts/recurse.mk

PSELIBS := SCICore
LIBS := $(GLOBUS_LIBS) -lglobus_nexus -lglobus_dc -lglobus_common

include $(OBJTOP_ABS)/scripts/largeso_epilogue.mk

#
# $Log$
# Revision 1.1  2000/03/17 09:25:10  sparker
# New makefile scheme: sub.mk instead of Makefile.in
# Use XML-based files for module repository
# Plus many other changes to make these two things work
#
#
