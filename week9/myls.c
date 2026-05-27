#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

// struct dirent {
//     ino_t d_ino;       /* Inode number */
//     char d_name[256];  /* Null-terminated filename */
// };


/*
 * Takes a dirname as a string ("." for current directory) and shows the
 * contents of that directory.
 */
void ls(const char *dirname)
{ 
    DIR *dirp = opendir(dirname);
    struct dirent *dirent = readdir(dirp);

    struct stat statbuf;
    char fullpath[4096];

    while(dirent != NULL){
        sprintf(fullpath, "%s/%s", dirname, dirent->d_name);
        stat(fullpath, &statbuf);

        printf("File Name: %s Inode Number: %lu\n", dirent->d_name, dirent->d_ino);
        printf("Mode: %o User ID: %d Group ID: %d Size: %ld Block Size %ld Blocks: %ld\n\n", 
            statbuf.st_mode, statbuf.st_uid, statbuf.st_gid, statbuf.st_size, statbuf.st_blksize, statbuf.st_blocks);

        dirent = readdir(dirp);
    }

    closedir(dirp);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: myls dirname\n");
        exit(1);
    }

    char *dirname = argv[1];
    
    ls(dirname);
}