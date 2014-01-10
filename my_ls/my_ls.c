#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include "my_ls.h"

int process_path(char *path){
	DIR *cd;
	struct dirent *dirp;
	struct stat c_obj;
	
	if(!(cd = opendir(path))){
		perror("Failed to open directory");
		return -1;
	}
	//printf("OPENED DIR\n");
	while((dirp = readdir(cd))){
		//printf("READING DIR\n");
		if(!strcmp(dirp->d_name, ".") 
			|| !strcmp(dirp->d_name, "..")
			|| dirp->d_name[0] == '.'){
			continue;
		}
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
		else if((x & S_IXUSR) != 0 
				|| (x & S_IXOTH) != 0 
				|| (x & S_IXGRP) != 0){
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
	//printf("PATH: %s\n", path);
	//if(no flags){
	if(process_path(path)){
		perror("Failed to process directory");
		exit(-1);
	}
	//}
	return 0;
}
