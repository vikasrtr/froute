# main makefile for entire project

#start with adding any dependencies
CC=gcc
CFLAGS = -Wall -g -O3
CFLAGS += -std=gnu++11

CXX = g++
CXXFLAGS = -Wall -g -O3
CXXFLAGS+= -std=c++11

export CXXFLAGS
export CFLAGS


SUB_DIRS = froute

###################
# build all modules
all: components
	@echo Building froute...
	@echo Bulding individual modules

####################
.PHONY: components clean
 
components:
	$(MAKE) -C $(SUB_DIRS)

####################
clean:
	$(MAKE) -C $(SUB_DIRS) clean


####################
print-%  : ; @echo $* = $($*)