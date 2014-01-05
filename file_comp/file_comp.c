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
//-1 is an error, 0 is success
int walk_dir(char *path){
	DIR *cd;
	struct dirent *dirp;
	if(!(cd = opendir(path))){
		printf("Error opening %s\n", path);
		return -1;
	}
	while((dirp = readdir(cd))){
		//skip '.'
		if(!strcmp(dirp->d_name, ".")){
			continue;
		}
		//skip '..'
		if(!strcmp(dirp->d_name, "..")){
			continue;
		}
		if(!strcmp(path, "/")){
			int p_len = strlen(dirp->d_name) + 2;
			char *new_path = calloc(p_len, sizeof(char));
			strcpy(new_path, path);
			strcat(new_path, dirp->d_name);
			push_key(to_visit, new_path);
		}
		else if(strcmp(path, ".") || strcmp(path, "..")){
			int p_len = strlen(path) + strlen(dirp->d_name)+2;
			char *new_path = calloc(p_len, sizeof(char));
			strcpy(new_path, path);
			strcat(new_path, "/");
			strcat(new_path, dirp->d_name);
			push_key(to_visit, new_path);
		}
		else{
			push_key(to_visit, dirp->d_name);
		}
	}
	closedir(cd);
	return 0;	
}

//0 is success, -1 is error
int process(char *path){
	struct stat cur_obj;
	if(lstat(path, &cur_obj)){
		perror("Failed to stat object\n");
		return -1;
	}
	mode_t x = cur_obj.st_mode;
	if(S_ISLNK(x)){
		return 1;
	}
	else if(S_ISDIR(x)){
		walk_dir(path);		
	}
	else{
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
	//start search at current working directory (default behavior)
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
		printf("%d. %s\n", (i+1), (char *)runner->data);
		runner = runner->next;
	}
	return 0;
}
