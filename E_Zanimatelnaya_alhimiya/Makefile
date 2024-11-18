CC=g++ 
STDFLAGS=-Wall -Wextra -Werror -std=c++20
LINUX_FLAGS=-lpthread -lm -lsubunit
LIBS=-lgtest

all: clean
	$(CC) $(STDFLAGS) src/main.cc -o main

run:
	./main

debug: clean
	$(CC) -g -O0 $(STDFLAGS) src/main.cc -o main


test: clean
	$(CC) -g -O0 $(STDFLAGS) src/main.cc -o main
	$(CC) -g -O0 $(STDFLAGS) test/*.cc -o unit_test $(LIBS)
	./unit_test

clean:
	rm -rf main
	rm -rf unit_test

clang:
	clang-format -style=Google -n src/*.cc
	clang-format -style=Google -n test/*.cc test/*.h

clang_i:
	clang-format -style=Google -i src/*.cc
	clang-format -style=Google -i test/*.cc test/*.h