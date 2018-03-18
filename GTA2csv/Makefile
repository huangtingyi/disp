#判断是否是debug编译条件,默认为release编译
ifeq ($(findstring debug,$(MAKECMDGOALS)),debug)
    CXXFLAGS=-g -W -Wall -Wno-unused-parameter  -std=c++0x
    TARGETPATH=../../Debug/Linux64
else
    CXXFLAGS=-O2 -W -Wall -Wno-unused-parameter -std=c++0x
    TARGETPATH=../../Release/Linux64
endif

#最终生成程序
TARGET_A=Example_2_A
TARGET_SO=Example_2_So
#编译命令
CXX=g++ 

#头文件
INC= -I../../Include -I../Common

#库
LIB=-pthread -lcurl -lboost_filesystem

#测试工程文件
Example_SRC=$(wildcard ./*.cpp ./../Common/*.cpp)
Example_OBJS=$(patsubst %.cpp, %.o, $(Example_SRC))

#所有目标文件
OBJS= $(Example_OBJS)

.PHONY: debug release clean

release: $(TARGET_A) $(TARGET_SO)

debug: $(TARGET_A) $(TARGET_SO)
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


