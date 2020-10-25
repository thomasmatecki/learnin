CXX = c++
CXXFLAGS = -std=c++17 -Wall -pthread -c
LXXFLAGS = -std=c++17 -lgtest -lgtest_main 
OBJECTS = obj/fib.o obj/lis.o obj/lcs.o obj/array_list.o \
	obj/fib_test.o obj/lis_test.o obj/lcs_test.o obj/array_list_test.o
TARGET = out/test

$(TARGET): $(OBJECTS)
	$(CXX) $(LXXFLAGS) $(OBJECTS) -o $(TARGET) 

./obj/fib.o: src/fib.cpp
	$(CXX) $(CXXFLAGS) src/fib.cpp -o obj/fib.o

./obj/fib_test.o: src/fib_test.cpp
	$(CXX) $(CXXFLAGS) src/fib_test.cpp -o obj/fib_test.o

./obj/lis.o: src/lis.cpp
	$(CXX) $(CXXFLAGS) src/lis.cpp -o obj/lis.o

./obj/lis_test.o: src/lis_test.cpp
	$(CXX) $(CXXFLAGS) src/lis_test.cpp -o obj/lis_test.o

./obj/lcs.o: src/lcs.cpp
	$(CXX) $(CXXFLAGS) src/lcs.cpp -o obj/lcs.o

./obj/lcs_test.o: src/lcs_test.cpp
	$(CXX) $(CXXFLAGS) src/lcs_test.cpp -o obj/lcs_test.o

./obj/array_list.o: src/array_list.cpp
	$(CXX) $(CXXFLAGS) src/array_list.cpp -o obj/array_list.o

./obj/array_list_test.o: src/array_list_test.cpp
	$(CXX) $(CXXFLAGS) src/array_list_test.cpp -o obj/array_list_test.o

