main: SharedPtr.hpp
	g++ test.cpp -Wall -Wextra -pthread -pedantic -o test

run: main
	./test

clean:
	rm -rf test
	rm -rf *.o

winclean:
	del *.exe
