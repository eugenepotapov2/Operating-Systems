#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    int fd;
    if (2 != argc) {
        printf("\n Usage: \n");
    }
    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    if (fd == -1) {
        printf("\n open() failed with error %s\n", strerror(errno));
        perror("open");
    } else {
        printf("\n open() successful\n");
    }
    close(fd);
    return 0;
}