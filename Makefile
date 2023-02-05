mapgen: map_navigation.o map_generator.o pathfinder.o heap.o
	gcc map_navigation.o pathfinder.o map_generator.o heap.o -o mapgen -lm

map_navigation.o: map_navigation.c map_navigation.h
	gcc -Wall -Werror -g map_navigation.c -c

pathfinder.o: pathfinder.c pathfinder.h
	gcc -Wall -Werror -g pathfinder.c -c

map_generator.o: map_generator.c map_generator.h
	gcc -Wall -Werror -g map_generator.c -c

heap.o: heap.c heap.h
	gcc -Wall -Werror -g heap.c -c

clean:
	rm -f *~ mapgen
	rm -f *~ *.o