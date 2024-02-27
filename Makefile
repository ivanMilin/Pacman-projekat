all: snake-game

main.o: main.cpp
	g++ -c main.cpp
	
snake-game: main.o
	g++ main.o -o snake-game -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f *.o snake-game

