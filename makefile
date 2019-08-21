#
# runs all the test cases named test*.cpp against the correct implementation.
#
# usage: make all
#
all:
	rm -f test.exe
	g++ -g -std=c++11 -Wall main.cpp test*.cpp gradeutil.cpp -o test.exe
	./test.exe


#
# Runs one test case, e.g. "test01.cpp", against the correct implementation.
#
# usage: make test=test1
#
one:
	rm -f test.exe
	g++ -g -std=c++11 -Wall main.cpp $(test).cpp gradeutil.cpp -o test.exe
	./test.exe


#
# runs the test(s):
#
run:
	./test.exe

