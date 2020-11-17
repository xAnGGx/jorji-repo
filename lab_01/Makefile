lab1: bin bin/io.o bin/util.o bin/algorithm.o bin/main.o
	gcc -o lab1 bin/main.o bin/algorithm.o bin/util.o bin/io.o

bin/main.o: src/main.c include/io.h include/util.h include/algorithm.h
	gcc -c src/main.c -o bin/main.o

bin/algorithm.o: src/algorithm.c include/io.h include/util.h
	gcc -c src/algorithm.c -o bin/algorithm.o

bin/util.o: src/util.c include/io.h
	gcc -c src/util.c -o bin/util.o

bin/io.o: src/io.c
	gcc -c src/io.c -o bin/io.o

bin:
	mkdir -p bin

clean:
	rm -rf lab1 bin
