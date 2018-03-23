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
#编译命令
CXX=g++ 

#头文件
INC= -I../Include -I../Common -I../public -I../app_ints

#库
LIB=-pthread -lcurl -lboost_log -lboost_filesystem -lboost_thread -lboost_log_setup -lboost_system -lprotobuf -lrt

#测试工程文件
Example_SRC=$(wildcard ./*.cpp ../app_ints/CallBackBase.cpp ../public/getToday.cpp ../public/MessageQueue.cpp ../public/mktdata.pb.cpp ../Common/*.cpp ../GTA2csv/*.cpp ../../cs_public/getToday/*.cpp ../GTA2TDF/*.cpp )
Example_OBJS=$(patsubst %.cpp, %.o, $(Example_SRC))

#所有目标文件
OBJS= $(Example_OBJS)

.PHONY: debug release clean

release: $(TARGET_A)

debug: $(TARGET_A)
#连接静态库
$(TARGET_A): $(Example_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB) $(TARGETPATH)/GTAQTSApi.a 

#连接动态库
$(TARGET_SO): $(Example_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB) -L$(TARGETPATH) -lGTAQTSApi 

%.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(INC) $<

clean:
	rm -fr $(TARGET_A) $(TARGET_SO)  $(OBJS)


