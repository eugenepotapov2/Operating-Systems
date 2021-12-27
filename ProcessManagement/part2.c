#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    pid_t childPid;
    childPid = fork();  //create child one using fork

    if (childPid < 0) {  //display message if fork failed
        printf("fork failed for child %d\n", getpid());
        perror("fork()");
    }

    if (childPid == 0) {
        printf("In the child process, my pid: %d, my ppid: %d\n", getpid(), getppid());
        char *args[] = {"ls", "-l", "-S", "-A", "-F", "-h", NULL};
        //-l - display owner, -S - display size and sort by size, -A - display all content except current folder and parent folder (./ ../) -F - display as list with * and / for .exe and folders -h - display in proper format
        execvp("ls", args);         //run execl with path
        printf("execvp failed\n");  //print message if execl wasn't executed
        perror("execvp()");
        return 0;
    }

    int status;
    waitpid(childPid, &status, 0);  //wait for child
    printf("In the parent process, my pid: %d\n", getpid());

    return 0;
}