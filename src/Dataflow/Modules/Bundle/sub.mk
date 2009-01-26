# 
# 
# The MIT License
# 
# Copyright (c) 1997-2009 Center for the Simulation of Accidental Fires and 
# Explosions (CSAFE), and  Scientific Computing and Imaging Institute (SCI), 
# University of Utah.
# 
# License for the specific language governing rights and limitations under
# Permission is hereby granted, free of charge, to any person obtaining a 
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation 
# the rights to use, copy, modify, merge, publish, distribute, sublicense, 
# and/or sell copies of the Software, and to permit persons to whom the 
# Software is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included 
# in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
# DEALINGS IN THE SOFTWARE.
# 
# 
# 
# 
# *** NOTE ***
#
# Do not remove or modify the comment line:
#
# #[INSERT NEW ?????? HERE]
#
# It is required by the Component Wizard to properly edit this file.
# if you want to edit this file by hand, see the "Create A New Component"
# documentation on how to do it correctly.

include $(SCIRUN_SCRIPTS)/smallso_prologue.mk

SRCDIR   := Dataflow/Modules/Bundle

SRCS += $(SRCDIR)/BundleSetBundle.cc\
        $(SRCDIR)/BundleSetField.cc\
        $(SRCDIR)/BundleSetMatrix.cc\
        $(SRCDIR)/BundleSetNrrd.cc\
        $(SRCDIR)/BundleSetColorMap.cc\
        $(SRCDIR)/BundleSetColorMap2.cc\
        $(SRCDIR)/BundleSetPath.cc\
        $(SRCDIR)/BundleSetString.cc\
        $(SRCDIR)/BundleInfo.cc\
        $(SRCDIR)/BundleGetBundle.cc\
        $(SRCDIR)/BundleGetField.cc\
        $(SRCDIR)/BundleGetMatrix.cc\
        $(SRCDIR)/BundleGetNrrd.cc\
        $(SRCDIR)/BundleGetColorMap.cc\
        $(SRCDIR)/BundleGetColorMap2.cc\
        $(SRCDIR)/BundleGetPath.cc\
        $(SRCDIR)/BundleGetString.cc\
        $(SRCDIR)/BundleMerge.cc\
#[INSERT NEW CODE FILE HERE]

PSELIBS := \
	Core/Datatypes     \
	Dataflow/Network   \
        Core/Persistent    \
	Core/Containers    \
	Core/Util          \
	Core/Exceptions    \
	Core/Thread        \
	Core/GuiInterface  \
        Core/Geom          \
	Core/Datatypes     \
	Core/Geometry      \
        Core/GeomInterface \
	Core/TkExtensions  \
        Core/Volume        \
	Core/Bundle

LIBS := $(TK_LIBRARY) $(GL_LIBRARY) $(M_LIBRARY)

include $(SCIRUN_SCRIPTS)/smallso_epilogue.mk

ifeq ($(LARGESOS),no)
  SCIRUN_MODULES := $(SCIRUN_MODULES) $(LIBNAME)
endif

