#define _GNU_SOURCE

#include <unistd.h>
#include <sys/signalfd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc == 2) {
        //get pid from program arguments
        int receiverPid = atoi(argv[1]);
        char* path;
        //create path with format giroXXXX
        asprintf(&path, "/tmp/giro%d", receiverPid);
        char* buffer;
        do {
            buffer = malloc(200);
            //read standard input
            buffer = fgets(buffer, 200, stdin);
            //fgets return NULL on ^D
            if (buffer != NULL) {
                FILE* file = fopen(path, "w");
                if (file != NULL) {
                    fputs(buffer, file);
                    free(buffer);
                    fclose(file);
                    //send signal SIGURS1 to receiver
                    kill(receiverPid, SIGUSR1);
                    sigset_t mask;
                    //initialize signal mask
                    sigemptyset(&mask);
                    sigaddset(&mask, SIGUSR2);
                    //block default handler for SIGUSR2
                    sigprocmask(SIG_BLOCK, &mask, NULL);
                    int sig;
                    //wait for a SIGURS2
                    sigwait(&mask, &sig);
                } else {
                    printf("Error opening the file %s \n", path);
                }
            }
        } while (buffer != NULL);
        //send a SIGUSR2 to receiver
        kill(receiverPid, SIGUSR2);
    } else {
        printf("Parameters error: pid required\n");
    }
    return 0;
}