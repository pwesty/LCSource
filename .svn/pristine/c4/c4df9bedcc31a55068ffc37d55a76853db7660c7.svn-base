#--------------------- Makefile which have no SubDir ---------------------
SHELL  		= /bin/sh
SRCDIR 		= .

CC 			= cc

TARGET		= all

SUBDIRS		= 

INSTALL 	= cp
MAKEDEPEND 	= makedepend

LOC_INCDIR	= ../../INC
MY_INCDIR	= ../../LIB

LOC_LIBDIR	= ../../LIB
MY_LIBDIR	= ../../LIB


INC_DIRS	= -I. -I$(LOC_INCDIR)
LIB_DIRS	= -L$(LOC_LIBDIR)
LIBS       = $(LIB_PATH_ORACLE)  -lnsl -ldl
#OPTIMIZE    = -O -g -xCC

OPTIMIZE	= -Wall -g3

#-------------------- Source & Objects & Libraries------------------------
PROGRAMS	= StartMC
PROGRAMS2	= KillMC
PROGRAMS3	= DisMC

#DEFINES 	= -DDEBUG -DPRINT
#DEFINES 	= -DDEBUG -DLIBPRN
#DEFINES 	= -DDEBUG -DPRINT -DPERROR -DTEST
DEFINES 	= -DDEBUG -DLINUX

MFLAGS		= 
CFLAGS		= $(DEFINES) $(INC_DIRS) $(OPTIMIZE) 

LDFLAGS		=	
LDOPTIONS	=


LIBRARIES	= 

DEPLIBS1	= 

SRC_COMM	=	tifb_util_lnx.c

SOURCE		=	StartMC.c $(SRC_COMM)
SOURCE2		=	KillMC.c $(SRC_COMM)
SOURCE3		=	DisMC.c $(SRC_COMM)

OBJS		= $(SOURCE:.c=.o)
OBJS2		= $(SOURCE2:.c=.o)
OBJS3		= $(SOURCE3:.c=.o)


#-------------------------------------------------------------------------

all:: $(PROGRAMS) $(PROGRAMS2) $(PROGRAMS3) install
#all:: $(PROGRAMS3)

$(PROGRAMS): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LIB_DIRS) $(LDFLAGS) $(LIBS)

$(PROGRAMS2): $(OBJS2)
	$(CC) -o $@ $(OBJS2) $(CFLAGS) $(LIB_DIRS) $(LDFLAGS) $(LIBS)

$(PROGRAMS3): $(OBJS3)
	$(CC) -o $@ $(OBJS3) $(CFLAGS) $(LIB_DIRS) $(LDFLAGS) $(LIBS)

linux:
#	make "OPTIMIZE=-g3" "DEFINES=-DDEBUG -DLINUX -Wall" "SRC_COMM=tifb_util_lnx.c"
	make "OPTIMIZE=${OPTIMIZE2}" "DEFINES=${DEFINES2}" "SRC_COMM=${SRC_LNX}"

test: t.o
	$(CC) -o $@ t.o $(CFLAGS) $(LIB_DIRS) $(LDFLAGS) $(LIBS)
	

clean::
#	rm -f $(PROGRAMS) $(PROGRAMS2) $(PROGRAMS3) $(OBJS) $(OBJS2) $(OBJS3) core*
	rm -f $(PROGRAMS) $(PROGRAMS2) $(PROGRAMS3) *.o core*

install::
	$(INSTALL) $(PROGRAMS) ../../BIN
	$(INSTALL) $(PROGRAMS2) ../../BIN
	$(INSTALL) $(PROGRAMS3) ../../BIN
