# Project 6 File System

## Building
Command line:

* `make` to build. Several object files and an executable `testfs` will be produced.
* `make clean` to clean up all build products except the executable.
* `make pristine` to clean up all build products entirely.
* `make test` to build and run the testfs executable.

## Files

* `image.c`: Code for opening and closing an image
* `block.c`: Code for reading and writing to a block
* `testfs.c`: Test code that verifies the code in other files is working.
* `ctest.h`: Library for creating code tests

## Data

The image code manages a global integer variable that represents the image's file descriptor. It is used to open and close the image, and also to find which file block needs to read and write to. 

## Functions

### `image.c`
* `image_open()`: sets the global `image_fd` variable to an open file
    * `open()`
* `image_close()`: closes the file defined by `image_fd`
    * `close()`

### `block.c`
* `bread()`: reads a block and returns the result
    * `lseek()`
* `bwrite()`: goes to a block and writes data to it
    * `lseek()`

### `testfs.c`
* `main()`: Runs various tests to assert that the other files functions are working correcly
