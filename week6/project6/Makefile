all: libvvsfs.a testfs

%.o: %.c
	gcc -Wall -Wextra -g -DCTEST_ENABLE -c $< -o $@

libvvsfs.a: image.o block.o
	ar rcs libvvsfs.a image.o block.o

testfs: testfs.o libvvsfs.a
	gcc -Wall -Wextra -g -o testfs testfs.o -L. libvvsfs.a

test: testfs
	./testfs

clean:
	rm -f *.o

pristine: clean
	rm -f testfs *.a
