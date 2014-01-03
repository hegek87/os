#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "file_comp.h"

void usage(void){
	printf("./file_comp <directory-name>\n");
}

int main(int argc, char **argv){
	struct stack *to_visit = create_stack();
	struct dl_list *visited = create_empty_list();
	struct stat cur_obj;
	if(argc > 2 || argc < 1){
		usage();
		return -1;
	}
	//start search at specified directory
	if(argc == 2){
		char *path = *(argv+1);
		printf("%s\n", path);
		push_key(to_visit, path);
	}
	//start search at current working directory
	else{
		push_key(to_visit, ".");
	}
	
	while(!is_stack_empty(to_visit)){
		char *cur = (char *)pop(to_visit);
		lstat(cur, &cur_obj);
		mode_t x = cur_obj.st_mode & S_IFMT;
		if(x == S_IFLNK){
			continue;
		}
		else if(x == S_IFDIR){
			DIR *cd;
			if(!(cd = opendir(cur))){
				printf("Error opening %s\n", cur);
				return -1;
			}
		}
		else if(x == S_IFREG){
			
		}
	}
	is_empty(visited);
	printf("HI\n");
	return 0;
}
