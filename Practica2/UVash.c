#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

//--- Start Structures definitions ---

typedef struct command{
	char *command;
	char **arguments;
	char *outFile;
} Command;

//--- End Structures definitios ---

//--- Start declarations ---

bool checkIfArgvIsFile(char *argv[]);
void runFiles();
void printError();
char *readCommand();
void deleteNewlineCharacter();
void removeWhiteSpaces(char *command);
Command *prepareCommand(char *command);
bool executeCommand(Command *command);
bool builtInCommands(Command *command);	


// --- End declarations ---

// --- Start functions definitions ---

bool checkIfArgvIsFile(char *argv[]){
	FILE *fp = fopen(argv[1],"r");
	if(fp == NULL) return false;
	return true;
}

void runFile(char *argv[]){
	FILE *fp = fopen(argv[1],"r");
	char *line = NULL;
	ssize_t nread;
	size_t size = 0;
	while((nread = getline(&line,&size,fp)) != -1){
		deleteNewlineCharacter(line);
		executeCommand(prepareCommand(line));
	}
	exit(0);
}

void printError(){
	char error_message[30] = "An error has occurred\n";
	fprintf(stderr, "%s", error_message);
}

char *readCommand(){
	char* command;
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

void removeWhiteSpaces(char *command) {
    size_t n = strspn(command, " \t\n\r\f\v");
    if (n > 0) {
        memmove(command, command + n, strlen(command + n) + 1);
    }
}

Command *prepareCommand(char *command){
	char *copyCommand = strdup(command);
	//Check if the line start with whitespace
	removeWhiteSpaces(copyCommand);
	if(isspace((unsigned char)copyCommand[0])){
		printError();
		exit(0);
	}

	Command *preprocessCommand = (Command *)malloc(sizeof(Command));
	preprocessCommand->arguments = (char **)malloc(sizeof(char *) * 10);
	int i = 1;
	
	//Capture of the first element of the command	
	char *commandSplited = strtok(copyCommand," \t");
	preprocessCommand->command = strdup(commandSplited);

	//Capture of the arguments of the command
	preprocessCommand->arguments[0] = preprocessCommand->command;
	while((commandSplited = strtok(NULL," \t")) != NULL){
		if (strcmp(commandSplited, ">") == 0) {
            		char *next = strtok(NULL, " \t");
            		if (next){
			       	preprocessCommand->outFile = strdup(next);
				if((commandSplited = strtok(NULL," \t")) != NULL){
					printError();
					exit(0);
				}
				break;
			}
			else{
				printError();
				exit(0);
			}
        	}else{
			preprocessCommand->arguments[i++] = strdup(commandSplited);
		}
	}
	preprocessCommand->arguments[i]=NULL;
	return preprocessCommand;
}

bool executeCommand(Command *command){
	int status;
	if(builtInCommands(command)) return true;
	pid_t pid = fork();
	if(pid == -1){
		printError();
		exit(1);
	}
	if(pid == 0){
		if (command->outFile != NULL) {
			printf("Hace esto otro");
			FILE *fp = fopen(command->outFile, "w");
			dup2(fileno(fp),1);
			dup2(fileno(fp),2);
		}
       		if(execvp(command->command,command->arguments) == -1){
			printError();
		       	exit(1);
		}	
		exit(0);
	}else{
		wait(&status);
	}
	return true;
}


bool builtInCommands(Command *command){
	if(strcmp(command->command,"exit")==0){
	       	if(command->arguments[1]==NULL) exit(0);
		else printError();
		return true;
	}
	if(strcmp(command->command,"cd") == 0){ 
		if(command->arguments[1]!=NULL && command->arguments[2]==NULL)chdir(command->arguments[1]); 
		else printError();
		return true;
	}
	return false;
}

// --- End function definicion ---

// --- Start main function ---

int main (int argc, char *argv[]){
	char* command;
	if(argc == 2){
	       	if(checkIfArgvIsFile(argv)) runFile(argv);
		else{
			printError();
			exit(1);
		}
	}
	if(argc > 2){
	       	printError();
		exit(1);
	}
	while(true){
		command = readCommand();
		if(!executeCommand(prepareCommand(command))) printError();
	}
}

// --- End main function ---

