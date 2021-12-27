#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd;
    int fd2;
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
    // Open source.txt read only mode
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        printf("\n open() failed with error %s\n", strerror(errno));
        perror("open");
    } else {
        printf("\n open() successful\n");
    }
    // Open new destination.txt
    fd2 = open(argv[2], O_RDWR);
    if (fd2 == -1) {
        printf("\n open() failed with error %s\n", strerror(errno));
        perror("open");
    } else {
        printf("\n open() successful\n");
    }
    // Use lseek() to reposition the offset and get exact number of characters in file
    int fileLength = lseek(fd, 0, SEEK_END);
    // Repostion offset to the beginning of the file to use read()
    lseek(fd, 0, SEEK_SET);
    // Using temp integer to track every 100 characters
    int temp = 0;
    // Default number of copied characters is set to 100
    int copyNum = 100;
    char buffer[100];
    // while loop to copy only given amount of characters from the source
    while (temp < fileLength - (fileLength % 100) + 100) {
        // If temp number is equal (1190 (total number of characters) - 90) then copyNum = 90
        if (temp == fileLength - (fileLength % 100)) {
            copyNum = fileLength % 100;
        }
        read(fd, buffer, copyNum);
        // for loop to replace '1' with 'L' in the copied buffer
        for (int i = 0; i < copyNum; i++) {
            if (buffer[i] == '1') {
                buffer[i] = 'L';
            }
        }
        write(fd2, buffer, copyNum);
        write(fd2, "XYZ", 3);
        // Use lseek() to move offset by the number of copied characters
        lseek(fd, temp, copyNum);
        lseek(fd2, temp, copyNum);
        temp = temp + 100;
    }
    printf("\n");
    close(fd);
    close(fd2);
    return 0;
}