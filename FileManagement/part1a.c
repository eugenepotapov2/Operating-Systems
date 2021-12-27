#include <errno.h>
#include <stdio.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    char *filepath = argv[1];
    int returnval;
    // Check file existence
    returnval = access(filepath, F_OK);
    if (returnval == 0)
        printf("\n %s exists\n", filepath);
    else {
        if (errno == ENOENT)
            printf("\n%s does not exist\n", filepath);
        else if (errno == EACCES)
            printf("\n%s is not accessible\n", filepath);
        return 0;
    }
    // Check read access
    returnval = access(filepath, R_OK);
    if (returnval == 0)
        printf("\n %s has read access\n", filepath);
    else {
        if (errno == EACCES) printf("\n %s has no read access\n", filepath);
    }
    // Check write access
    returnval = access(filepath, W_OK);
    if (returnval == 0)
        printf("\n %s has write access\n", filepath);
    else {
        if (errno == EACCES) printf("\n%s has no write access\n", filepath);
    }
    return 0;
}