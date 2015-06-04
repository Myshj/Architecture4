binary_dir ?= build
executable_name ?= server
run_args ?= -p 4242

tests_dir = tests
test_runner = tests

headers ?= thread/myThread.h socket/TCPSocket.h connection/connection.h tcp_server/asioTcpServer.h
main_file ?= server.cpp
source_files ?= thread/myThread.cpp socket/TCPSocket.cpp

flags += -lpthread -std=c++11 -lboost_serialization -lboost_program_options -lboost_system


ifndef suppress_warnings 
	warnings ?= -Wall -Wextra
endif

all : run

.PHONY : run


run : executable
	./$(executable_name) $(run_args)

valgrind_run: executable
	valgrind --track-origins=yes  ./$(executable_name) $(run_args)

.PHONY :executable

executable : $(executable_name)

$(executable_name) : build_files
	g++ $(binary_dir)/* -o $(executable_name) $(flags) 

.PHONY : build_files

build_files : $(binary_dir)

$(binary_dir) : $(headers) $(source_files) $(main_file)
	if ! [ -d $(binary_dir) ]; then mkdir $(binary_dir); fi
	rm -rf $(binary_dir)/*
	( cd $(binary_dir) ; g++ $(addprefix ../, $(main_file)  $(source_files)) -c $(flags) $(additional_flags) $(warnings))

.PHONY : clean

clean :
	rm -rf $(binary_dir)
	rm -rf $(executable_name)

.PHONY : run-tests

run-tests : execute-tests


clean-test-suite :
	( cd $(tests_dir); rm -rf $(test_runner) )

execute-tests : build-tests
	( cd $(tests_dir); ./$(test_runner) )

build-tests : $(tests_dir)/$(test_runner)

$(tests_dir)/$(test_runner) : $(shell find $(tests_dir) -name "*.cpp")
	( cd $(tests_dir) ; g++ $(addprefix ../, $(source_files) $(shell find $(tests_dir) -name "*.cpp"))  $(flags) $(additional_flags) -lboost_unit_test_framework -o $(test_runner))
