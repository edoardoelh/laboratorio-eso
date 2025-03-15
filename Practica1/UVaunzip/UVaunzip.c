#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int currentCharCount[1];
	char character[1];

	if(argc <= 1){
		printf("%s\n", "UVaunzip: file1 [file2 ...]"); 
		exit(1); 
	}	
	for(int i= 1;i<argc;i++){ 
		FILE *fp = fopen(argv[i], "r"); 
		if(fp == NULL){
			printf("%s\n", "UVaunzip: file1 [file2 ...]");
			exit(1);
		}
		else{
			int lectura = 0;
			do{					
				lectura = fread(currentCharCount, 4, 1, fp);
				lectura = fread(character, 1, 1, fp);
				if(lectura!=0){
					for(int j = 0;j<currentCharCount[0];j++){
						printf("%c",character[0]);
					}	
				}
			}while(lectura!=0);
		}
		fclose(fp);
	}	
	return(0);
}
