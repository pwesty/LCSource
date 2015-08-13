.SUFFIXES : .cpp .o
.PHONY: clean dep new cleanall local debug clear test

CXX = ccache g++ -g

BOOST_INCLUDE = /usr/local/include/boost-1_57
BOOST_LIB_VER = -gcc44-mt-d-1_57

OPT_CRYPT = -I../ShareLib \
		-I/usr/include/mysql \
		-I/usr/local/ssl/include \
		-I/usr/local/include/botan-1.10 \
		-I$(BOOST_INCLUDE)
		
OPT_LDOBJ = -L../ShareLib \
			-L/usr/local/lib \
			-L/usr/local/apr/lib \
			-L/usr/lib/mysql \
			-lSharelib \
			-lmysqlclient \
			-lboost_thread$(BOOST_LIB_VER) \
			-lboost_system$(BOOST_LIB_VER) \
			-llog4cxx \
			-laprutil-1 \
			-lapr-1 \
			-lrt \
			-lstdc++ \
			-lexpat \
			-lz \
			-lpthread \
			-ljsoncpp
			
OPT_VER=-DLC_VER=\"$(LC_VER)\"

CXXFLAGS =  -D_THREAD_SAFE -finline-functions \
			-funroll-loops \
			-fdefer-pop \
			-fstrength-reduce \
			-Wno-write-strings \
			-Wno-char-subscripts \
			$(OPT_BIGENDIAN) \
			$(OPT_LANG) \
			$(OPT_DEBUG) \
			$(OPT_DEF) \
			$(OPT_TEST) \
			$(OPT_VER) \
			$(OPT_CRYPT)
