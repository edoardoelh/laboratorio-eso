#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	ssize_t nread;
	int concurrentCharCount = 0;
	char lastCharRead;
	if(argc <= 1){
		printf("%s\n", "UVazip: file1 [file2 ...]"); exit(1); 
	}	
	for(int i= 1;i<argc;i++){ 
		FILE *fp = fopen(argv[i], "r"); 
		char *line = NULL;
		size_t size = 0;
		if(fp == NULL){
			printf("%s\n", "UVazip: file1 [file2 ...]");
			exit(1);
		}
		else{
			while((nread = getline(&line, &size, fp)) != -1){
				//printf("%s",line);
				for(int j=0;j<strlen(line);j++){
					if(!lastCharRead) lastCharRead=line[j];
					if(line[j]!=lastCharRead){
						//printf("%s%d::%d","Debug:",concurrentCharCount,lastCharRead);
						fwrite(&concurrentCharCount,sizeof(int),1,stdout);
               					fwrite(&lastCharRead,sizeof(char),1,stdout);
						concurrentCharCount=0;
						lastCharRead=line[j];
					}
					else{
						//lastCharRead=line[j];
						concurrentCharCount++;
					}
					//printf("\n");
				}
			}
		}
		fclose(fp);
	}	
	//printf("%s%d::%d","Debug:",concurrentCharCount,lastCharRead);
	fwrite(&concurrentCharCount,sizeof(int),1,stdout);
        fwrite(&lastCharRead,sizeof(char),1,stdout);
	return(0);
}
