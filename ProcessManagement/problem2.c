#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    int a = 10, b = 25, fq = 0, fr = 0;

    fq = fork();  //creates child Q
    if (fq == 0) {
        a = a + b;
        printf("a: %d, b: %d, process_id: %d\n", a, b, getpid());
        fr = fork();  //creates child R
        if (fr != 0) {
            b = b + 20;                                                //this still happens in Q, so the taken value of b is 25
            printf("a: %d, b: %d, process_id: %d\n", a, b, getpid());  //prints values of a and b of Q child
        } else {
            a = (a * b) + 30;  //takes the values of Q-parent (a=25 and b=25) because R was created in Q
            printf("a: %d, b: %d, process_id: %d\n", a, b, getpid());
        }

    } else {
        b = a + b - 5;  //takes parent values of a and b
        printf("a: %d, b: %d, process_id: %d\n", a, b, getpid());
    }
    return 0;
}