main: main.o pack.o
	gcc -o $@ $^

pack.o: pack.c
	gcc -Wall -Wextra -c $<

main.o: main.c
	gcc -Wall -Wextra -c $<