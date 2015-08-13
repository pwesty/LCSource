include ../makefile.inc

TARGET=libSharelib.a

LC_SRCS=$(wildcard *.cpp)
LC_OBJS=$(patsubst %.cpp,%.o,$(LC_SRCS))

all: kor

kor: svn_info
	OPT_LANG=-DLC_KOR=0 $(MAKE) $(TARGET)

kortest: svn_info
	OPT_LANG=-DLC_KOR=0 $(MAKE) $(TARGET)

usa: svn_info
	OPT_LANG=-DLC_USA=9 $(MAKE) $(TARGET)

brz: svn_info
	OPT_LANG=-DLC_BRZ=10 $(MAKE) $(TARGET)

mex: svn_info
	OPT_LANG=-DLC_MEX=20 $(MAKE) $(TARGET)
	
rus: svn_info
	OPT_LANG=-DLC_RUS=17 $(MAKE) $(TARGET)
	
tld: svn_info
	OPT_LANG=-DLC_TLD=4 $(MAKE) $(TARGET)
	
ger: svn_info
	OPT_LANG=-DLC_GER=13 $(MAKE) $(TARGET)
	
frc: svn_info
	OPT_LANG=-DLC_FRC=15 $(MAKE) $(TARGET)
	
pld: svn_info
	OPT_LANG=-DLC_PLD=16 $(MAKE) $(TARGET)
	
spn: svn_info
	OPT_LANG=-DLC_SPN=14 $(MAKE) $(TARGET)
	
ita: svn_info
	OPT_LANG=-DLC_ITA=19 $(MAKE) $(TARGET)

uk: svn_info
	OPT_LANG=-DLC_UK=24 $(MAKE) $(TARGET)
	
svn_info:
	@echo "Remake svn_info.h" 
	@echo "static const char* last_svn_revision=\""`export LANG=en_US && cd .. && svn info | grep Rev:`"\";" > svn_info.h
	@echo "static const int compile_time = `date +%s` ^ 0x12345678;" >> svn_info.h
		
dep: $(LC_SRCS)
	$(CXX) $(CXXFLAGS) -M $(LC_SRCS) > .depend

$(TARGET): $(LC_OBJS)
	ar rs $(TARGET) $(LC_OBJS)
	chmod +x $(TARGET)

.cpp .o:
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) -o $@ $(patsubst %.o,%.cpp,$@)

clean:
	rm -rf $(LC_OBJS) $(TARGET)

cleanall:
	find . -name "*.o" -exec rm -rf {} \;
	rm -rf $(TARGET)

ifeq (.depend,$(wildcard .depend))
include .depend
endif
