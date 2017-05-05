all:
	gcc ./src/*c -O3 -o main -Wall -lm -I./include
	
run:
	./main

debug:
	gcc ./src/*c -O3 -o main -Wall -I./include -v -g -lm
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./main
