CXX = g++
CXXFLAGS = -Wall -Werror -pedantic --std=c++11 -g

calculator_tests.exe: calculator_tests.cpp unit_test_framework.cpp calculator.h linked_list.h
		$(CXX) $(CXXFLAGS) $< unit_test_framework.cpp -o $@
