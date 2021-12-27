#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "sem.h"

int main() {
    srand(time(NULL));
    int mutex = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);           // mutex to lock critical section between all processes
    int smoker_match = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);    // semaphore to let smoker with matches smoke
    int smoker_paper = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);    // semaphore to let smoker with paper smoke
    int smoker_tobacco = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);  // semaphore to let smoker with tobacco smoke
    int agent = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);           // semaphore to make agent sleep when while smoker rolls a sigarette

    sem_create(mutex, 1);
    sem_create(smoker_match, 0);
    sem_create(smoker_paper, 0);
    sem_create(smoker_tobacco, 0);
    sem_create(agent, 0);

    pid_t agent_p;

    int counter = 10;  // counter to place only 10 items by agent
    pid_t smoker[3];
    if ((agent_p = fork()) < 0) {
        perror("fork()");
    }
    if (agent_p == 0) {  // create agent process
        if ((smoker[0] = fork()) < 0) {
            perror("fork()");
        }
        if (smoker[0] == 0) {  // create child smoker process inside agent
            while (true) {
                P(smoker_match);  // decrease semaphore when paper and tobacco are on the table
                P(mutex);         // lock critical section
                int r = rand() % 6 + 1;
                printf("\nSmoker with matches is smoking %ds\n", r);
                V(agent);  // signal agent about finishing rolling
                V(mutex);  // unlock critical section
                sleep(r);  // smoke some time
            }
            exit(0);
        }

        if ((smoker[1] = fork()) < 0) {
            perror("fork()");
        }
        if (smoker[1] == 0) {  // second child process
            while (true) {
                P(smoker_paper);  // decrease semaphore when matches and tobacco are on the table
                P(mutex);
                int r = rand() % 6 + 1;
                printf("\nSmoker with paper is smoking %ds\n", r);
                V(agent);
                V(mutex);
                sleep(r);
            }
            exit(0);
        }

        if ((smoker[2] = fork()) < 0) {
            perror("fork()");
        }
        if (smoker[2] == 0) {
            while (true) {
                P(smoker_tobacco);  // decrease semaphore when matches and paper are on the table
                P(mutex);
                int r = rand() % 6 + 1;
                printf("\nSmoker with tobacco is smoking %ds\n", r);
                V(agent);
                V(mutex);
                sleep(r);
            }
            exit(0);
        }
        while (counter) {  // agent part
            P(mutex);      // lock critical section
            printf("\n%d ---Agent starts distributing ingredients---\n", counter);
            int rand_num = rand() % 3 + 1;  // generate random number from 1 to 3 to put two ingredients on table
            if (rand_num == 1) {
                printf("Agent places tobacco and paper\n");
                V(smoker_match);  // increase semaphore for smoker with matches
            } else if (rand_num == 2) {
                printf("Agent places tobacco and matches\n");
                V(smoker_paper);  // increase semaphore for smoker with paper
            } else {
                printf("Agent places paper and matches\n");
                V(smoker_tobacco);  // increase semaphore for smoker with tobacco
            }
            counter--;  // decrease counter
            V(mutex);   // unlock critical section
            P(agent);   // agent sleeps until one of the smokers sends a signal
        }

        kill(smoker[0], SIGTERM);  // using kill to terminate smoker processes, otherwise they will never terminate because of the infinite while loop
        kill(smoker[1], SIGTERM);
        kill(smoker[2], SIGTERM);
        exit(0);
    }
    waitpid(agent_p, 0, 0);  // waiting for agent process
    return 0;
}
