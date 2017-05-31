#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <utime.h>

int main(int argc, char* argv[]) {
    int access = 0;
    int modification = 0;
    int c;
    int index = 0;
    time_t time = 10 * 24 * 60 * 60;
    while ((c = getopt(argc, argv, "amt:h")) != -1) {
        switch (c) {
            case 'a': access=1; break;
            case 'm': modification=1; break;
            case 't':
                time = strtol(optarg,NULL,10);
                break;
            case 'h':
            case '?':
                printf("usage: ager [-a] [-m] [-t time] [file ...]");
            default:
                abort ();
        }
    }
    for (index = optind; index < argc; index++) {
        struct stat structstat;
        if (stat(argv[index], &structstat) == 0) {
            //read last access time
            time_t atim = structstat.st_atim.tv_sec;
            //read last modification time
            time_t mtim = structstat.st_mtim.tv_sec;
            if (!modification || access) {
                atim -= time;
            }
            if (!access || modification) {
                mtim -= time;
            }
            //create structure for utime
            struct utimbuf structutimbuf = {atim, mtim};
            //call utime for setting the new date
            if (utime(argv[index], &structutimbuf) != 0) {
                //if utime failed print errno
                perror(NULL);
            }
        } else {
            //if stat failed print errno
            perror(NULL);
        }
    }
    return 0;
}