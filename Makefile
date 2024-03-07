all: pacman-game

main.o: main.cpp
	g++ -c main.cpp -fopenmp -Wall 
	
pacman-game: main.o
	g++ main.o -o pacman-game -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f *.o pacman-game

