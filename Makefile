#  Makefile for Mathtools
#
#  $Id: Makefile,v 1.2 1997/09/14 00:09:44 jak Exp $
#
#  Copyright (C) 1993, John A. Kassebaum
#
#  This software was written by John Kassebaum as a part of his graduate
#  course work at Purdue.  He retains all rights to this software.  
#
#

# Architecture Specific Definitions
ARCH=NeXT
include ./config/$(ARCH).def

INSTALLDIR=/usr/local

INCLUDE_DIR=$(INSTALLDIR)/include
LIB_DIR=$(INSTALLDIR)/lib
EXE_DIR=$(INSTALLDIR)/bin

CFLAGS=$(ARCHDEFS) -I/usr/local/lib/g++-include  -I/usr/local/include -I..
LIBS=-L/usr/local/lib -L. -lMathTools -lg++ -lgmp -lstdc++

TEMPLATEFILES=Matrix.h LU_Decomposition.h Cholesky.h MatrixComposition.h \
Range.h LinkedList.h IntArray.h MP_Float.h MP_Int.h MP_Ratio.h

OBJECTS=Matrix.o LU_Decomposition.o Cholesky.o MatrixComposition.o Range.o \
LinkedList.o IntArray.o MP_Float.o MP_Int.o MP_Ratio.o

LIBRARIES=libMathTools.a

EXES=
TESTEXES=doubletest

# Dependancy Files
DEPENDEXE= $(addsuffix .d,$(EXES) $(TESTEXES))
DEPENDOBJ= $(OBJECTS:.o=.d)

.PHONY: thisarch all install install-arch depend clean count

thisarch:
	-mkdir $(ARCH)
	-cd $(ARCH); ln -s ../config .
	cd $(ARCH); $(MAKE) -f ../Makefile -f ../DependRules.def all
	
all: depend $(OBJECTS) $(LIBRARIES) $(EXES) $(TESTEXES)

%.o:../%.cc
	$(CC) -c $(CFLAGS) -fexternal-templates $< -o $@

MP_Float.o MP_Int.o MP_Ratio.o: %.o:../%.cc
	$(CC) -c $(CFLAGS) $< -o $@

%:../%.cc $(LIBRARIES)
	$(CC) $(CFLAGS) -fexternal-templates $< $(LIBS) -o $@

libMathTools.a: $(OBJECTS)
	$(AR) -o $@  $(OBJECTS) 

install-arch:
	-mkdir $(LIB_DIR)
	cd ..; cp $(TEMPLATEFILES) $(INCLUDE_DIR)
	cp -p $(LIBRARIES) $(LIB_DIR)
#	cp -p $(EXES) $(EXE_DIR)

install:
	-mkdir $(LIB_DIR)
	cp $(TEMPLATEFILES) $(INCLUDE_DIR)
	cd $(ARCH); cp -p $(LIBRARIES) $(LIB_DIR)

deinstall:
	cd $(INCLUDE_DIR); rm -f $(TEMPLATEFILES)
	cd $(LIB_DIR); rm -f $(LIBRARIES)

tarfile:
	-rm -rf MathTools.tar.gz
	mkdir MathTools
	cp Makefile DependRules.def *.cc *.h MathTools
	tar cvf - MathTools | gzip -c > MathTools.tar.gz
	rm -rf MathTools

clean:
	rm -rf $(ARCH)

$(DEPENDOBJ): %.d: ../%.cc
	$(SHELL) -ec '$(CC) -MM -c $(CFLAGS) $< | sed '\''s/$*\.o/& $@/g'\'' > $@'

$(DEPENDEXE): %.d: ../%.cc
	$(SHELL) -ec '$(CC) -MM -c $(CFLAGS) $< | sed '\''s/$*\.o/$* $@/g'\'' > $@'

depend: $(DEPENDOBJ) $(DEPENDEXE)

# History:
#   $Log: Makefile,v $
#   Revision 1.2  1997/09/14 00:09:44  jak
#   Added deinstall and library paths. -jak
#
#   Revision 1.1.1.1  1997/09/13 23:38:41  jak
#   Restored MathTools to new CVS repository. -jak
#
#

