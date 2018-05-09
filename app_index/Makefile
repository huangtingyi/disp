#判断是否是debug编译条件,默认为release编译
ifeq ($(findstring debug,$(MAKECMDGOALS)),debug)
    CXXFLAGS=-g -W -Wall -Wno-unused-parameter  -std=c++0x
    TARGETPATH=../lib
else
#    CXXFLAGS=-O2 -W -Wall -Wno-unused-parameter -std=c++0x
	CXXFLAGS=-g -W -Wall -Wno-unused-parameter  -std=c++0x
    TARGETPATH=../lib
endif

#最终生成程序
TARGET_B=../bin/dmpi2txt
TARGET_D=../bin/index_gta
TARGET_E=../bin/dmpo2txt
TARGET_F=../bin/gta2tdf

#编译命令
CXX=g++

#头文件
INC= -I../Include -I../Common -I../public -I../IncludeTDF -I../comm_supp

#库
LIB=-pthread -lcurl -lboost_log -lboost_filesystem -lboost_thread -lboost_log_setup -lboost_system -lprotobuf -lrt



#工程文件
ExampleB_SRC=$(wildcard ./dmpi2txt.cpp ./dmpfmt.cpp ../public/wwtiny.cpp ../public/mktdata.pb.cpp ../public/d31data.pb.cpp)
ExampleB_OBJS=$(patsubst %.cpp, %.o, $(ExampleB_SRC))

ExampleD_SRC=$(wildcard ./index_gta.cpp ./index_supp.cpp ./index_ordpri.cpp ../public/bintree.cpp ../public/wwtiny.cpp ../comm_supp/gta_supp.cpp )
ExampleD_OBJS=$(patsubst %.cpp, %.o, $(ExampleD_SRC))

ExampleE_SRC=$(wildcard ./dmpo2txt.cpp ./dmpfmt.cpp ../public/wwtiny.cpp ../public/mktdata.pb.cpp ../public/d31data.pb.cpp)
ExampleE_OBJS=$(patsubst %.cpp, %.o, $(ExampleE_SRC))

ExampleF_SRC=$(wildcard ./gta2tdf.cpp ../comm_supp/gta_supp.cpp ../public/wwtiny.cpp )
ExampleF_OBJS=$(patsubst %.cpp, %.o, $(ExampleF_SRC))


#所有目标文件
OBJS= $(ExampleB_OBJS) $(ExampleD_OBJS) $(ExampleE_OBJS) $(ExampleF_OBJS)

TARGETS=$(TARGET_B) $(TARGET_D) $(TARGET_E) $(TARGET_F)

.PHONY: debug release clean

release: $(TARGETS)

debug: $(TARGETS)

#连接动态库
$(TARGET_B): $(ExampleB_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB)

$(TARGET_D): $(ExampleD_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^
	
$(TARGET_E): $(ExampleE_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB)

$(TARGET_F): $(ExampleF_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^

%.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(INC) $<

clean:
	rm -fr $(TARGETS) $(OBJS)


