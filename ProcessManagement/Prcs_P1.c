#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    int fd;

    remove("destination1.txt");  //remove both files in case if they already exist
    remove("destination2.txt");

    fd = open("destination1.txt", O_RDWR | O_CREAT, 0777);  //create first file with rwx permissions

    if (fd < 0) {
        printf("\n open() failed with error: %s\n", strerror(errno));
    } else {
        printf("\n file destination1.txt created successfully\n");
    }

    fd = chmod("destination1.txt", 0777);  //override permissions of created file as a safety measure (for some reason open() doesn't give write permission for others)

    if (fd < 0) {
        printf("\n chmod() failed with error: %s\n", strerror(errno));
    }

    fd = open("destination2.txt", O_RDWR | O_CREAT, 0777);  //create second file with rwx permissions

    if (fd < 0) {
        printf("\n open() failed with error: %s\n", strerror(errno));
    } else {
        printf("\n file destination2.txt created successfully\n");
    }

    fd = chmod("destination2.txt", 0777);

    if (fd < 0) {
        printf("\n chmod() failed with error: %s\n", strerror(errno));
    }

    close(fd);
    return 0;
}