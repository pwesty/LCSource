.SUFFIXES : .cpp .o
.PHONY: clean dep new cleanall local debug clear test

CXX=g++
ARBIN=ar

CXXFLAGS = -finline-functions -Wall -Werror

TARGET=libLCSha256.a

MAKEDEP=gccmakedep

LC_SRCS=$(wildcard *.cpp)
LC_OBJS=$(patsubst %.cpp,%.o,$(LC_SRCS))


all: $(TARGET)

dep: $(LC_SRCS)
	$(MAKEDEP) $(LC_SRCS)

$(TARGET): $(LC_OBJS)
	ar rs $(TARGET) $(LC_OBJS)
	chmod +x $(TARGET)

.cpp .o:
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) -o $@ $(patsubst %.o,%.cpp,$@)

clean:
	rm -rf $(LC_OBJS) $(TARGET)
