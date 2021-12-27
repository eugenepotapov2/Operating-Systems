#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    FILE *fp;                   //file pointer to use fopen
    fp = fopen(argv[1], "r+");  //open argv[1]
    int scanned_num;
    int num_of_grades = 0;
    int num_of_hw = 0;
    int num_of_chap = 0;
    while (fscanf(fp, "%d", &scanned_num) == 1) {  //use while loop with fscanf to get total number of grades
        num_of_grades++;
    }
    int temp_counter = 0;
    int array_grades[num_of_grades];               //declare array of size num_of_grades that we get from the while loop above
    rewind(fp);                                    //rewind file to use fscanf again
    while (fscanf(fp, "%d", &scanned_num) == 1) {  //use while loop with fscanf again to fill the array with the numbers from the file
        array_grades[temp_counter++] = scanned_num;
    }
    num_of_hw = num_of_grades / 10;             //get the total number of homeworks
    for (int i = num_of_hw - 1; i >= 2; i--) {  //for loop to equally spread homeworks per chapter (tries to spread the smallest possible number of homeworks, for example 2 homeworks per 5 chapters, not 5 homeworks per 2 chapters)
        if (num_of_hw % i == 0) {
            num_of_chap = i;
            break;
        }
    }
    if (num_of_chap == 0) {  //return 0 if number of homeworks cannot be spread equally
        printf("Number of homeworks must be the same for each chapter\n");
        return 0;
    }
    pid_t manager[num_of_chap];
    for (int i = 0; i < num_of_chap; i++) {  //for loop to create managers (one for each chapter)
        if ((manager[i] = fork()) < 0) {     //perror statement
            perror("fork()");
        } else if (manager[i] == 0) {  //inside the manager
            pid_t worker[num_of_hw / num_of_chap];
            for (int j = 0; j < num_of_hw / num_of_chap; j++) {  //for loop to create workers (one for each homework in the same chapter)
                int sum = 0;
                if ((worker[j] = fork()) < 0) {  //perror statement
                    perror("fork()");
                } else if (worker[j] == 0) {                                                               //inside the worker
                    for (int z = i * (num_of_hw / num_of_chap) + j; z < num_of_hw * 10; z += num_of_hw) {  //for loop to calculate the sum for the current homework in the current chapter
                        sum = sum + array_grades[z];                                                       //starting index is chapter index multipled by number of homeworks per chapter + hw index
                    }                                                                                      //(for example for the total of 9 hw, 9 workers will start from 0, 1, 2, 3, 4, 5, 6, 7, 8 respectively)
                    printf("Average for X%dY%d = %.1f\n", i + 1, j + 1, (float)sum / 10);                  //print average
                    return 0;
                }
            }
            for (int j = 0; j < num_of_hw / num_of_chap; j++) {  //for loop to wait for workers
                waitpid(worker[j], NULL, 0);
            }
            return 0;
        }
    }
    for (int i = 0; i < num_of_chap; i++) {  //for loop to wait for managers
        waitpid(manager[i], NULL, 0);
    }
    return 0;
}