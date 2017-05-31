#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>

#define MAXSIZE 4096

int main(int argc, char* argv[]) {
    int sharedFD;
    printf("pid = %d\n", getpid());
    //create shared memory object
    if ((sharedFD = shm_open("/sharedPID", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) != -1) {
        //set size of shared memory object
        if (ftruncate(sharedFD, MAXSIZE) == 0) {
            //create a memory mapping with sharedVariable and the shared memory object
            #define OFFSET 0
            #define ADDRESS NULL
            char* sharedVariable = mmap(ADDRESS, MAXSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, sharedFD, OFFSET);
            if (sharedVariable != MAP_FAILED) {
                sprintf(sharedVariable, "%d\n", getpid());
                sigset_t mask;
                //initialize signal mask
                sigemptyset(&mask);
                sigaddset(&mask, SIGUSR1);
                //block default handler for SIGUSR1
                sigprocmask(SIG_BLOCK, &mask, NULL);
                int sig;
                //wait for a SIGURS1
                sigwait(&mask, &sig);
                printf("%s\n", sharedVariable);
                shm_unlink("/sharedPID");
                return 0;
            } else {
                perror(NULL);
            }
        } else {
            perror(NULL);
        }
    } else {
        perror(NULL);
    }

}