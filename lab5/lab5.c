#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int counter;
pid_t newPid = 0;
FILE* file = NULL;
const char filename[] = "log.txt";
const char counterFilename[] = "counter.txt";

void signalHandler(int signum) {
    newPid = fork();
    if (newPid == -1) {
        printf("fork failed");
        fclose(file);
    }
    else if (newPid == 0) {
        newPid = getpid();
    }
    else {
        printf("process forked\n");
        fprintf(file, "Process %d forked to  %d\n", getpid(), newPid);
        FILE* counterFile = fopen(counterFilename, "w");
        fprintf(counterFile, "%d", counter);
        fclose(counterFile);
        fprintf(file, "Pid %d killed\n", getpid());
        fclose(file);
        kill(getpid(), 9);
    }    
}

int main() {    
    FILE* counterFile = fopen(counterFilename, "r");
    fscanf(counterFile, "%d", &counter);    
    fclose(counterFile);

    file = fopen(filename, "a");    
    signal(SIGINT, signalHandler);
    struct timespec delta = {1, 0};
    fprintf(file, "Pid %d main\n", getpid());
    while (counter > 0) {
        fprintf(file, "Pid %d: counter %d\n", getpid(), counter);
        if(!nanosleep(&delta, NULL)) {
            counter--;
        }
    }
    fprintf(file, "Program executed\n\n");
    fclose(file);
}