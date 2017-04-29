all:
	gcc ./src/*c -o main -Wall -lm -I./include
	
run:
	./main

debug:
	gcc ./src/*c -o main -Wall -I./include -v -g -lm
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./main