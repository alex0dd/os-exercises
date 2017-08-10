#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


void init(pid_t pid, int signumber){
	//child's pid
    pid_t childpid;
    //child's return status
	int status;
	printf("pid: %d sig: %d\n",pid, signumber);
	switch(childpid = fork()){
		case -1:
			printf("Error when forking\n");
			break;
		default:
			//wait for child, get his return status inside "status"
		    if(waitpid(childpid,&status,0) == -1){
		    	printf("Can't perform waitpid\n");
		    	exit(1);
		    }
		    if(WIFEXITED(status)){
		    	printf("Child process returned normally\n");
		    }
		    else if(WIFSIGNALED(status)){
				printf("Child process terminated by signal\n");
		    }
		    break;
		case 0:
	        //child
		    printf("Child\n");
	    	kill(pid, signumber);
	    	//sleep(1);
	    	printf("exiting\n");
			break;
	}
}

int main(int argc, char const *argv[])
{
	if(argc == 3){
		init(atoi(argv[1]),atoi(argv[2]));
    }
    else{
    	printf("Please insert a process id and a signal number\n");
    }
	return 0;
}