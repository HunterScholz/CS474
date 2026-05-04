.PHONY: all clean

prog: adder.o main.o
	gcc -o main $^

clean:
	rm -f *.o

main.o: main.c
	gcc -Wall -Wextra -c $<

adder.o: adder.c
	gcc -Wall -Wextra -c $<