CXX = g++
CC = gcc

CPPFLAGS = -g -Wall -Wextra -std=c++11 
CCFLAGS =  -g -Wall -Wextra
LDFLAGS = -shared
TESTFLAGS = -g -L/usr/lib -lgtest -lgtest_main -lpthread

SRCPATH = src
INCPATH = include
BUILDPATH = build
TESTPATH = test
EXAMPLEPATH = examples

COMMONTARGET = libdoipcommon
SERVERTARGET = libdoipserver
CLIENTTARGET = libdoipclient
UDSDOIPTARGET = UdsDoIPServer

# List of all source files, separated by whitespace
COMMONSOURCE = $(wildcard $(COMMONTARGET)/$(SRCPATH)/*.cpp)
SERVERSOURCE = $(wildcard $(SERVERTARGET)/$(SRCPATH)/*.cpp)
CLIENTSOURCE = $(wildcard $(CLIENTTARGET)/$(SRCPATH)/*.cpp)
UDSSOURCE = $(wildcard $(UDSDOIPTARGET)/*.c)

TESTSOURCE = TestRunner.cpp
TESTSOURCE += $(wildcard $(COMMONTARGET)/test/*.cpp)
TESTSOURCE += $(wildcard $(SERVERTARGET)/test/*.cpp)
TESTSOURCE += $(wildcard $(CLIENTTARGET)/test/*.cpp)

COMMONOBJS = $(patsubst $(COMMONTARGET)/$(SRCPATH)/%.cpp, $(BUILDPATH)/%.o, $(COMMONSOURCE))
SERVEROBJS = $(patsubst $(SERVERTARGET)/$(SRCPATH)/%.cpp, $(BUILDPATH)/%.o, $(SERVERSOURCE))
CLIENTOBJS = $(patsubst $(CLIENTTARGET)/$(SRCPATH)/%.cpp, $(BUILDPATH)/%.o, $(CLIENTSOURCE))
UDSOBJS = $(patsubst $(UDSDOIPTARGET)/%.c, $(BUILDPATH)/%.o, $(UDSSOURCE))

EXAMPLESERVERSOURCE = $(EXAMPLEPATH)/exampleDoIPServer.cpp
UDSDOIPSERVERSOURCE = $(UDSDOIPTARGET)/UdsDoIPServer.cpp

.PHONY: all clean

all: env $(BUILDPATH)/$(COMMONTARGET).so $(BUILDPATH)/$(SERVERTARGET).so $(BUILDPATH)/$(CLIENTTARGET).so test examples udsdoip

env:
	mkdir -p $(BUILDPATH)

clean:
	rm -rf $(BUILDPATH)/* 

$(BUILDPATH)/%.o: $(COMMONTARGET)/$(SRCPATH)/%.cpp
	$(CXX) $(CPPFLAGS) -I $(COMMONTARGET)/$(INCPATH) -I $(SERVERTARGET)/$(INCPATH) -fPIC -c $< -o $@
	
$(BUILDPATH)/%.o: $(SERVERTARGET)/$(SRCPATH)/%.cpp
	$(CXX) $(CPPFLAGS) -I $(SERVERTARGET)/$(INCPATH) -I $(COMMONTARGET)/$(INCPATH) -fPIC -c $< -o $@
	
$(BUILDPATH)/%.o: $(CLIENTTARGET)/$(SRCPATH)/%.cpp
	$(CXX) $(CPPFLAGS) -I $(CLIENTTARGET)/$(INCPATH) -I $(COMMONTARGET)/$(INCPATH) -fPIC -c $< -o $@

$(BUILDPATH)/%.o: $(UDSDOIPTARGET)/%.c
	$(CC) $(CCFLAGS) -I $(UDSDOIPTARGET) -c $< -o $@
    
$(BUILDPATH)/$(COMMONTARGET).so: $(COMMONOBJS)
	$(CXX) $(CPPFLAGS) $^ $(LDFLAGS) -o $@
	
$(BUILDPATH)/$(SERVERTARGET).so: $(SERVEROBJS)
	$(CXX) $(CPPFLAGS) $^ $(LDFLAGS) -o $@
	
$(BUILDPATH)/$(CLIENTTARGET).so: $(CLIENTOBJS)
	$(CXX) $(CPPFLAGS) $^ $(LDFLAGS) -o $@
	
test:
	$(CXX) $(CPPFLAGS) -I $(COMMONTARGET)/$(INCPATH) -I $(SERVERTARGET)/$(INCPATH) -I $(CLIENTTARGET)/$(INCPATH) $(COMMONSOURCE) $(SERVERSOURCE) $(CLIENTSOURCE) -o runTest $(TESTSOURCE) $(TESTFLAGS) 

examples: $(BUILDPATH)/exampleDoIPServer

$(BUILDPATH)/exampleDoIPServer: $(EXAMPLESERVERSOURCE)
	$(CXX) $(CPPFLAGS) -I $(COMMONTARGET)/$(INCPATH) -I $(SERVERTARGET)/$(INCPATH) -o $@ $^ -ldoipserver -ldoipcommon -lpthread -L$(BUILDPATH)

udsdoip: $(BUILDPATH)/UdsDoIPServer

$(BUILDPATH)/UdsDoIPServer: $(UDSDOIPSERVERSOURCE) $(UDSOBJS)
	$(CXX) $(CPPFLAGS) -I $(COMMONTARGET)/$(INCPATH) -I $(SERVERTARGET)/$(INCPATH) -I $(UDSDOIPTARGET) -o $@ $^ -ldoipserver -ldoipcommon -lpthread -L$(BUILDPATH)

install:
	install -d /usr/lib/libdoip
	install -d /usr/lib/libdoip/include
	install build/*.so /usr/lib/libdoip
	install libdoipcommon/include/*.h /usr/lib/libdoip/include
	install libdoipserver/include/*.h /usr/lib/libdoip/include
	
