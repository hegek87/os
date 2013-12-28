#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

int main(void){
	while(1){
		char in[BUFFER_SIZE];
		/*
		pid_t result = fork();
		int status;
		//pid_t parent = getpid();
		char *pwd_arg[2] = {"pwd", NULL};
		if(result == 0){
			execvp("pwd", pwd_arg);
		}
		*/
		struct passwd *user_data = getpwuid(getuid());
		char host[BUFFER_SIZE];
		char c_dir[BUFFER_SIZE];
		int host_res = gethostname(host, BUFFER_SIZE);
		if(host_res == -1){
			perror("Failed to get host name.");
			exit(-1);
		}
		if(!getcwd(c_dir, BUFFER_SIZE)){
			perror("Failed to get working directory.");
			exit(-1);
		}
		printf("%s@%s %s %% ", user_data->pw_name, host, c_dir);
		scanf("%s", in);
		printf("%s\n", in);
	}
	return 0;
}
