run : compile
	./main

debug : compile
	valgrind --track-origins=yes ./main

compile:
	gcc -o main main.c