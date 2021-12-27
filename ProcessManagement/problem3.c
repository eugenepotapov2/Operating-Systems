#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    pid_t child1Pid, child2Pid;
    (child1Pid = fork()) && (child2Pid = fork());  //create two children
    int limit = strtol(argv[1], 0, 10);            //covert argv[1] into integer

    if (child1Pid < 0) {  //display message if fork failed
        printf("fork failed for child one %d\n", getpid());
    }

    if (child1Pid == 0) {
        printf("I am child one, my pid is %d\n", getpid());
        int temp = 0;
        for (int i = 1; i <= limit; i = i + 2) { //for loop to display series of odd
            printf("%d\n", temp = temp + i);
        }
        return 0;
    }

    if (child2Pid < 0) {  //display message if fork failed
        printf("fork failed for child two %d\n", getpid());
    }

    if (child2Pid == 0) {
        printf("I am child two, my pid is %d\n", getpid());
        int temp = 0;
        for (int i = 2; i <= limit; i = i + 2) { //for loop to display series of even
            printf("%d\n", temp = temp + i);
        }
        return 0;
    }

    int status1, status2;
    waitpid(child1Pid, &status1, 0);  //wait for the first child
    waitpid(child2Pid, &status2, 0);  //wait for the second child
    printf("I am a parent, my pid: %d\n", getpid());
    int temp = 0;
    for (int i = 1; i <= limit; i++) { //for loop to display the whole series
        printf("%d\n", temp = temp + i);
    }

    return 0;
}