#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	if (argc!=2){
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(1);
	}

	return system(argv[1]);
}

int system(const char * command){
	int ret = -1;
	int pid = fork();

	if(pid == -1)
		fprintf(stderr, "Fallo fork\n");

	else if(pid == 0)
		execlp("/bin/bash", "bash", "-c", command, (char*) NULL);
	
	else{
		wait(&ret);
		ret = WEXITSTATUS(ret);
	}
}
