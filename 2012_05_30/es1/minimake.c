#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <execs.h>

int main(int argc, char* argv[]) {
    FILE* file;
    if ((file = fopen("minimake", "r")) != NULL) {
        char target[256], path[256], args[256];
        while (fscanf(file, "%40[^:]: %99[^\n]\n", target, path) != EOF) {
            //check if target file exist
            if (access(target, F_OK) != 0) {
                //if not fork and exec command
                int status;
                switch (fork()) {
                    case 0:
                        execsp(path);
                        break;
                    default:
                        wait(&status);
                        break;
                }
            }
        }
        fclose(file);
    } else {
        perror(NULL);
    }
    return 0;
}