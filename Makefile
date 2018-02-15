all:
	make robot

robot: bumpers.o IR_sensors.o light_sensors.o main.o
	g++ ED.o main.o -o editDistance -lsfml-graphics -lsfml-window -lsfml-system

light_sensors.o: light_sensors.cpp light_sensors.hpp
	g++ -c ED.cpp -Wall -Werror -ansi -pedantic



main.o: main.cpp ED.hpp
	g++ -c main.cpp -Wall -Werror -ansi -pedantic

clean:
rm -rf *.o editDistance *~
