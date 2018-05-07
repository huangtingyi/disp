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
TARGET_A=../bin/gta_ints_2
TARGET_B=../bin/dmpi2txt
TARGET_C=../bin/gta_replay
TARGET_D=../bin/index_stat
TARGET_E=../bin/dmpo2txt
TARGET_F=../bin/gta2tdf

#编译命令
CXX=g++

#头文件
INC= -I../Include -I../Common -I../public -I../IncludeTDF -I../comm_supp -I../app_ints

#库
LIB=-pthread -lcurl -lboost_log -lboost_filesystem -lboost_thread -lboost_log_setup -lboost_system -lprotobuf -lrt



#测试工程文件
Example_SRC=$(wildcard ./main.cpp ../app_ints/CallBackBase.cpp ../app_ints/callsupp.cpp ../public/wwtiny.cpp ../public/MessageQueue.cpp ../public/mktdata.pb.cpp ../public/d31data.pb.cpp ../Common/*.cpp ../comm_supp/gta_supp.cpp )
Example_OBJS=$(patsubst %.cpp, %.o, $(Example_SRC))

ExampleB_SRC=$(wildcard ./dmpi2txt.cpp ../public/wwtiny.cpp )
ExampleB_OBJS=$(patsubst %.cpp, %.o, $(ExampleB_SRC))

ExampleC_SRC=$(wildcard ./gta_replay.cpp ../app_ints/CallBackBase.cpp ../app_ints/callsupp.cpp ../public/wwtiny.cpp ../public/MessageQueue.cpp ../public/mktdata.pb.cpp ../public/d31data.pb.cpp ../Common/*.cpp ../comm_supp/gta_supp.cpp )
ExampleC_OBJS=$(patsubst %.cpp, %.o, $(ExampleC_SRC))

ExampleD_SRC=$(wildcard ./index_stat.cpp ./index_supp.cpp ./index_ordpri.cpp ../public/bintree.cpp ../public/wwtiny.cpp ../comm_supp/gta_supp.cpp )
ExampleD_OBJS=$(patsubst %.cpp, %.o, $(ExampleD_SRC))

ExampleE_SRC=$(wildcard ./dmpo2txt.cpp ../public/wwtiny.cpp ../public/mktdata.pb.cpp )
ExampleE_OBJS=$(patsubst %.cpp, %.o, $(ExampleE_SRC))

ExampleF_SRC=$(wildcard ./gta2tdf.cpp ../comm_supp/gta_supp.cpp ../public/wwtiny.cpp )
ExampleF_OBJS=$(patsubst %.cpp, %.o, $(ExampleF_SRC))


#所有目标文件
OBJS= $(Example_OBJS)
OBJBS= $(ExampleB_OBJS)
OBJCS= $(ExampleC_OBJS)
OBJDS= $(ExampleD_OBJS)
OBJES= $(ExampleE_OBJS)
OBJFS= $(ExampleF_OBJS)


.PHONY: debug release clean

release: $(TARGET_A) $(TARGET_B) $(TARGET_C) $(TARGET_D) $(TARGET_E) $(TARGET_F)

debug: $(TARGET_A) $(TARGET_B) $(TARGET_C) $(TARGET_D)
#连接静态库
$(TARGET_A): $(Example_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB) -L$(TARGETPATH) -lGTAQTSApi

$(TARGET_B): $(ExampleB_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^

$(TARGET_C): $(ExampleC_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB) -L$(TARGETPATH) -lGTAQTSApi

$(TARGET_D): $(ExampleD_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^
	
$(TARGET_E): $(ExampleE_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB)

$(TARGET_F): $(ExampleF_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^

%.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(INC) $<

clean:
	rm -fr $(TARGET_A) $(TARGET_B) $(TARGET_C) $(TARGET_D) $(TARGET_E) $(TARGET_E) $(OBJS) $(OBJBS) $(OBJCS) $(OBJDS) $(OBJES) $(OBJFS)


