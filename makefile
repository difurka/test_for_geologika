CXX=g++
CPPFLAGS=-lstdc++ -std=c++17
WCHECK=-Wall -Wextra -Werror

OS = $(shell uname -s)

ifeq ($(OS), Linux)
	CPPFLAGS+= -pthread
endif

HEADERS=read_from_consol.h \
		commands_execution.h \
		./elements_of_device/pump.h \
		./elements_of_device/sensor.h
		

SOURCE=read_from_consol.cpp \
	   commands_execution.cpp

MAIN=main.cpp

.PHONY: all implement clean

all: implement

implement:
	@make clean
	@$(CXX) $(CPPFLAGS) $(MAIN) $(SOURCE) $(WCHECK) -o device.out
	@./device.out

clean:
	@rm -rf device.out
