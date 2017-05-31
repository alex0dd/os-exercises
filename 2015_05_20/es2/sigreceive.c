#define _GNU_SOURCE
#include <unistd.h>
#include <sys/signalfd.h>
#include <signal.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int pid = getpid();
    printf("pid: %d\n",pid);
    char* path;
    //create path with format giroXXXX
    asprintf(&path, "/tmp/giro%d", pid);
    sigset_t mask;
    //initialize signal mask
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    //block default handler for SIGUSR1 and SIGUSR2
    sigprocmask(SIG_BLOCK, &mask, NULL);
    int fd;
    //get a file descript for signal SIGUSR2 and SIGUS2
    fd = signalfd(-1,&mask,0);
    //create a poll structur for waiting signal USR1 and USR2
    struct pollfd polls = {.fd = fd,.events=POLLIN};
    //bool for checking is a sigsr2 arrived
    int sigusr2 = 0;
    while (!sigusr2) {
        //wait for signal
        poll(&polls,1,-1);
        //if a signal arrived
        if (polls.revents & POLLIN) {
            struct signalfd_siginfo siginfo;
            //read signal information from signal file descriptor
            read(fd, &siginfo, sizeof(struct signalfd_siginfo));
            if (siginfo.ssi_signo == SIGUSR1) {
                FILE* file = fopen(path, "r");
                if (file!=NULL) {
                    char* buffer = malloc(50);
                    //read from file
                    fgets(buffer, 50, file);
                    //print to standard output
                    printf("%s", buffer);
                    free(buffer);
                    fclose(file);
                    //remove file
                    remove(path);
                    //send a USR2 signal to sender of signal
                    kill(siginfo.ssi_pid, SIGUSR2);
                } else {
                    printf("Error opening the file %s \n",path);
                }
            } else if (siginfo.ssi_signo == SIGUSR2 ) {
                sigusr2 = 1;
            }
            polls.revents = 0;
        }
    }
    return 0;
}