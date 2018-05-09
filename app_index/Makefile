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
TARGET_A=../bin/dmpi2txt
TARGET_B=../bin/dmpo2txt
TARGET_C=../bin/index_gta
TARGET_D=../bin/index_tdf
TARGET_F=../bin/gta2tdf

#编译命令
CXX=g++

#头文件
INC= -I../Include -I../Common -I../public -I../IncludeTDF -I../comm_supp

#库
LIB=-pthread -lcurl -lboost_log -lboost_filesystem -lboost_thread -lboost_log_setup -lboost_system -lprotobuf -lrt



#工程文件
ExampleA_SRC=$(wildcard ./dmpi2txt.cpp ./dmpfmt.cpp ../public/wwtiny.cpp ../public/mktdata.pb.cpp ../public/d31data.pb.cpp)
ExampleA_OBJS=$(patsubst %.cpp, %.o, $(ExampleA_SRC))

ExampleB_SRC=$(wildcard ./dmpo2txt.cpp ./dmpfmt.cpp ../public/wwtiny.cpp ../public/mktdata.pb.cpp ../public/d31data.pb.cpp)
ExampleB_OBJS=$(patsubst %.cpp, %.o, $(ExampleB_SRC))

ExampleC_SRC=$(wildcard ./index_gta.cpp ./index_supp.cpp ./index_ordpri.cpp ../public/bintree.cpp ../public/wwtiny.cpp ../comm_supp/gta_supp.cpp )
ExampleC_OBJS=$(patsubst %.cpp, %.o, $(ExampleC_SRC))

ExampleD_SRC=$(wildcard ./index_tdf.cpp ./index_supp.cpp ./index_ordpri.cpp ../public/bintree.cpp ../public/wwtiny.cpp ../comm_supp/gta_supp.cpp )
ExampleD_OBJS=$(patsubst %.cpp, %.o, $(ExampleD_SRC))


ExampleF_SRC=$(wildcard ./gta2tdf.cpp ../comm_supp/gta_supp.cpp ../public/wwtiny.cpp )
ExampleF_OBJS=$(patsubst %.cpp, %.o, $(ExampleF_SRC))


#所有目标文件
OBJS= $(ExampleA_OBJS) $(ExampleB_OBJS) $(ExampleC_OBJS) $(ExampleD_OBJS) $(ExampleF_OBJS)

TARGETS=$(TARGET_A) $(TARGET_B) $(TARGET_C) $(TARGET_D) $(TARGET_F)

.PHONY: debug release clean

release: $(TARGETS)

debug: $(TARGETS)

#连接动态库
$(TARGET_A): $(ExampleA_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB)
$(TARGET_B): $(ExampleB_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIB)

$(TARGET_C): $(ExampleC_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^
	
$(TARGET_D): $(ExampleD_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^

$(TARGET_F): $(ExampleF_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^

%.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(INC) $<

clean:
	rm -fr $(TARGETS) $(OBJS)


