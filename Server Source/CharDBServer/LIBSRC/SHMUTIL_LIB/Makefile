
SHELL = /bin/sh

CC = cc
AR = ar 
RM = /bin/rm
DEPEND=makedepend
LINT = lint

TOP_BIN = 

INCLUDES =  -I../../INC -I./ -I../../LIB 
DEFINES  = 
OPTIMIZE = -O -g3
DEBUG =

CFLAGS = $(OPTIMIZE) $(INCLUDES) $(DEFINES) $(DEBUG)
SFLAGS = $(INCLUDES) $(DEFINES) $(DEBUG)
DEPOPTS = $(INCLUDES) $(DEFINES) 

HDRS = 

SRCS = shmutil.c 

TARGET = libSHMUTIL.a 

OBJSV = $(SRCS:.c=s.o)
OBJCL = $(SRCS:.c=c.o)

#
#-----------------------------------------------------------
#

#
.SUFFIXES: .c .s s.o c.o p.o q.o .i .u .H
.s.o:
	$(CC) $(CFLAGS) -c $<
.c.s:
	$(CC) $(SFLAGS) -S $<
.cs.o:
	$(CC) -o $@ $(CFLAGS) -c $<
.cc.o:
	$(CC) -o $@ $(CFLAGS) -c $<
.c.u:
	$(CC) $(CFLAGS) -j $<
.c.i:
	$(CC) $(CFLAGS) -P $<

all: $(TARGET) $(HADS)


libSHMUTIL.a: $(SRCS) $(HDRS) $(OBJSV) 
	$(RM) -f $@
	$(AR) rcu $@ $(OBJSV)
	$(AR) ts $@
	cp $@ ../../LIB

clean:
	$(RM) -f *.s *.u *.o *.i core $(TARGET)

depend: $(SRCS)
	$(DEPEND) -o.o -f Makefile -- $(DEPOPTS) -- $(SRCS)

lint:
	$(LINT) $(INCLUDES) -h -v -a -u -x -m $(SRCS)

# DO NOT DELETE THIS LINE -- make depend depends on it.
