#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	for(int i= 1;i<argc;i++){
		FILE *fp = fopen(argv[i],"r");
		char line[256];
		if(fp == NULL){
			printf("%s\n","UVacat: no puedo abrir fichero");
			exit(1);
		}
		else{
			while(fgets(line, sizeof(line), fp)){
				printf("%s",line);
			}
		}
	}
	return(0);
}
