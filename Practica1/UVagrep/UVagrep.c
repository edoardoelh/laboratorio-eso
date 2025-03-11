#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	ssize_t nread;
	if(argc <= 1){
		printf("%s\n", "UVagrep: searchterm [file ...]"); exit(1); 
	}
	if(argc == 2){
		char line[1000];
		while(fgets(line,sizeof(line),stdin)!=NULL){
			if(strstr(line, (char *)argv[1]) != NULL)  
				printf("%s", line);
		}
	}	
	else{	
		for(int i= 2;i<argc;i++){ 
			FILE *fp = fopen(argv[i], "r"); 
			char *line = NULL;
			size_t size = 0;
			if(fp == NULL){
				printf("%s\n", "UVagrep: cannot open file");
				exit(1);
			}
			else{
				while((nread = getline(&line, &size, fp)) != -1){
					if(strstr(line, (char *)argv[1]) != NULL)  printf("%s",line);
				}
			}
			fclose(fp);
		}
	}
	return(0);
}
