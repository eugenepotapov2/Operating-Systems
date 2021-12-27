#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    pid_t childPid;
    childPid = fork();  //create child one using fork

    if (childPid < 0) {  //display message if fork failed
        printf("fork failed for child%d\n", getpid());
        perror("fork()");
    }

    if (childPid == 0) {
        printf("In the child process, my pid: %d, my ppid: %d\n", getpid(), getppid());
        execl("/bin/date", "date", (char *)0);  //run execl with path
        printf("execl failed\n");               //print message if execl wasn't executed
        perror("execl()");
        return 0;
    }

    int status;
    waitpid(childPid, &status, 0);  //wait for child
    printf("In the parent process, my pid: %d\n", getpid());

    return 0;
}