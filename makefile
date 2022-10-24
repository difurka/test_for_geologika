CXX=g++
CPPFLAGS=-lstdc++ -std=c++17
WCHECK=#-Wall -Wextra -Werror
# TESTFLAG=-lgtest --coverage -fprofile-arcs -ftest-coverage
# DEBUGFLAG=-ggdb3

# OS = $(shell uname -s)

# ifeq ($(OS), Linux)
# 	CPPFLAGS+= -pthread
# endif

HEADERS=read_from_consol.h \
		./elements_of_device/pump.h \
		./elements_of_device/sensor.h \
		queue_of_commands.h

SOURCE=read_from_consol.cpp \
		# ./elements_of_device/pump.cpp \
		# elements_of_device/sensor.cpp 
	   
# HASHTABLE=containers/hash_table.cpp
# SELFBALANCING=containers/self_balancing_binary_search_tree.cpp
# BPLUS=containers/b_plus_tree.cpp

# ALLSOURCE=$(HASHTABLE) $(SELFBALANCING) $(BPLUS) $(SOURCE)
MAIN=main.cpp
# TESTFILE=tests.cpp

.PHONY: all implement #clean test leaks linter check test_out transactions

all: implement

implement:
	@make clean
	@$(CXX) $(CPPFLAGS) $(MAIN) $(SOURCE) $(WCHECK) -pthread -o device.out
	@./device.out

# hash_table.a:  hash_table.o
# 	@ar -crs $@ $^
# 	@ranlib $@

# self_balancing_binary_search_tree.a:  self_balancing_binary_search_tree.o
# 	@ar -crs $@ $^
# 	@ranlib $@

# b_plus_tree.a:  b_plus_tree.o
# 	@ar -crs $@ $^
# 	@ranlib $@

# hash_table.o: $(HASHTABLE)
# 	@$(CXX) $(CPPFLAGS) $(SOURCE) -c $^

# self_balancing_binary_search_tree.o: $(SELFBALANCING)
# 	@$(CXX) $(CPPFLAGS) $(SOURCE) -c $^

# b_plus_tree.o: $(BPLUS)
# 	@$(CXX) $(CPPFLAGS) $(SOURCE) -c $^

# tests: test_out
# 	@./test.out
# 	@make clean

# test_out: clean
# 	@$(CXX) $(TESTFILE) $(ALLSOURCE) $(CPPFLAGS) $(DEBUGFLAG) $(TESTFLAG) $(WCHECK) -o test.out

clean:
	@rm -rf device.out

# leaks: test_out
# 	@CK_FORK=no leaks --atExit -- ./test.out
# 	@make clean

# linter:
# 	@cp ../materials/linters/CPPLINT.cfg .
# 	@python3 ../materials/linters/cpplint.py --extensions=cpp $(HEADERS) $(ALLSOURCE) $(TESTFILE) $(MAIN)
# 	@rm CPPLINT.cfg

check:
	@cppcheck --enable=all --suppress=missingIncludeSystem $(HEADERS) $(ALLSOURCE) $(TESTFILE) $(MAIN)
