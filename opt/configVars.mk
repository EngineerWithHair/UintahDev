#
#  The MIT License
#
#  Copyright (c) 1997-2023 The University of Utah
# 
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to
#  deal in the Software without restriction, including without limitation the
#  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
#  sell copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
# 
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
# 
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
#  IN THE SOFTWARE.



# configVars.mk.in holds all of the variables needed by the make 
# system to create makefiles. Various Makefiles can include this central
# copy of the vars. This file has all of the variables and rules common to 
# all generated makefiles.

# 'a' if archives, 'so' if shared libs.
SO_OR_A_FILE := so
IS_STATIC_BUILD := no

# Blow away a bunch of makes internal rules to improve the performance
# of make
.SUFFIXES:
% :: RCS/%,v
% :: RCS/%
% :: %,v
% :: s.%
% :: SCCS/s.%
%.out :: %
%.c :: %
%.tex :: %


ifeq ($(OBJTOP),.)
  LIBDIR := lib
else
  LIBDIR := $(OBJTOP_ABS)/lib
endif

# squash any output from noisy environments, we just want the pwd output
LIBDIR_ABS    := $(shell mkdir -p $(LIBDIR) > /dev/null; cd $(LIBDIR)\
	           > /dev/null; pwd) 

SCIRUN_LIBDIR := $(LIBDIR)

# squash any output from noisy environments, we just want the pwd output
SCIRUN_LIBDIR_ABS := $(shell cd $(SCIRUN_LIBDIR) > /dev/null; pwd)

VPATH := $(SRCTOP)

# Optional pieces
HAVE_VISIT     := 
VISIT_INCLUDE  := 
VISIT_LIBRARY  :=  

# These cannot be :=
REFCOUNT_IMPL = $(SRCDIR)/RefCounted_gcc.cc

IS_OSX := no
IS_BGQ := 
MAKE_ARCHIVES := no
IS_DEBUG := no

# Set to 'yes' if Fortran is disabled:
NO_FORTRAN := yes

SCI_MALLOC_ON := no

NEED_OSX_SYSTEMSTUBS := 

LDRUN_PREFIX      := -Wl,-rpath -Wl,


# Libraries and other flags

M_LIBRARY :=  -lm

#ViSUS (nvisusio)
HAVE_VISUS := 
VISUS_LIBRARY :=  
VISUS_INCLUDE := 

#ViSUS (Parallel IDX)
HAVE_PIDX := 
PIDX_LIBRARY :=  
PIDX_INCLUDE := 

# MallocTrace
MALLOC_TRACE_INCLUDE = 
MALLOC_TRACE_LIBRARY =  

# Zoltan
ZOLTAN_INCLUDE = 
ZOLTAN_LIBRARY =  

# Boost
HAVE_BOOST    := no
BOOST_INCLUDE := 
BOOST_LIBRARY :=  

# SpatialOps - Wasatch 3P
HAVE_SPATIALOPS    := no
SPATIALOPS_INCLUDE := 
SPATIALOPS_LIBRARY :=  

# ExprLib - Wasatch 3P
HAVE_EXPRLIB    := no
EXPRLIB_INCLUDE := 
EXPRLIB_LIBRARY :=  

# TabProps - Wasatch 3P
HAVE_TABPROPS    := no
TABPROPS_INCLUDE =  
TABPROPS_LIBRARY =    $(BOOST_LIBRARY)

# RadProps - Wasatch 3P
HAVE_RADPROPS    := no
RADPROPS_INCLUDE =  
RADPROPS_LIBRARY =    $(BOOST_LIBRARY)

# PoKiTT - Wasatch 3P Library wrapping Cantera
HAVE_POKITT    := no
POKITT_INCLUDE := 
POKITT_LIBRARY :=  

# NSCBC - Wasatch 3P
HAVE_NSCBC    := no
NSCBC_INCLUDE =  

#############################################################################

HAVE_BLAS      := yes
HAVE_CBLAS     := yes
BLAS_LIBRARY   :=  -lblas
BLAS_INCLUDE   := 
HAVE_ACCELERATE	:= no

# Note: If we are not on an SGI, then LAPACKMP will be the same as
# LAPACK!
HAVE_LAPACK    := yes
HAVE_LAPACKMP  := yes
LAPACK_LIBRARY :=  -llapack
LAPACKMP_LIBRARY :=  -llapack

ifeq ($(IS_BGQ),yes)
  CFLAGS   := $(CFLAGS) -DPETSC_RESTRICT=__restrict__ -DPETSC_DEPRECATED\(why\)= 
  CXXFLAGS := $(CXXFLAGS) -DPETSC_RESTRICT=__restrict__ -DPETSC_DEPRECATED\(why\)= 
endif

GPERFTOOLS_INCLUDE     := 
GPERFTOOLS_LIBRARY     :=  

HAVE_KOKKOS        := 
KOKKOS_INCLUDE     := 
KOKKOS_LIBRARY     :=  

HAVE_CUDA        := 
CUDA_INCLUDE     := 
CUDA_LIBRARY     :=  

KOKKOS_USING_GPU := 

MPI_INCLUDE := -I/usr/lib/x86_64-linux-gnu/openmpi/include
MPI_LIBRARY := -Wl,-rpath -Wl,/usr/lib/x86_64-linux-gnu/openmpi/lib -L/usr/lib/x86_64-linux-gnu/openmpi/lib  -lmpi -lrt 

# not blank.  Its "value" is not useful.
HAVE_MPI := yes

HAVE_TIFF := no
TIFF_LIBRARY :=  

XALAN_PATH := 

PERL := /usr/bin/perl
SED := sed

MAKE_PARALLELISM := 8

LARGESOS := no

ifeq ($(LARGESOS),yes)
  MALLOCLIB := Core
else
  MALLOCLIB := Core/Malloc
endif

# Convenience variables - compounded from above definitions

DL_LIBRARY :=  -lc
Z_LIBRARY :=  -lz 

XML2_LIBRARY := $(Z_LIBRARY)  -lxml2
XML2_INCLUDE := -I/usr/include/libxml2

HAVE_HYPRE     := no
HYPRE_INCLUDE  := 
HYPRE_LIBRARY  :=  

HAVE_PETSC     := 
PETSC_INCLUDE  := 
PETSC_LIBRARY  :=  

INCLUDES += -I$(SRCTOP)/include -I$(SRCTOP) -I$(OBJTOP) -I$(OBJTOP)/include $(XML2_INCLUDE) $(MPI_INCLUDE) $(GPERFTOOLS_INCLUDE) $(KOKKOS_INCLUDE) $(CUDA_INCLUDE)  $(ZOLTAN_INCLUDE) $(TABPROPS_INCLUDE) ${MALLOC_TRACE_INCLUDE}
FINCLUDES += -I$(SRCTOP) -I$(OBJTOP) -I$(OBJTOP)/include

BUILD_ARCHES=no
BUILD_EXAMPLES=no
BUILD_HEAT=@BUILD_HEAT@
BUILD_ICE=no
BUILD_FVM=no
BUILD_MODELS_RADIATION=no
BUILD_MPM=yes
BUILD_PHASEFIELD=no
BUILD_WASATCH=no

BUILD_UNIT_TESTS=no

BUILD_MINIMAL=no

# Subdirectories
SUBDIRS := Core CCA VisIt

ifneq ($(BUILD_MINIMAL),yes)
SUBDIRS += StandAlone tools
endif

ifeq ($(BUILD_UNIT_TESTS),yes)
SUBDIRS += testprograms
endif

################################################################
# Variables and suffix rules for module transformation:
#

CC              := gcc
CXX             := g++
NVCC            := 
AS              := as
LD              := ld
CFLAGS          := -fPIC  -Wall -O2 -DNDEBUG  $(CFLAGS) 
CXXFLAGS        := -fPIC  -Wall -O2 -DNDEBUG  $(CXXFLAGS) 
NVCC_CFLAGS     :=  
NVCC_CXXFLAGS   :=  
SOFLAGS         := $(CFLAGS) -shared  -L$(LIBDIR)
LDFLAGS         := $(CXXFLAGS)  -L$(LIBDIR)
NVCC_LDFLAGS    := $(NVCC_CXXFLAGS) 
ASFLAGS         :=   $(ASFLAGS)
F77             := 
FFLAGS          := fortran_disabled $(FFLAGS)

# Fortran Library:
F_LIBRARY	:=   -lrt
REPOSITORY_FLAGS := 
OBJEXT := o

################################################################
# When building a static executable, these are the basic PSE libraries that are needed.  (Order matters!)
# Note, for some of the Uintah executables (most importantly, for 'sus') many more libraries are needed
# and these libraries are listed in the individual sub.mk file for that executable.
#
CORE_STATIC_PSELIBS := \
    Core_DataArchive Core_Grid Core_ProblemSpec Core_GeometryPiece CCA_Components_ProblemSpecification CCA_Ports \
    Core_Parallel Core_Math Core_Disclosure Core_Util Core_Exceptions Core_Containers \
    Core_Malloc Core_IO Core_OS                             
CORE_STATIC_LIBS := \
    $(XML2_LIBRARY) $(Z_LIBRARY) $(PETSC_LIBRARY) $(HYPRE_LIBRARY) $(LAPACK_LIBRARY) \
    $(BLAS_LIBRARY) $(F_LIBRARY) $(MPI_LIBRARY) $(X_LIBRARY) $(M_LIBRARY) \
    $(DL_LIBRARY) $(CUDA_LIBRARY) $(GPERFTOOLS_LIBRARY)

################################################################
# Auto dependency generation flags
#

CC_DEPEND_MODE	    := normal
CC_DEPEND_REGEN      = -MD
CC_DEPEND_EXT       := d
F77_DEPEND_MODE      = normal
F77_DEPEND_REGEN     = -MD
F77_DEPEND_EXT	    := d

NEED_SONAME  := yes

# Echo (with no new line)
ECHO_NNL     := echo -n

ifeq ($(REPOSITORY_FLAGS),)
  repository = 
else
  repository = $(REPOSITORY_FLAGS) $(patsubst %:$(1),%,$(filter %:$(1),$(ALL_LIB_ASSOCIATIONS)))
endif

ifeq ($(NEED_SONAME),yes)
  SONAMEFLAG = -Wl,-soname,$(notdir $@)
else
  SONAMEFLAG = 
endif

.SUFFIXES: .cc .cu .c .$(OBJEXT) .s .F .f .cpp .cxx

.cc.$(OBJEXT) .cpp.$(OBJEXT) : $<
ifeq ($(SCI_MAKE_BE_QUIET),true)
	@-rm -f $@
else
	-rm -f $@
endif
ifeq ($(CC_DEPEND_MODE),normal)
  ifeq ($(SCI_MAKE_BE_QUIET),true)
	  @echo "Compiling: $<"
	  @$(CXX) $(CXXFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) $(call repository,$@) -c $< -o $@
  else
	  $(CXX) $(CXXFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) $(call repository,$@) -c $< -o $@
  endif
else
  ifeq ($(CC_DEPEND_MODE),modify)
    ifeq ($(SCI_MAKE_BE_QUIET),true)
	    @echo "Compiling: $<"
	    @$(CXX) $(CXXFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) $(call repository,$@) -c $< -o $@ && $(SED) -e 's,^$(notdir $@),$@,' $(basename $@).$(CC_DEPEND_EXT) > $(basename $@).$(CC_DEPEND_EXT).tmp && mv -f $(basename $@).$(CC_DEPEND_EXT).tmp $(basename $@).$(CC_DEPEND_EXT)
    else
	    $(CXX) $(CXXFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) $(call repository,$@) -c $< -o $@ && $(SED) -e 's,^$(notdir $@),$@,' $(basename $@).$(CC_DEPEND_EXT) > $(basename $@).$(CC_DEPEND_EXT).tmp && mv -f $(basename $@).$(CC_DEPEND_EXT).tmp $(basename $@).$(CC_DEPEND_EXT)
    endif
  else
    ifeq ($(CC_DEPEND_MODE),move)
      ifeq ($(SCI_MAKE_BE_QUIET),true)
	      @echo "Compiling: $<"
	      @$(CXX) $(CXXFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) $(call repository,$@) -c $< -o $@
	      @sed 's,^$(basename $@),$@,g' $(basename $(notdir $@)).$(CC_DEPEND_EXT) > $(basename $@).$(CC_DEPEND_EXT)
	      @rm $(basename $(notdir $@)).$(CC_DEPEND_EXT)
      else
	      $(CXX) $(CXXFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) $(call repository,$@) -c $< -o $@
	      sed 's,^$(basename $@),$@,g' $(basename $(notdir $@)).$(CC_DEPEND_EXT) > $(basename $@).$(CC_DEPEND_EXT)
	      rm $(basename $(notdir $@)).$(CC_DEPEND_EXT)
      endif
    else
      ifeq ($(CC_DEPEND_MODE),modify_and_move)
        ifeq ($(SCI_MAKE_BE_QUIET),true)
	        @echo "Compiling: $<"
	        @$(CXX) $(CXXFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) $(call repository,$@) -c $< -o $@ && \
          $(SED) -e 's,^$(notdir $@),$@,' $(basename $(notdir $@)).$(CC_DEPEND_EXT) > $(basename $@).$(CC_DEPEND_EXT)
	        @rm $(basename $(notdir $@)).$(CC_DEPEND_EXT)
        else
	        $(CXX) $(CXXFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) $(call repository,$@) -c $< -o $@ && \
          $(SED) -e 's,^$(notdir $@),$@,' $(basename $(notdir $@)).$(CC_DEPEND_EXT) > $(basename $@).$(CC_DEPEND_EXT)
	        rm $(basename $(notdir $@)).$(CC_DEPEND_EXT)
        endif
      else
        ifeq ($(CC_DEPEND_MODE),stdout)
          #
          # .d - most likely gcc... have to explicitly put it in a .d file.
          #
          # -M outputs to stdout(err?) dependency info.  Redirect this output
          # to a temp file.  Prepend that file to replace filename.o with:
          # <path_to_file>/filename.o.  Then remove the temp file.
          #
          ifeq ($(SCI_MAKE_BE_QUIET),true)
	          @echo "Compiling: $<"
	          @rm -f $(basename $@).$(CC_DEPEND_EXT)
	          @$(CXX) $(CXXFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) -c $< -o $@ | sed 's,^$(notdir $@),$@,g' > $(basename $@).$(CC_DEPEND_EXT)

            # Now do the actual compile!
            #     @$(CXX) $(CXXFLAGS) $(INCLUDES) $(call repository,$@) -c $< -o $@
          else
	          @echo "Compiling and Creating dependency information (stdout):"
	          @rm -f $(basename $@).$(CC_DEPEND_EXT)
	          $(CXX) $(CXXFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) -c $< -o $@ | sed 's,^$(notdir $@),$@,g' > $(basename $@).$(CC_DEPEND_EXT)

            # Now do the actual compile!
            #     @echo "ACTUALLY COMPILING:"
            #     $(CXX) $(CXXFLAGS) $(INCLUDES) $(call repository,$@) -c $< -o $@
          endif
        else
	        @echo ".cc rule: Unknown CC_DEPEND_MODE: $(CC_DEPEND_MODE)"
	         @exit 1
        endif
      endif
    endif
  endif
endif

.cu.$(OBJEXT): $<
ifeq ($(SCI_MAKE_BE_QUIET),true)
	@-rm -f $@
else
	-rm -f $@
endif
#
# .d - most likely gcc... have to explicitly put it in a .d file.
#
# -M outputs to stdout(err?) dependency info.
# Prepend that file to replace filename.o with:
#     <path_to_file>/filename.o.
# Note: For the CUDA compiler (nvcc), it is necessary to generate
#       dependencies after the compilation phase.
#
ifeq ($(SCI_MAKE_BE_QUIET),true)
	@echo "Compiling CUDA file: $<"
	@$(NVCC) $(NVCC_CXXFLAGS) $(INCLUDES) -Xcompiler $(CC_DEPEND_REGEN) $(call repository,$@) -c $< -o $@
        # FIXME: this should only be done on Titan... probably
	@rm -f *.d
	@$(NVCC) $(NVCC_CXXFLAGS) $(INCLUDES) -M $< | sed 's,^$(notdir $@),$@,g' > $(basename $@).$(CC_DEPEND_EXT)
else
	$(NVCC) $(NVCC_CXXFLAGS) $(INCLUDES) -Xcompiler $(CC_DEPEND_REGEN) $(call repository,$@) -c $< -o $@
        # On Titan, remove temporary .d files from the top level bin directory.
        #   FIXME: perhaps should verify that we are on Titan? Though it should not hurt anything...
	rm -f *.d 
	$(NVCC) $(NVCC_CXXFLAGS) $(INCLUDES) -M $< | sed 's,^$(notdir $@),$@,g' > $(basename $@).$(CC_DEPEND_EXT)
endif

.F.$(OBJEXT): $<
ifeq ($(SCI_MAKE_BE_QUIET),true)
	@-rm -f $@
else
	-rm -f $@
endif
ifeq ($(F77_DEPEND_MODE),normal)
  ifeq ($(SCI_MAKE_BE_QUIET),true)
	@echo "Fortran:   $<" 
	@$(F77) $(FFLAGS) $(FINCLUDES) $(F77_DEPEND_REGEN) -c $< -o $@
  else
	$(F77) $(FFLAGS) $(FINCLUDES) $(F77_DEPEND_REGEN) -c $< -o $@
  endif
else
  ifeq ($(F77_DEPEND_MODE),modify)
	$(F77) $(FFLAGS) $(FINCLUDES) $(F77_DEPEND_REGEN) -c $< -o $@ && if test -f $(basename $@).$(F77_DEPEND_EXT); then $(SED) -e 's,^$(notdir $@),$@,' $(basename $@).$(F77_DEPEND_EXT) > $(basename $@).$(F77_DEPEND_EXT).tmp && mv -f $(basename $@).$(F77_DEPEND_EXT).tmp $(basename $@).$(F77_DEPEND_EXT); fi
  else
    ifeq ($(F77_DEPEND_MODE),move)
	$(F77) $(FFLAGS) $(FINCLUDES) $(F77_DEPEND_REGEN) -c $< -o $@
	sed 's,^$(basename $@),$@,g' $(basename $(notdir $@)).$(F77_DEPEND_EXT) > $(basename $@).$(F77_DEPEND_EXT)
	rm $(basename $(notdir $@)).$(F77_DEPEND_EXT)
    else
      ifeq ($(F77_DEPEND_MODE),modify_and_move)
	$(F77) $(FFLAGS) $(FINCLUDES) $(F77_DEPEND_REGEN) -c -o $@ $< && \
             $(SED) -e 's,^$(notdir $@),$@,' $(basename $(notdir $@)).$(F77_DEPEND_EXT) > $(basename $@).$(F77_DEPEND_EXT)
	@rm $(basename $(notdir $@)).$(F77_DEPEND_EXT)
      else
        ifeq ($(F77_DEPEND_MODE),stdout)
          #
          # .d - most likely gcc... have to explicitly put it in a .d file.
          #
          # -M outputs to stdout(err?) dependency info.  Redirect this output
          # to a temp file.  Awk that file to replace filename.o with:
          # <path_to_file>/filename.o.  Then remove the temp file.
          #
	  @rm -f $(basename $@).$(F77_DEPEND_EXT)
          ifeq ($(SCI_MAKE_BE_QUIET),true)
            # This just generates the .d dependency file:
	    @$(F77) $(FFLAGS) $(FINCLUDES) $(F77_DEPEND_REGEN) -c $< -o $@ | sed 's,^$(notdir $@),$@,g' > $(basename $@).$(F77_DEPEND_EXT)

            # The following if/grep/sed is necessary to fix the .d file for the PGI compiler on Ranger:
	    @if `grep -q "^../src/" $(basename $@).$(F77_DEPEND_EXT)`; then sed 's,^../src/,,' $(basename $@).$(F77_DEPEND_EXT) > $(basename $@).$(F77_DEPEND_EXT).tmp; mv $(basename $@).$(F77_DEPEND_EXT).tmp $(basename $@).$(F77_DEPEND_EXT) ; fi

            # Now do the actual compile!
            #@echo "Fortran:   $<"
            #@$(F77) $(FFLAGS) $(INCLUDES) -c $< -o $@
          else
	    @echo "CREATING DEPENDENCY INFORMATION:"
	    $(F77) $(FFLAGS) $(FINCLUDES) $(F77_DEPEND_REGEN) -c $< -o $@ | sed 's,^$(notdir $@),$@,g' > $(basename $@).$(F77_DEPEND_EXT)

            # The following if/grep/sed is necessary for PGI compiler on Ranger:
	    @if `grep -q "^../src/" $(basename $@).$(F77_DEPEND_EXT)`; then sed 's,^../src/,,' $(basename $@).$(F77_DEPEND_EXT) > $(basename $@).$(F77_DEPEND_EXT).tmp; mv $(basename $@).$(F77_DEPEND_EXT).tmp $(basename $@).$(F77_DEPEND_EXT) ; fi

            # Now do the actual compile!
            #@echo "ACTUALLY COMPILING:"
            #$(F77) $(FFLAGS) $(INCLUDES) -c $< -o $@
          endif
        else
	  @echo "Unknown F77_DEPEND_MODE: $(F77_DEPEND_MODE) (for $@)"
	  @exit 1
        endif
      endif
    endif
  endif
endif

.f.$(OBJEXT): $<
	-rm -f $@
ifeq ($(F77_DEPEND_MODE),normal)
	$(F77) $(FFLAGS) $(FINCLUDES) $(F77_DEPEND_REGEN) -c $< -o $@
else
  ifeq ($(F77_DEPEND_MODE),modify)
	$(F77) $(FFLAGS) $(FINCLUDES) $(F77_DEPEND_REGEN) -c $< -o $@ && if test -f $(basename $@).$(F77_DEPEND_EXT); then $(SED) -e 's,^$(notdir $@),$@,' $(basename $@).$(F77_DEPEND_EXT) > $(basename $@).$(F77_DEPEND_EXT).tmp && mv -f $(basename $@).$(F77_DEPEND_EXT).tmp $(basename $@).$(F77_DEPEND_EXT); fi
  else
    ifeq ($(F77_DEPEND_MODE),move)
	$(F77) $(FFLAGS) $(FINCLUDES) $(F77_DEPEND_REGEN) -c $< -o $@
	sed 's,^$(basename $@),$@,g' $(basename $(notdir $@)).$(F77_DEPEND_EXT) > $(basename $@).$(F77_DEPEND_EXT)
	rm $(basename $(notdir $@)).$(F77_DEPEND_EXT)
    else
      ifeq ($(F77_DEPEND_MODE),stdout)
        #
        # .d - most likely gcc... have to explicitly put it in a .d file.
        #
        # -M outputs to stdout(err?) dependency info.  Redirect this output
        # to a temp file.  Awk that file to replace filename.o with:
        # <path_to_file>/filename.o.  Then remove the temp file.
        #
	@echo "CREATING DEPENDENCY INFORMATION:"
	@rm -f $(basename $@).$(F77_DEPEND_EXT)
	$(F77) $(FFLAGS) $(FINCLUDES) $(F77_DEPEND_REGEN) -c $< | sed 's,^$(notdir $@),$@,g' > $(basename $@).$(F77_DEPEND_EXT)

        # Now do the actual compile!
	@echo "ACTUALLY COMPILING:"
	$(F77) $(FFLAGS) $(INCLUDES) -c $< -o $@
      else
	@$echo "Unknown F77_DEPEND_MODE: $(F77_DEPEND_MODE) (for $@)"
	@exit 1
      endif
    endif
  endif
endif

.c.$(OBJEXT): $<
ifeq ($(SCI_MAKE_BE_QUIET),true)
	@-rm -f $@
else
	-rm -f $@
endif
ifeq ($(CC_DEPEND_MODE),normal)
  ifeq ($(SCI_MAKE_BE_QUIET),true)
	@echo "Compiling: $<"
	@$(CC) $(CFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) -c $< -o $@
  else
	$(CC) $(CFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) -c $< -o $@
  endif
else
  ifeq ($(CC_DEPEND_MODE),modify)
	$(CC) $(CFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) -c $< -o $@ && $(SED) -e 's,^$(notdir $@),$@,' $(basename $@).$(CC_DEPEND_EXT) > $(basename $@).$(CC_DEPEND_EXT).tmp && mv -f $(basename $@).$(CC_DEPEND_EXT).tmp $(basename $@).$(CC_DEPEND_EXT)
  else
    ifeq ($(CC_DEPEND_MODE),move)
	$(CC) $(CFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) -c $< -o $@
	sed 's,^$(basename $@),$@,g' $(basename $(notdir $@)).$(CC_DEPEND_EXT) > $(basename $@).$(CC_DEPEND_EXT)
	rm $(basename $(notdir $@)).$(CC_DEPEND_EXT)
    else
      ifeq ($(CC_DEPEND_MODE),modify_and_move)
        ifeq ($(SCI_MAKE_BE_QUIET),true)
	  @echo "Compiling: $<"
	  @$(CC) $(CFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) $(call repository,$@) -c $< -o $@ && \
             $(SED) -e 's,^$(notdir $@),$@,' $(basename $(notdir $@)).$(CC_DEPEND_EXT) > $(basename $@).$(CC_DEPEND_EXT)
	  @rm $(basename $(notdir $@)).$(CC_DEPEND_EXT)
        else
	  $(CC) $(CFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) $(call repository,$@) -c $< -o $@ && \
             $(SED) -e 's,^$(notdir $@),$@,' $(basename $(notdir $@)).$(CC_DEPEND_EXT) > $(basename $@).$(CC_DEPEND_EXT)
	  rm $(basename $(notdir $@)).$(CC_DEPEND_EXT)
        endif
      else
        ifeq ($(CC_DEPEND_MODE),stdout)
          #
          # .d - most likely gcc... have to explicitly put it in a .d file.
          #
          # -M outputs to stdout(err?) dependency info.  Redirect this output
          # to a temp file.  Awk that file to replace filename.o with:
          # <path_to_file>/filename.o.  Then remove the temp file.
          #
	  @echo "CREATING DEPENDENCY INFORMATION:"
	  @rm -f $(basename $@).$(CC_DEPEND_EXT)
	  $(CC) $(CFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) -c $< | sed 's,^$(notdir $@),$@,g' > $(basename $@).$(CC_DEPEND_EXT)
	  @rm -f $(basename $@).depend

          # Now do the actual compile!
	  @echo "ACTUALLY COMPILING:"
	  $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
        else
	  @echo ".c rule: Unknown CC_DEPEND_MODE: $(CC_DEPEND_MODE)"
	  @exit 1
        endif
      endif
    endif
  endif
endif

.s.$(OBJEXT): $<
	$(AS) $(ASFLAGS) -o $@ $< -o $@

.cxx.o: $<
ifeq ($(SCI_MAKE_BE_QUIET),true)
	@-rm -f $@
else
	-rm -f $@
endif
ifeq ($(CC_DEPEND_MODE),normal)
  ifeq ($(SCI_MAKE_BE_QUIET),true)
	@echo "Compiling: $<"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) $(call repository,$@) -c $< -o $@ ;
  else
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) $(call repository,$@) -c $< -o $@ ;
  endif
else
  ifeq ($(CC_DEPEND_MODE),modify)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) $(call repository,$@) -c $< -o $@ && $(SED) -e 's,^$(notdir $@),$@,' $(basename $@).$(CC_DEPEND_EXT) > $(basename $@).$(CC_DEPEND_EXT).tmp && mv -f $(basename $@).$(CC_DEPEND_EXT).tmp $(basename $@).$(CC_DEPEND_EXT)
  else
    ifeq ($(CC_DEPEND_MODE),move)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) $(call repository,$@) -c $< -o $@
	sed 's,^$(basename $@),$@,g' $(basename $(notdir $@)).$(CC_DEPEND_EXT) > $(basename $@).$(CC_DEPEND_EXT)
	rm $(basename $(notdir $@)).$(CC_DEPEND_EXT)
    else
      ifeq ($(CC_DEPEND_MODE),stdout)
        #
        # .d - most likely gcc... have to explicitly put it in a .d file.
        #
        # -M outputs to stdout(err?) dependency info.  Redirect this output
        # to a temp file.  Prepend that file to replace filename.o with:
        # <path_to_file>/filename.o.  Then remove the temp file.
        #
	@echo "CREATING DEPENDENCY INFORMATION:"
	@rm -f $(basename $@).$(CC_DEPEND_EXT)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(CC_DEPEND_REGEN) -c $< | sed 's,^$(notdir $@),$@,g' > $(basename $@).$(CC_DEPEND_EXT)

        # Now do the actual compile!
	@echo "ACTUALLY COMPILING:"
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(call repository,$@) -c $< -o $@
      else
	@echo ".cxx rule: Unknown CC_DEPEND_MODE: $(CC_DEPEND_MODE)"
	@exit 1
      endif
    endif
  endif
endif
