#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#define MAXSIZE 4096

int main(int argc,char *argv[]) {
    int sharedFD;
    //create shared memory object
    if ((sharedFD = shm_open("/sharedPID", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) != -1) {
        ftruncate(sharedFD, MAXSIZE);
        #define OFFSET 0
        #define ADDRESS NULL
        char* sharedVariable = mmap(ADDRESS, MAXSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, sharedFD, OFFSET);
        int effectivePID = atoi(sharedVariable);
        printf("pid = %d\n",effectivePID);
        shm_unlink("/sharedPID");
        int i;
        for (i=0;i<MAXSIZE;i++) sharedVariable[i]='\0';
        for (i = 1; i < argc; i++) {
            strcat(sharedVariable, argv[i]);
            strcat(sharedVariable, " ");
        }
        kill(effectivePID, SIGUSR1);
        return 0;
    } else {
        perror(NULL);
    }
}