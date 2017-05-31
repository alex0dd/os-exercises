#define _GNU_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char* argv[]) {
    if (argc == 3) {
        //get file descriptors
        int fdFile2 = open(argv[2], O_WRONLY);
        int fdFile1 = open(argv[1], O_RDONLY);
        //create structure for file locking
        struct flock flock;
        struct stat stat_buf;
        fstat(fdFile1, &stat_buf);
        //type = write lock
        flock.l_type = F_WRLCK;
        flock.l_start = 0;
        //set l_start relative of beginning of file
        flock.l_whence = SEEK_SET;
        flock.l_len = stat_buf.st_size;
        flock.l_pid = getpid();
        fcntl(fdFile2, F_SETLKW, &flock);
        //print "locked"
        printf("locked\n");
        //use sendfile for copy, use fdFile1 offset and copy all file 1
        sendfile(fdFile2, fdFile1, NULL, stat_buf.st_size);
        //wait for a random time between 1 and 10 seconds
        srand(time(NULL));
        int r = rand() % 10 + 1;
        sleep(r);
        //unclock second file
        flock.l_type = F_UNLCK;
        fcntl(fdFile2, F_SETLKW, &flock);
        //print "unlocked"
        printf("unlocked\n");
        //wait for a random time between 1 and 10 second
        r = rand() % 10 + 1;
        sleep(r);
        return 0;
    } else {
        printf("wrong number of arguments!");
        return -1;
    }
}