#--------------------- Makefile which have no SubDir ---------------------
SHELL  		= /bin/sh
SRCDIR 		= .
DSTDIR 		= ../../LIB

CC = gcc

TARGET		= all

SUBDIRS		= 

MAKEDEPEND 	= makedepend

STD_INCDIR 	= 
STD_LIBDIR	=

LOC_INCDIR	= ../../INC 
LOC_LIBDIR	=

GBL_INCDIR	=
GBL_LIBDIR	=

INC_DIRS	= -I$(LOC_INCDIR)
LIB_DIRS	=

#-------------------- Source & Objects & Libraries------------------------
PROGRAMS 	= libLogUtil.a
LIBS 		= 

CFLAGS 		= $(OPTIMIZE) $(INC_DIRS) $(DEFINES) $(DEBUG)
CPPFLAGS	= 
LDOPTIONS	=

DEFINES 	=
OPTIMIZE	= -O
ifeq ($(DTAS_DEBUG),)
DEBUG		= -g3 -DLOGDEBUG -Wall
else
DEBUG		= $(DTAS_DEBUG)
endif

LIBRARIES	=

DEPLIBS1	= 

SRCS1		= lwrite.c
OBJS1		= $(SRCS1:.c=.o)
HDRS1		= 


DEPLIBS2	= 
SRCS2		= 
OBJS2		=
HDRS2		=

#-------------------------------------------------------------------------

all:: $(PROGRAMS) install

libLogUtil.a: $(OBJS1) $(HDRS1)
	ar rcu $@ $(OBJS1)

clean::
	rm -f $(PROGRAMS) $(OBJS1) $(OBJS2)

install:: $(PROGRAMS) 
	@for i in $(PROGRAMS); do \
		echo Installing $$i in $(DSTDIR); \
		cp $$i $(DSTDIR); \
	done
