#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sig_handler(int signo, siginfo_t* info, void* vp)
{
    int killResult;
    pid_t signaler = info->si_pid;
    printf("Got signaled by %d\n",signaler);
    killResult = kill(signaler, SIGKILL);
    if(killResult < 0){
        printf("Failed to kill %d, it was stronger than me..\n", signaler);
    }
    else{
        printf("Hey %d, don't signal me!\n", signaler);
    }
}

void init(){
    printf("PID: %d\n", getpid());
    struct sigaction sa;
    //set signal handler
    sa.sa_sigaction = &sig_handler;
    //get all the signals from &sa.sa_mask
    sigfillset(&sa.sa_mask);
    //signals are numbered from 1 till SIGRTMAX
    int i = 1;
    for(i; i < SIGRTMAX; i++){
        //SIGKILL and SIGSTOP can't be caught
        if(i != SIGKILL && i != SIGSTOP){
            //add sigaction to the i'th signal
            if (sigaction(i, &sa, NULL) == -1) {
                //if failed adding signal handler
                printf("Signal %d can't be caught\n",i);
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    init();
    while (1) {
        sleep(1);
    }
	return 0;
}