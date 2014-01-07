#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>
#include <stdlib.h>

#define RESET_COLOR 	"\033[0m"
#define GREEN		"\e[1;32m"
#define BLUE		"\e[1;34m"
#define RED			"\e[1;31m"
#define BUFFER_SIZE 	4096

int process_cmd(char **);
void print_prompt(void);
void parse_command(char *[], char *);

int main(void){
	while(1){
		print_prompt();
		char *command = NULL;
		size_t len = 0;
		ssize_t in;
		if((in = getline(&command, &len, stdin)) == -1){
			perror("Line not read");
			exit(-1);
		}
		//parse command
		char *parsed_cmd[BUFFER_SIZE];
		parse_command(parsed_cmd, command);
		int i = 0;
		
		//If command is cd
		if(!strcmp(parsed_cmd[0], "cd")){
			//remove '\n' from path
			*(parsed_cmd[1]+(strlen(parsed_cmd[1])-1)) = '\0';
			if(chdir(parsed_cmd[1])){
				printf("%s : No such file or directory exists\n"
					, parsed_cmd[1]);
			}
		}
		//command is exit
		else if(!strcmp(parsed_cmd[0], "exit\n")){
			exit(0);
		}
		//fork and execute command otherwise
		else{
			//find last element and strip off '\n'
			while(parsed_cmd[i])
				++i;
			int last_el = i-1;
			*(parsed_cmd[last_el]+(strlen(parsed_cmd[last_el])-1)) = '\0';
			process_cmd(parsed_cmd);
		}
	}
	return 0;
}

int process_cmd(char **parsed_cmd){
	pid_t child = fork();
	int status;
	if(!child){
		execvp(parsed_cmd[0], parsed_cmd);
	}
	else{
		wait(&status);
	}
	return status;
}

void print_prompt(void){
	struct passwd *user_data = getpwuid(getuid());
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
	printf(GREEN "%s@%s " BLUE "%s $ " RESET_COLOR,
				 user_data->pw_name, host, c_dir);
}	

void parse_command(char *parsed_cmd[], char *command){
	char *token, *save_ptr;
	int i;
	char *delim = " \t";
	for(i = 0, token = command; ; ++i, token = NULL){
		parsed_cmd[i] = strtok_r(token, delim, &save_ptr);
		if(!parsed_cmd[i]){
			break;
		}
	}
}


