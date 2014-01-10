#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <strings.h>
#include "my_ls.h"

int process_path(char *path){
	DIR *cd;
	struct dirent *dirp;
	
	if(!(cd = opendir(path))){
		perror("Failed to open directory");
		return -1;
	}
	
	int i = 0;
	while((dirp = readdir(cd))){
		if(!strcmp(dirp->d_name, ".") 
			|| !strcmp(dirp->d_name, "..")){
			continue;
		}
		printf("%s\t\t", dirp->d_name);
		if(++i == 3){
			printf("\n");
			i = 0;
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
	//if(no flags){
	process_path(path);
	//}
	return 0;
}
