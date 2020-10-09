CXX = c++
CXXFLAGS = -std=c++17 -Wall -pthread -c
LXXFLAGS = -std=c++17 -lgtest -lgtest_main 
OBJECTS = obj/fib.o obj/lis.o obj/lcs.o obj/fibMain.o obj/lisMain.o obj/lcsMain.o
TARGET = out/test

$(TARGET): $(OBJECTS)
	$(CXX) $(LXXFLAGS) $(OBJECTS) -o $(TARGET) 

./obj/fib.o: src/fib.cpp
	$(CXX) $(CXXFLAGS) ./src/fib.cpp -o ./obj/fib.o

./obj/fibMain.o: test/fibMain.cpp
	$(CXX) $(CXXFLAGS) ./test/fibMain.cpp -o ./obj/fibMain.o

./obj/lis.o: src/lis.cpp
	$(CXX) $(CXXFLAGS) ./src/lis.cpp -o ./obj/lis.o

./obj/lisMain.o: test/lisMain.cpp
	$(CXX) $(CXXFLAGS) ./test/lisMain.cpp -o ./obj/lisMain.o

./obj/lcs.o: src/lcs.cpp
	$(CXX) $(CXXFLAGS) ./src/lcs.cpp -o ./obj/lcs.o

./obj/lcsMain.o: test/lcsMain.cpp
	$(CXX) $(CXXFLAGS) ./test/lcsMain.cpp -o ./obj/lcsMain.o