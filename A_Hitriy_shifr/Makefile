CC=g++ 
STDFLAGS=-Wall -Wextra -Werror -std=c++20

all: clean
	$(CC) $(STDFLAGS) src/main.cc -o main
	./main

debug: clean
	$(CC) -g -O0 $(STDFLAGS) src/main.cc -o main
	./main

clean:
	rm -rf main

clang:
	clang-format -style=Google -n src/*.cc

clang_i:
	clang-format -style=Google -i src/*.cc