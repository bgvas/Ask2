/***************************************************************************//**
  @file     Ask2.c
  @authors  Basilis Georgoulas A.M 2118147 : & Ioannis Skoumpas A.M : 2118063
*******************************************************************************/

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "head.h"



#define RL_BUFSIZE 255

// read a command without arguments
char *read_command(void)
{
  int bufsize = RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "uthsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    
    c = getchar(); // Read a character

	// If character is space then return
    if (c == '\n' || c == 32) {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;
  }
}


// Loop getting input and executing it.
void loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("uthsh1> ");
    line = read_command();
    args = split_line(line);
    status = execute(args);
    //free(line);
    //free(args);
  } while (status != EOF);

do {
    printf("uthsh2> ");
    line = read_line();
    args = split_line(line);
    status = execute(args);
    //free(line);
    //free(args);
  } while (status != EOF);

  do {
    printf("uthsh3> ");
    line = read_line();
    args = split_line(line);
    status = execute(args);
    //free(line);
    //free(args);
  } while (status != EOF);

  do {
    printf("uthsh4> ");
    line = read_line();
    args = split_line(line);
    status = execute(args);
    free(line);
    free(args);
  } while (status != EOF);
}


// The Main Function
int main(int argc, char **argv)
{
  loop();
  return EXIT_SUCCESS;
}
