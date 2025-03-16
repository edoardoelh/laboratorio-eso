#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

//--- Start declarations ---

bool checkIfArgvIsFile(char *argv[]);
void runFiles();
void printError();
char *readCommand();
void deleteNewlineCharacter();
bool executeCommand(char *command);

// --- End declarations ---

// --- Start functions definitions ---

bool checkIfArgvIsFile(char *argv[]){
	FILE *fp = fopen(argv[1],"r");
	if(fp == NULL) return false;
	return true;
}

void runFiles(){
	printf("Es un archivo\n");
	exit(0);
}

void printError(){
	char error_message[30] = "An error has occurred\n";
	fprintf(stderr, "%s", error_message);
}

char *readCommand(){
	char *command;
	size_t numBytes = 0;
	printf("UVash>");
	getline(&command,&numBytes,stdin);
	deleteNewlineCharacter(command);
	return command;
}

void deleteNewlineCharacter(char *text){
	size_t len = strlen(text);
	if(len > 0 && text[len-1] == '\n') text[len-1] = '\0'; 
}

bool executeCommand(char* command){
	char* argumentList[] = {command, NULL};	
	if(execvp(command,argumentList) == -1) return false;
	return true;
}

// --- End function definicion ---

// --- Start main function ---

int main (int argc, char *argv[]){
	char *command;	
	bool exit = false;//Cuando el while sea un bucle infitino (si funciona el exit) se tendra que borrar
	while(!exit){//Esto despues tendra que ser un bucle infinito
		if(argc > 1) if(checkIfArgvIsFile(argv)) runFiles();
		command = readCommand();
		if(!executeCommand(command)) printError();
		exit = true; //Esto actualmente esta para no probocar un bucle infinito
	}
}

// --- End main function ---

