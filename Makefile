CXX = g++ -std=c++17 -O3 -Wall -Wextra -pedantic
MAIN_BINARIES = $(basename $(wildcard *Main.cpp))
TEST_BINARIES = $(basename $(wildcard *Test.cpp))
HEADERS = $(wildcard *.h)
OBJECTS = $(addsuffix .o, $(basename $(filter-out %Main.cpp %Test.cpp, $(wildcard *.cpp))))
LIBRARIES = -lglfw -lGL -lX11 -lrt -ldl -lfreetype

.PRECIOUS: %.o
.SUFFIXES:
.PHONY: all compile test valgrind checkstyle clean install

all: compile test checkstyle

install:
	sudo apt install libglfw3 libglfw3-dev libfreetype6-dev

compile: $(MAIN_BINARIES) $(TEST_BINARIES)

test: $(TEST_BINARIES)
	for T in $(TEST_BINARIES); do ./$$T || exit; done

valgrind: $(TEST_BINARIES)
	for T in $(TEST_BINARIES); do valgrind --leak-check=full ./$$T; done

checkstyle:
	python3 ../cpplint.py --repository=. *.h *.cpp

clean:
	rm -f *.o
	rm -f $(MAIN_BINARIES)
	rm -f $(TEST_BINARIES)
	clear

%Main: %Main.o $(OBJECTS)
	$(CXX) -o $@ $^ $(LIBRARIES)

%Test: %Test.o $(OBJECTS)
	$(CXX) -o $@ $^ $(LIBRARIES) -lgtest -lgtest_main -lpthread

%.o: %.cpp $(HEADERS)
	$(CXX) -I/usr/include/freetype2 -c $<
