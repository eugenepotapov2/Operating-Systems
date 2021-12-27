#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    pid_t child1Pid, child2Pid;
    child1Pid = fork();  //create child one using fork

    if (child1Pid < 0) {  //display message if fork failed
        printf("fork failed for child one %d\n", getpid());
        perror("fork()");
    }

    if (child1Pid == 0) {
        printf("In the child one process, my pid: %d, my ppid: %d\n", getpid(), getppid());
        char *args[] = {"Prcs_P1.exe", NULL};
        execv("Prcs_P1.exe", args);  //run execv with path
        printf("execv failed\n");    //print message if execl wasn't executed
        perror("execv()");
        return 0;
    }

    sleep(2);
    child2Pid = fork();  //create child two using fork

    if (child2Pid < 0) {  //display message if fork failed
        printf("fork failed for child two %d\n", getpid());
        perror("fork()");
    }

    if (child2Pid == 0) {
        printf("\nIn the child two process, my pid: %d, my ppid: %d\n", getpid(), getppid());
        char *args[] = {"Prcs_P2.exe", NULL};
        execv("Prcs_P2.exe", args);  //run execv with path
        printf("execv failed\n");    //print message if execl wasn't executed
        perror("execv()");
        return 0;
    }

    int status1, status2;
    waitpid(child1Pid, &status1, 0);  //wait for child
    waitpid(child2Pid, &status2, 0);  //wait for child
    printf("In the parent process, my pid: %d\n", getpid());

    return 0;
}