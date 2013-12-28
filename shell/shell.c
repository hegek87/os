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
		char *command = NULL;
		char host[BUFFER_SIZE];
		char c_dir[BUFFER_SIZE];
		if(gethostname(host, BUFFER_SIZE) == -1){
			perror("Failed to get host name.");
			exit(-1);
		}
		if(!getcwd(c_dir, BUFFER_SIZE)){
			perror("Failed to get working directory.");
			exit(-1);
		}
		printf("%s@%s %s %% ", user_data->pw_name, host, c_dir);
		size_t len = 0;
		ssize_t in;
		if((in = getline(&command, &len, stdin)) == -1){
			perror("Line not read");
			exit(-1);
		}
		//parse command
		printf("%s\n", command);
		char *parsed_cmd[BUFFER_SIZE];
		char *token, *save_ptr;
		int i;
		char *delim = " \t";
		for(i = 0, token = command; ; ++i, token = NULL){
			parsed_cmd[i] = strtok_r(token, delim, &save_ptr);
			if(!parsed_cmd[i]){
				break;
			}
		}
		i = 0;
		while(parsed_cmd[i]){
			*(parsed_cmd[i]+(strlen(parsed_cmd[i])-1))='\0';
			++i;
		}
		
		//If command is cd
		if(!strcmp(parsed_cmd[0], "cd")){
			printf("Comp success");
			if(chdir(parsed_cmd[1])){
				if(*(parsed_cmd[1]+2)=='\n'){
					printf("YES");
				}
				else{
					printf("NO");
				}
				printf("%s : No such file or directory exists\n", parsed_cmd[1]);
			}
		}
		//command is exit
		else if(strcmp(parsed_cmd[0], "exit")){
			printf("Exiting\n");
			exit(0);
		}
		//fork and execute command otherwise
		//else{}
		//printf("%s\n", in);
	}
	return 0;
}
