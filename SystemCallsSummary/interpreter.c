#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    pid_t childPid;
    char inputString[256];  //array to take input from terminal
    char quit[] = "quit\n";
    char *args[256];  //array that is used for exec command
    char *token;      //token to split input array into components
    while (1) {       //while loop to always run
        int startArg = 0;
        printf("command: ");
        fgets(inputString, 256, stdin);  //fgets to get input string

        if (strcmp(inputString, quit) == 0) {  //return 0 when "quit" typed in terminal
            return 0;
        }

        token = strtok(inputString, " ,\t\n");  //get first argument from input string that is divided by space
        args[startArg++] = token;               //write first argument into args array
        while (token != NULL) {                 //while loop to write other arguments from the string
            token = strtok(NULL, " ,\t\n");
            args[startArg++] = token;
        }
        args[startArg++] = NULL;  //add NULL at the and of args

        if ((childPid = fork()) < 0) {  //create child
            perror("fork()");           //display error if couldn't fork
        }

        if (childPid == 0) {
            execvp(args[0], args);              //run execvp with our args array
            printf("\n---execvp failed---\n");  //print message if execvp wasn't executed
            perror("execvp()");
            return 0;
        }
        sleep(1);  //sleep to have a delay between error message and "command"
    }

    int status;
    waitpid(childPid, &status, 0);  //wait for child
    printf("In the parent process, my pid: %d\n", getpid());

    return 0;
}