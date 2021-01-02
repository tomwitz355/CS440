main: Map.hpp
	g++ minimal.cpp -Wall -Wextra -pedantic -o mini
	g++ test1.cpp -Wall -Wextra -pedantic -o test1
	g++ morse.cpp -Wall -Wextra -pedantic -o morse
	g++ test2.cpp -Wall -Wextra -pedantic -o test2
	g++ performance.cpp -Wall -Wextra -pedantic -O -o performance

mini: Map.hpp
	g++ minimal.cpp -Wall -Wextra -pedantic -o mini

test1: Map.hpp
	g++ test1.cpp -Wall -Wextra -pedantic -o test1

morse: Map.hpp
	g++ morse.cpp -Wall -Wextra -pedantic -o morse

test2: Map.hpp
	g++ test2.cpp -Wall -Wextra -pedantic -o test2

performance: Map.hpp
	g++ performance.cpp -Wall -Wextra -pedantic -o performance


run: main
	./mini
	./test1
	./test2
	./performance

clean:
	rm -rf mini
	rm -rf test1
	rm -rf morse
	rm -rf test2
	rm -rf performance

winclean:
	del *.exe
