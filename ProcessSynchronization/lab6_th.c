#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define NUM_THREADS 4

pthread_t tid[NUM_THREADS];
pthread_mutex_t lock;  // declare mutexes
pthread_mutex_t match;
pthread_mutex_t paper;
pthread_mutex_t tobacco;
pthread_mutex_t agent;
void *runner();        // only one fucntion runner will be used for smokers
void *agent_runner();  // function for agent
int counter = 10;
int rand_num = 0;
int run = 1;  // integer used to terminate smoker threads when run is set to 0

int main() {
    srand(time(NULL));
    pthread_mutex_init(&lock, NULL);  // initialize mutexes
    pthread_mutex_init(&match, NULL);
    pthread_mutex_init(&paper, NULL);
    pthread_mutex_init(&tobacco, NULL);
    pthread_mutex_init(&agent, NULL);

    pthread_mutex_lock(&match);  // lock match, paper and tobacco to stop threads from running before ingredients were distributed
    pthread_mutex_lock(&paper);
    pthread_mutex_lock(&tobacco);
    pthread_mutex_lock(&agent);  // lock agent to prevent it running before smoker rolled a cigarette

    int matches_check = 1;  // passing these parameters to runner function, so that runner function worked specifically for some smoker with one of the ingredients
    int paper_check = 2;
    int tobacco_check = 3;

    if (pthread_create(&tid[0], NULL, &agent_runner, NULL)) {  // create agent thread
        perror("pthread_create");
    }
    if (pthread_create(&tid[1], NULL, &runner, (void *)&matches_check)) {  // thread for smoker with matches
        perror("pthread_create");
    }
    if (pthread_create(&tid[2], NULL, &runner, (void *)&paper_check)) {  // thread for smoker with paper
        perror("pthread_create");
    }
    if (pthread_create(&tid[3], NULL, &runner, (void *)&tobacco_check)) {  // thread for smoker with tobacco
        perror("pthread_create");
    }

    int i = 0;
    while (i < NUM_THREADS) {  // loop to join threads
        if (pthread_join(tid[i], NULL)) {
            perror("pthread_join");
        }
        i++;
    }

    pthread_mutex_destroy(&lock);  // destroying mutexes after joining threads
    pthread_mutex_destroy(&match);
    pthread_mutex_destroy(&paper);
    pthread_mutex_destroy(&tobacco);
    pthread_mutex_destroy(&agent);
    return 0;
}

void *agent_runner() {
    while (counter) {               // while loop to run 10 times
        pthread_mutex_lock(&lock);  // locking critical section
        printf("\n%d ---Agent starts distributing ingredients---\n", counter);
        int rand_num = rand() % 3 + 1;
        if (rand_num == 1) {
            printf("Agent places tobacco and paper\n");
            pthread_mutex_unlock(&match);  // unlocking mutex for particular smoker who can roll cigarette
        } else if (rand_num == 2) {
            printf("Agent places tobacco and matches\n");
            pthread_mutex_unlock(&paper);
        } else {
            printf("Agent places paper and matches\n");
            pthread_mutex_unlock(&tobacco);
        }
        counter--;
        pthread_mutex_unlock(&lock);  // unlocking critical section
        pthread_mutex_lock(&agent);   // here agent will sleep until one the smokers unlocks agent mutex
    }
    run = 0;  // after while loop runs 10 times, set run flag to 0 to terminate smoker threads

    pthread_mutex_unlock(&match);  // unlocking smoker mutexes in case if one of them got stuck on its inner mutex and can't be terminated even when run = 0
    pthread_mutex_unlock(&paper);
    pthread_mutex_unlock(&tobacco);
    pthread_exit(0);  // exit agent
}

void *runner(void *runner_value) {
    int mutex_index = *((int *)runner_value);  // taking passed parameter
    while (run) {                              // while loop that runs until run is set to 1
        if (mutex_index == 1) {                // if condition to use speficic mutex lock based on the passed parameter (1 for matches, 2 for paper, 3 for tobacco)
            pthread_mutex_lock(&match);
            if (run == 0) {  // this if condition is necessary because thread could get stuck on its smoker mutex and won't be terminated even when run is set to 0
                pthread_exit(0);
            }
        } else if (mutex_index == 2) {
            pthread_mutex_lock(&paper);
            if (run == 0) {
                pthread_exit(0);
            }
        } else if (mutex_index == 3) {
            pthread_mutex_lock(&tobacco);
            if (run == 0) {
                pthread_exit(0);
            }
        } else {
            (pthread_exit(0));
        }

        pthread_mutex_lock(&lock);  // lock for critical section
        int r = rand() % 6 + 1;
        if (mutex_index == 1) {  // if condtions to smoke based on ingredient that smokers already have
            printf("\nSmoker with matches is smoking %ds\n", r);
        }
        if (mutex_index == 2) {
            printf("\nSmoker with paper is smoking %ds\n", r);
        }
        if (mutex_index == 3) {
            printf("\nSmoker with tobacco is smoking %ds\n", r);
        }

        pthread_mutex_unlock(&agent);  // unlock agent
        pthread_mutex_unlock(&lock);   // unlock critical section
        sleep(r);                      // smoke random number of seconds
    }
    pthread_exit(0);
}