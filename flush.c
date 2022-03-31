#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

char commands[][100] = {"help", "cd", "user"};

int ownCommands(char** parsedArgs);
int status;
int isWait = 1;
struct PIDs {
  pid_t PID;
  struct PIDs* next;
};

//method for "help".
int showCommands() {
  int state;
  pid_t PID = fork();
  if (PID == 0) {
    printf("Available commands: \n");
    int len = sizeof(commands)/sizeof(commands[0]);
    for (int i = 0; i < len; i++) {
      printf("- %s\n", commands[i]);
    }
    exit(0);
  } else {
    waitpid(-1, &state, 0);
    return state;
  }
}

//method for "user".
int printUser() {
  int state;
  pid_t PID = fork();
  if (PID == 0) {
    printf("User: %s\n", getenv("USER"));
    exit(0);
  } else {
    waitpid(-1, &state, 0);
    return state;
  }
}

//method for "cd".
int changeDirectory(char** parsedArgs) {
  char* formatted;
  if (parsedArgs[1][0] == '/') {
  formatted = parsedArgs[1] + 1;
  } else {
  formatted = parsedArgs[1];
  }
  return chdir(formatted);
}
  

void printCurrentDirectory() {
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  printf("%s: ", cwd);
}

int takeInput(char* input) {
  char* buffer;
  buffer = readline("\n>>>");
  if (buffer == NULL) {
    return -1;
  }
  if (strlen(buffer) != 0) {
    add_history(buffer);
    strcpy(input, buffer);
    free(buffer);
    return 0;
  } else {
  return 1;
  }
} 

int processString(char* input, char** parsedArgs) {
  char* containsAnd = strchr(input, '&');
  if (containsAnd != NULL) {
    isWait = 0;
    input[strlen(input) - 1] = 0;
  } else {
    isWait = 1;
  }
  for (int i = 0; i < 100; i++) {
    parsedArgs[i] = strsep(&input, " ");
    if (parsedArgs[i] == NULL) {
      break;
    }
    if (strlen(parsedArgs[i]) == 0) {
      i--;
    }
  }
  //sjekker om args inneholder < eller >
  for (int i = 0; i < sizeof(parsedArgs)/sizeof(parsedArgs[0]); i++) {
    if ((parsedArgs[i] == "<") || (parsedArgs[i] == ">")) {
      return 2;
    }
  }
  if (ownCommands(parsedArgs)) {
    return 0;
  } else {
    return 1;
  }
}

int ownCommands(char** parsedArgs) {
  int num = 3;
  int argNum;
  
  for (int i = 0; i < num; i++) {
    if (strcmp(parsedArgs[0], commands[i]) == 0) {
      argNum = i;
      break;
    }
  } 

  switch(argNum) {
    case 0:
      status = showCommands();
      return 1;
    case 1:
      status = changeDirectory(parsedArgs);
      return 1;
    case 2:
      status = printUser();
      return 1;
    default: 
      break;
  }
  return 0;
}

int executeCommand(char** parsedArgs) {
  int state;
  int len = sizeof(parsedArgs)/sizeof(parsedArgs[0]);
  pid_t PID = fork();
  if (PID == 0) {
    //må returnere execvp for å få state til å bli null med &
    if (execvp(parsedArgs[0], parsedArgs) < 0) {
      printf("Could not execute program. For information about available commands, type 'help'\n");
    }
    exit(0);
  } else {
    if (isWait) {
      waitpid(-1, &state, 0);
      printf("PID %d \n", getpid());
    }
    return state;
  }
}

//main
int main() {
  char input[100], *parsedArgs[100];
  int execFlag = 0;

  while (1) {
    printCurrentDirectory();
    int inputValue = takeInput(input);

    if (inputValue == -1) {
      break;
    } else if (inputValue == 1) {
      continue;
    } else {
      execFlag = processString(input, parsedArgs);
      if (execFlag == 1) {
        status = executeCommand(parsedArgs);
      }
      printf("Exit status [%s] = %i\n", parsedArgs[0], status);
    }
  }

  printf("\nByeBye!\n");
  return 0;
}