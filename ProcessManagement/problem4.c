#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    char *filepath = argv[1];  //our file name (readme.txt or anything else)
    pid_t childPid;
    int fd;
    int returnval;
    int pipefd[2];
    pipe(pipefd);       //create pipe
    childPid = fork();  //create child

    if (childPid < 0) {  //display message if fork failed
        printf("fork failed got child %d\n", getpid());
    }

    if (childPid == 0) {
        printf("I am child, my pid is %d\n", getpid());

        //open file
        fd = open(argv[1], O_RDONLY);
        if (fd == -1) {
            printf("open() failed with error: %s\n", strerror(errno));
        } else {
            printf("open() successful\n");
        }

        //use lseek() to reposition the offset and get exact number of characters in file
        int fileLength = lseek(fd, 0, SEEK_END);
        //repostion offset to the beginning of the file to use read()
        lseek(fd, 0, SEEK_SET);

        char buffer[fileLength];
        read(fd, buffer, fileLength);          //read file content into the buffer
        close(pipefd[0]);                      //close unused end of the pipe
        write(pipefd[1], buffer, fileLength);  //write buffer into the writing end of the pipe
        close(pipefd[1]);                      //close writing end of the pipe for the child process
        close(fd);                             // close file
        return 0;                              //use return 0 to termiante child process
    }

    int status;
    int buf;
    waitpid(childPid, &status, 0);  //wait for the child
    printf("I am, a parent, my pid: %d\n", getpid());

    //open file
    fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        printf("open() failed with error: %s\n", strerror(errno));
    } else {
        printf("open() successful\n");
    }

    close(pipefd[1]);  //close unused end of the pipe
    char *pWriting = "Parent is writing: ";
    write(fd, pWriting, 19);                //write into the file "Parent is writing: "
    while (read(pipefd[0], &buf, 1) > 0) {  //use while loop to write contents of the reading end of the pipe into the file
        write(fd, &buf, 1);
    };
    close(pipefd[0]);  //close other end of the pipe
    close(fd);         //close file

    return 0;
}