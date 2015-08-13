include ../makefile.inc

TARGET=lastchaos_Authentication

LC_SRCS=$(wildcard *.cpp)
LC_OBJS=$(patsubst %.cpp,%.o,$(LC_SRCS))

all : $(TARGET)

$(TARGET): $(LC_OBJS)
	make -j32 -C ../ShareLib kor
	$(CXX)  -o $@ $(LC_OBJS) $(OPT_LDOBJ)

.cpp .o:
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) -o $@ $(patsubst %.o,%.cpp,$@)

cleanall:
	make -C ../ShareLib cleanall
	find . -name "*.o" -exec rm -rf {} \;
	rm -rf $(TARGET)

ifeq (.depend,$(wildcard .depend))
include .depend
endif
