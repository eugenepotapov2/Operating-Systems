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
    int fd3;

    // Open source.txt read only mode
    fd = open("source.txt", O_RDONLY);
    if (fd < 0) {
        printf("\n open() for source.txt failed with error: %s\n", strerror(errno));
    } else {
        printf("\n source.txt opened successfully\n");
    }

    // Open destination1.txt
    fd2 = open("destination1.txt", O_RDWR);
    if (fd2 < 0) {
        printf("\n open() for destination1.txt failed with error: %s\n", strerror(errno));
    } else {
        printf("\n destination1.txt opened successfully\n");
    }

    // Open destination2.txt
    fd3 = open("destination2.txt", O_RDWR);
    if (fd3 < 0) {
        printf("\n open() for destination1.txt failed with error: %s\n", strerror(errno));
    } else {
        printf("\n destination2.txt opened successfully\n");
    }

    // Use lseek() to reposition the offset and get exact number of characters in source file
    int sourceLength = lseek(fd, 0, SEEK_END);

    int temp;       // Using temp integer to track every 100 or 50 characters
    int copyNum;    // Number of copied characters
    int adder;      // Number (either 50 or 100) to increases temp and calculate last remaining chars
    char replacer;  // Character that replaces either 1 or 3
    char replaced;  // 1 or 3
    int fdTemp;     // fd that we want to use (either fd2 or fd3)
    char buffer[100];

    // for loop to run while loop for destination1.txt and destination2.txt
    for (int i = 0; i < 2; i++) {
        if (i == 0) {                // for i == 0 we use parameters for destination1.txt
            lseek(fd, 0, SEEK_SET);  // Repostion offset to the beginning of the source file to use read()
            temp = 0;
            copyNum = 100;
            adder = 100;
            replaced = '1';
            replacer = 'L';
            fdTemp = fd2;

        } else {                     // for i == 1 we use parameters for destination1.txt
            lseek(fd, 0, SEEK_SET);  // Repostion offset to the beginning of the source file to use read()
            temp = 0;
            copyNum = 50;
            adder = 50;
            replaced = '3';
            replacer = 'E';
            fdTemp = fd3;
        }
        // while loop to copy only given amount of characters from the source
        while (temp < sourceLength - (sourceLength % adder) + adder) {
            // If temp number is equal (total number of characters minus remainder that is less than 100 or 50) then copyNum = remainder
            if (temp == sourceLength - (sourceLength % adder)) {
                copyNum = sourceLength % adder;
            }
            read(fd, buffer, copyNum);

            // for loop to replace '1' or '3' with 'L' or 'E' respectively in the copied buffer
            for (int i = 0; i < copyNum; i++) {
                if (buffer[i] == replaced) {
                    buffer[i] = replacer;
                }
            }
            write(fdTemp, buffer, copyNum);

            // Use lseek() to move offset by the number of copied characters
            lseek(fd, temp, copyNum);
            lseek(fdTemp, temp, copyNum);
            temp = temp + adder;
        }
    }
    printf("\n");
    close(fd);
    close(fd2);
    close(fd3);
    return 0;
}