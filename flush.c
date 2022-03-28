#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

char history[10][100];
char commands[][100] = {"help", "cd", "ls"};

void showCommands() {
  printf("Available commands: \n");
  int len = sizeof(commands)/sizeof(commands[0]);
  for (int i = 0; i < len; i++) {
    printf("%s\n", commands[i]);
  }
}

void printCurrentDirectory() {
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  printf("%s: ", cwd);
}

void executeCommand(char input[10]) {
  pid_t PID = fork();
  int status;
  if (PID != 0) {
    //parent fork
    waitpid(-1, &status, 0);

  } else {
    //child fork
    if (strcmp(input, "help") == 0) {
      showCommands();
    } else if (strcmp(input, "cd") == 0){
      //TODO cd
    } else if (strcmp(input, "ls") == 0){
      //TODO ls:
    } else {
      printf("command not available. For information about available commands, type 'help'. \n");
    }
    exit(0); 
  }
  printf("Exit status [%s] = %i\n", input, status);
}

int main() {
  char input[10];

  while (1) {
    printCurrentDirectory();
    if (scanf("%s", input) == EOF) {
      break;
    }
    executeCommand(input);
  }
  printf("\nCompiled\n");
  return 0;
}