#include ../../ora_common_lib.mk

SHELL		= /bin/sh
CC			= cc
AR			= ar
RM			= /bin/rm
MV			= mv
DEPEND		= makedepend
LINT		= lint


TOP_LIB		= ../../LIB
TOP_BIN		= ../../BIN
LIBRARY =   -L$(TOP_LIB)


INCLUDES   	= -L../../LIB -I. -I../../INC
OPTIMIZE    = -g -Wall
DEBUG       = -DDEBUG 

CFLAGS		= $(OPTIMIZE) $(INCLUDES) $(DEBUG) -g

SRCS		= chsmd_main.c chsmd_mmc.c df_com.c chsmd_init.c \
			 chsmd_fidb.c check_sw.c cpu_check.c mem_check.c link_check.c
				
TARGET		= CHSMD

OBJS		= $(SRCS:.c=.o)



LIBS    =  -lnsl -lLogUtil -lSHMUTIL


MAKEFILE	= Makefile

#
#-----------------------------------------------------------
#

#
.SUFFIXES: .c .s .o .i .u
.s.o:
	$(CC) $(CFLAGS) -c $<
.c.s:
	$(CC) $(SFLAGS) -S $<
.c.o:
	$(CC) $(CFLAGS) -c $<
.c.u:
	$(CC) $(CFLAGS) -j $<
.c.i:
	$(CC) $(CFLAGS) -P $<



all: $(TARGET)

#$(SRCS):
#	$(GET) -s $@

$(SRCS):
	c_sess_main.c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS) $(LIBRARY) $(LDFLAGS)
	cp $(TARGET) $(TOP_BIN)

lint:
	lint $(INCLUDES) -Xarch=v9 -errchk=longptr64 $(SRCS)

depend: $(SRCS)
	$(DEPEND) -o.o -f $(MAKEFILE) -- $(INCLUDES) -- $(SRCS)


clean:
	/bin/rm -f *.o $(TARGET) core 

new : clean $(SRCS)

# DO NOT DELETE THIS LINE -- make depend depends on it.
