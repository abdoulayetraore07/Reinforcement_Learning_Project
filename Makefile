all: src/Q_learning.x src/Sarsa.x src/Morpion_2Q.x src/Morpion_Q.x

src/dfs         : src/dfs.o src/functions.o src/mazeEnv.o
	gcc -I include/ -g -o src/dfs src/dfs.o src/functions.o src/mazeEnv.o -Wall

src/dfs.o       : src/dfs.c  include/mazeEnv.h
	gcc -I include/ -g -o src/dfs.o -c src/dfs.c -Wall

src/functions.o : src/functions.c include/functions.h
	gcc -I include/ -g -o src/functions.o -c src/functions.c -Wall

src/mazeEnv.o   : src/mazeEnv.c include/mazeEnv.h
	gcc -I include/ -g -o src/mazeEnv.o -c src/mazeEnv.c -Wall

src/Q_learning.x	: src/Q_learning.c include/mazeEnv.h include/functions.h include/mazeEnv.h
	gcc -I include/ -o src/Q_learning.x -c src/Q_learning.c -Wall

src/Sarsa.x	: src/Sarsa.c include/mazeEnv.h include/functions.h include/mazeEnv.h
	gcc -I include/ -o src/Sarsa.x -c src/Sarsa.c -Wall

src/Morpion_2Q.x	: src/Morpion_2Q.c 
	gcc -o src/Morpion_2Q.x -c src/Morpion_2Q.c -Wall

src/Morpion_Q.x	: src/Morpion_Q.c 
	gcc -o src/Morpion_Q.x -c src/Morpion_Q.c -Wall


clean		:		
	rm -f *~ *.o

realclean	: clean 	
	rm -f *.x
