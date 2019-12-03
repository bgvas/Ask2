/***************************************************************************//**

  @file     Ask2.c

  @authors  Basilis Georgoulas A.M : & Ioannis Skoumpas A.M : 2118063

*******************************************************************************/

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
  Function Declarations for builtin shell commands:
 */
int cd_command(char **args);

/*
  List of builtin commands, followed by the corresponding function.
 */
char *builtin_str[] = {
  "cd"
};

int (*builtin_func[]) (char **) = {
  &cd_command
};

int num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/**
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */
int cd_command(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "uth: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("uth");
    }
  }
  return 1;
}


/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    /*When execvp() is executed, the program file given by the first argument will be loaded into the
    caller's address space and over-write the program there. Then, the second argument will be provided to the
    program and starts the execution. As a result, once the specified program file starts its execution, 
    the original program in the caller's address space is gone and is replaced by the new program. */
    if (execvp(args[0], args) == -1) {
      perror("uthsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("uthsh");
  } else {
    // Parent process
    do { //WUNTRACED return if a child has stopped
      wpid = waitpid(pid, &status, WUNTRACED); //Suspends the calling process until a child process ends or is stopped
    } while (!WIFEXITED(status) && !WIFSIGNALED(status)); //determines whether the child and the parent process ended normally.
  }

  return 1;
}

/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int execute(char **args)
{
  int i = 0;

  if (args[0] == NULL) {
    // An empty command was entered.
    printf("\n");
    return EOF;
  }else if(strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  return launch(args);
}

#define RL_BUFSIZE 255
/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *read_line(void)
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
    // Read a character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "uthsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

#define RL_BUFSIZE 255
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
    // Read a character
    c = getchar();

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

#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **split_line(char *line)
{
  int bufsize = TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char));
  char *token = malloc(bufsize * sizeof(char));

  if (!tokens) {
    fprintf(stderr, "uthsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOK_DELIM);
  while (token != NULL) {
    
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "uthsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
    token = strtok(NULL, TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

/**
   @brief Loop getting input and executing it.
 */
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

    free(line);
    free(args);
  } while (status!= EOF);

  do {
    printf("uthsh2> ");
    line = read_line();
    args = split_line(line);
    status = execute(args);

    free(line);
    free(args);
  } while (status!= EOF);


  do {
    printf("uthsh3> ");
    line = read_line();
    args = split_line(line);
    status = execute(args);

    free(line);
    free(args);
  } while (status!= EOF);
  
  do {
    printf("uthsh4> ");
    line = read_line();
    args = split_line(line);
    status = execute(args);

    free(line);
    free(args);
  } while (status!= EOF);
  
}

/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}
