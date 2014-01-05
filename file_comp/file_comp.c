#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#include "file_comp.h"

static struct stack *to_visit;
static struct dl_list *visited;

void usage(void){
	printf("./file_comp <directory-name>\n");
}

//0 is success, -1 is error
int process(char *path){
	struct stat cur_obj;
	//printf("Stack size: %d\n", stack_size(to_visit));
	//char *path = (char *)(pop(to_visit)->data);
	//printf("Statting: %s\n", path);
	if(lstat(path, &cur_obj)){
		//printf("\n\n%s\n", path);
		perror("Failed to stat object\n");
		return -1;
	}
	mode_t x = cur_obj.st_mode & S_IFMT;
	if(x == S_IFLNK){
		//printf("Is Link\n");
		return 1;
	}
	else if(x == S_IFDIR){
		DIR *cd;
		struct dirent *dirp;
		if(!(cd = opendir(path))){
			printf("Error opening %s\n", path);
			return -1;
		}
		//printf("Found Directory\n");
		//printf("Opening: %s\n", path);
		while((dirp = readdir(cd))){
			//skip .
			if(!strcmp(dirp->d_name, ".")){
				//printf("Found '.'\n");
				continue;
			}
			//skip ..
			if(!strcmp(dirp->d_name, "..")){
				//printf("Found '..'\n");
				continue;
			}
			if(strcmp(path, ".") || strcmp(path, "..")){
				int p_len = strlen(path) + strlen(dirp->d_name)+2;
				char *new_path = calloc(p_len, sizeof(char));
				strcpy(new_path, path);
				strcat(new_path, "/");
				strcat(new_path, dirp->d_name);
				//printf("Pushing: %s\n", new_path);
				push_key(to_visit, new_path);
			}
			else{
				//printf("Pushing: %s\n", dirp->d_name);
				push_key(to_visit, dirp->d_name);
			}
			//printf("Stack size: %d\n", stack_size(to_visit));
		}
		closedir(cd);		
	}
	else{
		//printf("Found File\n");
		//printf("%s\n", path);
		insert_el_head(visited, path);
	}
	return 0;
}

int main(int argc, char **argv){


	to_visit = create_stack();
	visited = create_empty_list();
	
	char *path = NULL;
	if(argc > 2 || argc < 1){
		usage();
		exit(-1);
	}
	//start search at specified directory
	if(argc == 2){
		path = *(argv+1);
	}
	//start search at current working directory
	else{
		path = ".";
	}
	push_key(to_visit, path);
	
	char *cur;
	while(!is_stack_empty(to_visit)){
		cur = (char *)(pop(to_visit)->data);
		if(process(cur) == -1){
			break;
		}
	}
	
	printf("Visited empty? %d\n", is_empty(visited));
	int i;
	struct node *runner = visited->head;
	for(i = 0; i < visited->size; ++i){
		printf("%s\n", (char *)runner->data);
		runner = runner->next;
	}
	return 0;
}
