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
TARGET_A=../bin/ints_gta
TARGET_B=../bin/ints_gta_t
TARGET_C=../bin/replay_gta


#编译命令
CXX=g++

#头文件
INC= -I../Include -I../Common -I../public -I../IncludeTDF -I../comm_supp

#库
LIB=-pthread -lcurl -lboost_log -lboost_filesystem -lboost_thread -lboost_log_setup -lboost_system -lprotobuf -lrt

#工程文件
Example_SRC=$(wildcard ./ints_gta.cpp ./CallBackBase.cpp ../comm_supp/callsupp.cpp ../public/wwtiny.cpp ../public/MessageQueue.cpp ../public/mktdata.pb.cpp ../public/d31data.pb.cpp ../Common/*.cpp ../comm_supp/gta_supp.cpp ../comm_supp/tdf_supp.cpp)
Example_OBJS=$(patsubst %.cpp, %.o, $(Example_SRC))

ExampleB_SRC=$(wildcard ./ints_gta_t.cpp ./CallBackBase.cpp ../comm_supp/callsupp.cpp ../public/wwtiny.cpp ../public/MessageQueue.cpp ../public/mktdata.pb.cpp ../public/d31data.pb.cpp ../Common/*.cpp ../comm_supp/gta_supp.cpp ../comm_supp/tdf_supp.cpp)
ExampleB_OBJS=$(patsubst %.cpp, %.o, $(ExampleB_SRC))

ExampleC_SRC=$(wildcard ./replay_gta.cpp ./CallBackBase.cpp ../comm_supp/callsupp.cpp ../public/wwtiny.cpp ../public/MessageQueue.cpp ../public/mktdata.pb.cpp ../public/d31data.pb.cpp ../Common/*.cpp ../comm_supp/gta_supp.cpp ../comm_supp/tdf_supp.cpp)
ExampleC_OBJS=$(patsubst %.cpp, %.o, $(ExampleC_SRC))

#所有目标文件
OBJS= $(Example_OBJS) $(ExampleB_OBJS) $(ExampleC_OBJS)
TARGETS=$(TARGET_A) $(TARGET_B) $(TARGET_C)

.PHONY: debug release clean 

release: $(TARGETS)

debug: $(TARGETS)
#连接动态库
$(TARGET_A): $(Example_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB) -L$(TARGETPATH) -lGTAQTSApi
$(TARGET_B): $(ExampleB_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB) -L$(TARGETPATH) -lGTAQTSApi
$(TARGET_C): $(ExampleC_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB) -L$(TARGETPATH) -lGTAQTSApi

%.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(INC) $<

clean:
	rm -fr $(TARGETS) $(OBJS)


