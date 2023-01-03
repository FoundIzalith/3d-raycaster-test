main: main.c
	gcc main.c -lGL -lGLU -lglut -o main.o 

run: main
	./main.o 

clean: 
	rm *.o
