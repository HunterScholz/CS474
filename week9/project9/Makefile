all: libvvsfs.a testfs
OBJS = image.o block.o free.o inode.o pack.o dir.o

%.o: %.c
	gcc -Wall -Wextra -g -DCTEST_ENABLE -c $< -o $@

libvvsfs.a: $(OBJS)
	ar rcs $@ $^

testfs: testfs.o libvvsfs.a
	gcc -Wall -Wextra -g -o testfs testfs.o -L. libvvsfs.a

test: testfs
	./testfs

clean:
	rm -f *.o

pristine: clean
	rm -f testfs *.a
