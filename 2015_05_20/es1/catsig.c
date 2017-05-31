#include <unistd.h>
#include <sys/signalfd.h>
#include <signal.h>
#include <poll.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    //declare a mask for signal
    sigset_t mask;
    //initialize signal mask
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    //create a file descriptor for signal USR1
    int signalFileDescriptor = signalfd(-1, &mask, 0);
    //remove default handler for SIGURS1
    sigprocmask(SIG_BLOCK, &mask, NULL);
    struct pollfd pollstruct[2];
    pollstruct[0].fd = signalFileDescriptor;
    pollstruct[0].events = POLLIN;
    pollstruct[1].fd = STDIN_FILENO;
    pollstruct[1].events = POLLIN;
    while (1) {
        poll(pollstruct, 2, -1);
        //is signal USR1 is received
        if (pollstruct[0].revents & POLLIN) {
            struct signalfd_siginfo siginfo;
            //read signal information from signal file descriptor
            read(signalFileDescriptor, &siginfo, sizeof(struct signalfd_siginfo));
            printf("ho ricevuto un segnale\n");
            //set event as handled
            pollstruct[0].revents = 0;
        }
        //if there is something to read in standard input
        if (pollstruct[1].revents & POLLIN) {
            char character;
            //read one character until there is a character to read
            while (read(fileno(stdin), &character, 1) > 0) {
                //print that character to standard output
                write(fileno(stdout), &character, 1);
            }
            //set event as handled
            pollstruct[1].revents = 0;
        }
    }
}