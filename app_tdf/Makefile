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
TARGET_A=../bin/tdf_demo
TARGET_B=../bin/tdf_ints

#编译命令
CXX=g++ 

#头文件
INC= -I../Include -I../Common -I../public -I../IncludeTDF -I../app_ints

#库
LIB=-pthread -lcurl -lboost_log -lboost_filesystem -lboost_thread -lboost_log_setup -lboost_system -lprotobuf -lrt

#测试工程文件
Example_SRC=$(wildcard ./main_demo.cpp ../public/MessageQueue.cpp ../public/mktdata.pb.cpp ../Common/*.cpp)
Example_OBJS=$(patsubst %.cpp, %.o, $(Example_SRC))

Example_SRC_B=$(wildcard ./main.cpp ../app_ints/callsupp.cpp ../public/wwtiny.cpp ../public/MessageQueue.cpp ../public/mktdata.pb.cpp ../Common/*.cpp)
Example_OBJS_B=$(patsubst %.cpp, %.o, $(Example_SRC_B))


#所有目标文件
OBJS= $(Example_OBJS)
OBJS_B= $(Example_OBJS_B)


.PHONY: debug release clean

release: $(TARGET_A) $(TARGET_B)

debug: $(TARGET_A) $(TARGET_B)
#直接连接动态库
$(TARGET_A): $(Example_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB) -L$(TARGETPATH) -lTDFAPI30

$(TARGET_B): $(Example_OBJS_B)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB) -L$(TARGETPATH) -lTDFAPI30

%.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(INC) $<

clean:
	rm -fr $(TARGET_A) $(OBJS) $(TARGET_B) $(OBJS_B)


