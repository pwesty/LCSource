
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

SRCS = hashshm.c 
SRCS4B = hashshm_4b.c 

TARGET = libhashshm.a 
TARGET4B = libhashshm4b.a 

OBJSV = $(SRCS:.c=s.o)
OBJCL = $(SRCS:.c=c.o)

OBJSV4B = $(SRCS4B:.c=s.o)
OBJCL4B = $(SRCS4B:.c=c.o)

SAMPLESRCS = sample.c
SAMPLEOBJS = $(SAMPLESRCS:.c=s.o)

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

all: $(TARGET) $(TARGET4B) $(HADS)

$(TARGET): $(SRCS) $(HDRS) $(OBJSV) 
	$(RM) -f $@
	$(AR) rcu $@ $(OBJSV)
	$(AR) ts $@
	cp $(TARGET) ../../LIB

$(TARGET4B): $(SRCS4B) $(HDRS) $(OBJSV4B) 
	$(RM) -f $@
	$(AR) rcu $@ $(OBJSV4B)
	$(AR) ts $@
	cp $(TARGET4B) ../../LIB

install:
	cp $(TARGET) ../../LIB

clean:
	$(RM) -f *.s *.u *.o *.i core $(TARGET) $(TARGET4B) sample

depend: $(SRCS)
	$(DEPEND) -o.o -f Makefile -- $(DEPOPTS) -- $(SRCS)

lint:
	$(LINT) $(INCLUDES) -h -v -a -u -x -m $(SRCS)

sample: $(TARGET) $(TARGET4B) $(SAMPLESRCS) $(SAMPLEOBJS)
	$(CC) $(CFLAGS) -o $@ $(SAMPLEOBJS) ./$(TARGET) ./$(TARGET4B)

# DO NOT DELETE THIS LINE -- make depend depends on it.
