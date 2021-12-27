#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    pid_t child1Pid, child2Pid;
    (child1Pid = fork()) && (child2Pid = fork());  //create two children

    if (child1Pid < 0) {  //display message if fork failed
        printf("fork failed for child one %d\n", getpid());
    }

    if (child1Pid == 0) {  //print message for the first child
        printf("I am child one, my pid is %d\n", getpid());
        return 0;
    }

    if (child2Pid < 0) {  //display message if fork failed
        printf("fork failed for child two %d\n", getpid());
    }

    if (child2Pid == 0) {  //print message for the second child
        printf("I am child two, my pid is %d\n", getpid());
        return 0;
    }

    int status1, status2;
    waitpid(child1Pid, &status1, 0);  //wait for the first child
    waitpid(child2Pid, &status2, 0);  //wait for the second child
    printf("I am a parent, my pid: %d\n", getpid());

    return 0;
}