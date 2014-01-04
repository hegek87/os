#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include "file_comp.h"

void usage(void){
	printf("./file_comp <directory-name>\n");
}

int main(int argc, char **argv){
	struct stack *to_visit = create_stack();
	struct dl_list *visited = create_empty_list();
	struct stat cur_obj;
	char *path = NULL;
	if(argc > 2 || argc < 1){
		usage();
		return -1;
	}
	//start search at specified directory
	if(argc == 2){
		path = *(argv+1);
		printf("%s\n", path);
		push_key(to_visit, (void *)path);
		//char *x = (char *)(pop(to_visit)->data);
		//printf("%s\n", x);
	}
	//start search at current working directory
	else{
		push_key(to_visit, ".");
	}
	
	while(!is_stack_empty(to_visit)){
		printf("In processing loop\n");
		char *cur = (char *)(pop(to_visit)->data);
		//printf("%s\n", cur);
		if(lstat(cur, &cur_obj)){
			printf("\n\n%s\n", cur);
			perror("Failed to stat object\n");
			return -1;
		}
		mode_t x = cur_obj.st_mode & S_IFMT;
		if(x == S_IFLNK){
			printf("Is Link\n");
			continue;
		}
		else if(x == S_IFDIR){
			DIR *cd;
			struct dirent *dirp;
			if(!(cd = opendir(cur))){
				printf("Error opening %s\n", cur);
				return -1;
			}
			printf("Found Directory\n");
			while((dirp = readdir(cd))){
				//construct full path
				//printf("%s\n", dirp->d_name);
				//skip .
				if(!strcmp(dirp->d_name, ".")){
					continue;
				}
				//skip ..
				if(!strcmp(dirp->d_name, "..")){
					continue;
				}
				//add other files to stack
				push_key(to_visit, dirp->d_name);
			}			
		}
		else if(x == S_IFREG){
			printf("Found File\n");
			printf("%s\n", cur);
		}
	}
	is_empty(visited);
	//printf("HI\n");
	return 0;
}
