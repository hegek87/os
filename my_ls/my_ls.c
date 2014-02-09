#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include "my_ls.h"

int process_path(char *path, char flag){
	DIR *cd;
	struct dirent *dirp;
	struct stat c_obj;
	
	if(!(cd = opendir(path))){
		perror("Failed to open directory");
		return -1;
	}
	while((dirp = readdir(cd))){
		if(dirp->d_name[0] == '.' && (flag - 'a')){ continue; }
		int p_len = strlen(dirp->d_name) + strlen(path)+2;
		char *new_path = calloc(p_len, sizeof(char));
		strcpy(new_path, path);
		strcat(new_path, "/");
		strcat(new_path, dirp->d_name);
		if(lstat(new_path, &c_obj)){
			printf("Failed to stat\n");
			return -1;
		}
		mode_t x = c_obj.st_mode;
		if(S_ISDIR(x)){
			printf(BLUE "%-11s" RESET, dirp->d_name);
		}
		else if((x & S_IXUSR) || (x & S_IXOTH) || (x & S_IXGRP)){
			printf(GREEN "%-11s" RESET, dirp->d_name);
		}
		else{
			printf("%-11s", dirp->d_name);
		}
	}
	printf("\n");
	return 0;
}

int main(int argc, char **argv){
	char *path;
	
	//no flag support yet
	if(argc == 2){
		path = *(argv + 1);
	}
	else{
		path = ".";
	}
	if(process_path(path, 'a')){
		perror("Failed to process directory");
		exit(-1);
	}
	return 0;
}
