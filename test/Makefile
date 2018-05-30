#判断是否是debug编译条件,默认为release编译
ifeq ($(findstring debug,$(MAKECMDGOALS)),debug)
    CXXFLAGS=-g -W -Wall -Wno-unused-parameter  -std=c++0x
else
#    CXXFLAGS=-O2 -W -Wall -Wno-unused-parameter -std=c++0x
	CXXFLAGS=-g -W -Wall -Wno-unused-parameter  -std=c++0x
endif

#最终生成程序
TARGET_A=../bin/notify_watch
TARGET_B=../bin/tcpserver
TARGET_C=../bin/tcpclient
TARGET_D=../bin/memtest
TARGET_E=../bin/broadcast_s
TARGET_F=../bin/broadcast_c
TARGET_G=../bin/agentcli_t


#编译命令
CXX=g++ 

#头文件
INC= -I../public

#库
LIB=-pthread -lcurl -lboost_log -lboost_filesystem -lboost_thread -lboost_log_setup -lboost_system -lprotobuf

#测试工程文件
Example_SRC=$(wildcard ./main.cpp ./Watch.cpp )
Example_OBJS=$(patsubst %.cpp, %.o, $(Example_SRC))

ExampleB_SRC=$(wildcard ./tcpserver.c )
ExampleB_OBJS=$(patsubst %.c, %.o, $(ExampleB_SRC))

ExampleC_SRC=$(wildcard ./tcpclient.c )
ExampleC_OBJS=$(patsubst %.c, %.o, $(ExampleC_SRC))

ExampleD_SRC=$(wildcard ./memtest.c )
ExampleD_OBJS=$(patsubst %.c, %.o, $(ExampleD_SRC))

ExampleE_SRC=$(wildcard ./broadcast_s.cpp )
ExampleE_OBJS=$(patsubst %.cpp, %.o, $(ExampleE_SRC))

ExampleF_SRC=$(wildcard ./broadcast_c.cpp )
ExampleF_OBJS=$(patsubst %.cpp, %.o, $(ExampleF_SRC))

ExampleG_SRC=$(wildcard ./agentcli_t.cpp ./CallBackBase.cpp ../public/wwtiny.cpp ../public/mktdata.pb.cpp ../public/d31data.pb.cpp)
ExampleG_OBJS=$(patsubst %.cpp, %.o, $(ExampleG_SRC))


#所有目标文件
OBJS= $(Example_OBJS) $(ExampleB_OBJS) $(ExampleC_OBJS) $(ExampleD_OBJS) $(ExampleE_OBJS) $(ExampleF_OBJS) $(ExampleG_OBJS)
TARGETS=$(TARGET_A) $(TARGET_B) $(TARGET_C) $(TARGET_D) $(TARGET_E) $(TARGET_F) $(TARGET_G)

.PHONY: debug release clean

release: $(TARGETS)

debug: $(TARGETS)

#连接静态库
$(TARGET_A): $(Example_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB)
$(TARGET_B): $(ExampleB_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB)
$(TARGET_C): $(ExampleC_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB)
$(TARGET_D): $(ExampleD_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB)
$(TARGET_E): $(ExampleE_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB)
$(TARGET_F): $(ExampleF_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB)

$(TARGET_G): $(ExampleG_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB)

%.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(INC) $<

clean:
	rm -fr $(TARGETS)  $(OBJS)


