#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd;
    char *filepath = argv[1];
    int returnval;
    // Check file existence
    returnval = access(filepath, F_OK);
    if (returnval == 0)
        printf("\n %s exists\n", filepath);
    else {
        if (errno == ENOENT)
            printf("\n%s does not exist\n", filepath);
        else if (errno == EACCES)
            printf("\n%s is not accessible\n", filepath);
        return 0;
    }
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        printf("\n open() failed with error %s\n", strerror(errno));
        perror("open");
    } else {
        printf("\n open() successful\n");
    }
    //use lseek() to reposition the offset and get exact number of characters in file
    int fileLength = lseek(fd, 0, SEEK_END);
    //repostion offset to the beginning of the file to use read()
    lseek(fd, 0, SEEK_SET);
    char buffer[fileLength];
    read(fd, buffer, fileLength);
    write(1, buffer, fileLength);
    printf("\n");
    close(fd);
    return 0;
}