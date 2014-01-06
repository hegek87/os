#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "file_comp.h"

#define MD5_PATH "objects/md5"

static struct stack *to_visit;
//dl_list where the nodes contain dl_lists
static struct dl_list *visited;

void usage(void){
	printf("./file_comp <directory-name>\n");
}

void print_results(void){
	int i;
	struct node *runner = visited->head;
	for(i = 0; i < visited->size; ++i){
		struct file_data *temp = (struct file_data *)(runner->data);
		printf("HASH: %s\n", temp->digest);
		struct node *cur_node = temp->files->head;
		int j;
		printf("Total matches: %d\n", temp->files->size);
		printf("FILES:\n");
		for(j = 0; j < temp->files->size; ++j){
			printf("\t%s\n", (char *)(cur_node->data));
			cur_node = cur_node->next;
		}
		printf("\n");
		runner = runner->next;
	}
	return;
}
	
/*
 We have a list of lists, if an element is found, insert it into it's 
 containing list. Otherwise add it to a list, and add the list to visited
 Return 1 if added new sublist
 If found and inserted, return 0
*/
int visit(char *digest, char *file_name){
	struct node *cur_node = visited->head;
	//list is empty, value can't be found
	if(!cur_node){
		struct file_data *temp = malloc(sizeof(struct file_data));
		temp->digest = digest;
		struct dl_list *file_list = create_empty_list();
		insert_el_head(file_list, file_name);
		temp->files = file_list;
		insert_el_head(visited, temp);
		return 1;
	}
	while(cur_node){
		struct file_data *temp = (struct file_data *)(cur_node->data);
		if(!strcmp(digest, (char *)(temp->digest))){
			insert_el_head((struct dl_list *)(temp->files),file_name);
			return 0;
		}
		cur_node = cur_node->next;
	}
	//key not found, insert new element
	struct file_data *temp = malloc(sizeof(struct file_data));
	temp->digest = digest;
	struct dl_list *file_list = create_empty_list();
	insert_el_head(file_list, file_name);
	temp->files = file_list;
	insert_el_head(visited, temp);
	return 1;
}

char *compute_hash(char **cmd){
	int pipe_fd[2];
	char *digest = calloc(HASH_SIZE, sizeof(char));
	
	if(pipe(pipe_fd) == -1){
		perror("Pipe error");
		return NULL;
	}
	
	pid_t cpid = fork();
	int status;
	
	if(cpid == -1){
		perror("Fork failure");
		return NULL;
	}
	
	//child process
	if(!cpid){
		close(pipe_fd[0]);
		dup2(pipe_fd[1], 1); //redirect stdout (1) to the pipe
		//compute hash, write to pipe
		execvp(cmd[0], cmd);
		close(pipe_fd[1]);
	}
	else{
		char buf;
		//read hash
		int i = 0;
		close(pipe_fd[1]);
		while(read(pipe_fd[0], &buf, 1) > 0 && i < HASH_SIZE){
			//printf("%d. BUF: %c\n", (i+1), buf);
			digest[i] = buf;
			++i;
		}
		close(pipe_fd[0]);
		wait(&status);
	}
	return digest;
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
		char *p_cmd[3] = { MD5_PATH, (char *)path, (char *)NULL };
		char *hash = compute_hash(p_cmd);
		visit(hash, (char *)path);
		//printf("%s\n", hash);
		//insert_el_head(visited, path);
	}
	return 0;
}

int main(int argc, char **argv){
	to_visit = create_stack();
	visited = create_empty_list();
	
	char *path = NULL;
	if(argc > 2){
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
	
	print_results();
	
	
	return 0;
}
